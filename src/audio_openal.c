//------------------------------------------------------------------------------
// Copyright (c) 2021-2024 tuorqai
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//------------------------------------------------------------------------------

#ifdef QU_USE_OPENAL

//------------------------------------------------------------------------------

#include <stb_ds.h>
#include "audio.h"
#include "dyn_openal.h"
#include "log.h"
#include "platform.h"

//------------------------------------------------------------------------------

#ifdef NDEBUG
    #define _AL(call) call
#else
    #define _AL(call) \
        do { \
            call; \
            _al(#call, __FILE__, __LINE__); \
        } while (0);

    static char const *_alerrstr(ALenum error)
    {
        switch (error) {
        case AL_INVALID_NAME:
            return "AL_INVALID_NAME";
        case AL_INVALID_ENUM:
            return "AL_INVALID_ENUM";
        case AL_INVALID_VALUE:
            return "AL_INVALID_VALUE";
        case AL_INVALID_OPERATION:
            return "AL_INVALID_OPERATION";
        case AL_OUT_OF_MEMORY:
            return "AL_OUT_OF_MEMORY";
        default:
            return NULL;
        }
    }

    static void _al(char const *call, char const *file, int line)
    {
        ALenum error = alGetError();

        if (error == AL_NO_ERROR) {
            return;
        }

        LIBQU_LOGE("OpenAL error(s) occurred in %s:\n", file);
        LIBQU_LOGE("%4d: %s\n", line, call);

        do {
            char const *str = _alerrstr(error);
            if (str) {
                LIBQU_LOGE("-- %s\n", str);
            } else {
                LIBQU_LOGE("-- 0x%04x\n", error);
            }
        } while ((error = alGetError()) != AL_NO_ERROR);
    }
#endif

//------------------------------------------------------------------------------

static struct
{
    void        *dll;
    ALCdevice   *device;
    ALCcontext  *context;
    ALuint      *sources;
} priv;

//------------------------------------------------------------------------------

static ALenum choose_format(int channels)
{
    if (channels == 1) {
        return AL_FORMAT_MONO16;
    } else if (channels == 2) {
        return AL_FORMAT_STEREO16;
    }

    return AL_INVALID_ENUM;
}

//------------------------------------------------------------------------------

static bool audio_openal_check_if_available(void)
{
    priv.dll = dyn_load_openal();
    
    if (!priv.dll) {
        return false;
    }

    priv.device = alcOpenDevice(NULL);

    if (!priv.device) {
        LIBQU_LOGW("alcOpenDevice() returned NULL.\n");
        return false;
    }

    LIBQU_LOGI("alcOpenDevice() successful.\n");

    return true;
}

static bool audio_openal_initialize(struct libqu_audio_params const *params)
{
    priv.context = alcCreateContext(priv.device, NULL);

    if (!priv.context) {
        LIBQU_LOGE("Failed to create OpenAL context.\n");
        return false;
    }

    if (!alcMakeContextCurrent(priv.context)) {
        LIBQU_LOGE("Failed to activate OpenAL context.\n");
        return false;
    }

    arrsetlen(priv.sources, 64);
    _AL(alGenSources(arrlen(priv.sources), priv.sources));

    LIBQU_LOGI("Initialized.\n");

    LIBQU_LOGI("AL_VENDOR: %s\n", alGetString(AL_VENDOR));
    LIBQU_LOGI("AL_VERSION: %s\n", alGetString(AL_VERSION));
    LIBQU_LOGI("AL_RENDERER: %s\n", alGetString(AL_RENDERER));
    LIBQU_LOGI("AL_EXTENSIONS: %s\n", alGetString(AL_EXTENSIONS));

    return true;
}

static void audio_openal_terminate(void)
{
    _AL(alDeleteSources(64, priv.sources));
    alcDestroyContext(priv.context);
    alcCloseDevice(priv.device);

    pl_close_dll(priv.dll);

    LIBQU_LOGI("Terminated.\n");
}

static void audio_openal_set_master_volume(float volume)
{
    _AL(alListenerf(AL_GAIN, volume));
}

static qu_handle audio_openal_load_buffer(struct libqu_wave *wave)
{
    ALenum format = choose_format(wave->channel_count);

    if (format == AL_INVALID_ENUM) {
        LIBQU_LOGE("load_buffer(): invalid format (channels=%d)\n",
            wave->channel_count);
        return -1;
    }

    ALuint buffer;
    _AL(alGenBuffers(1, &buffer));

    if (buffer == 0) {
        LIBQU_LOGE("load_buffer(): buffer == 0\n");
        return -1;
    }

    ALvoid *data = wave->samples;
    ALsizei size = wave->sample_count * sizeof(*wave->samples);
    ALsizei freq = wave->sample_rate;

    _AL(alBufferData(buffer, format, data, size, freq));

    return (qu_handle) buffer;
}

static void audio_openal_unload_buffer(qu_handle buffer_id)
{
    _AL(alDeleteBuffers(1, (ALuint *) &buffer_id));
}

enum libqu_voice_state audio_openal_get_voice_state(qu_handle voice_id)
{
    if (voice_id < 0 || voice_id >= arrlen(priv.sources)) {
        LIBQU_LOGE("get_voice_state(): out of bounds\n");
        return LIBQU_VOICE_INVALID;
    }

    ALint state;
    _AL(alGetSourcei(priv.sources[voice_id], AL_SOURCE_STATE, &state));

    switch (state) {
    case AL_INITIAL:
        return LIBQU_VOICE_INITIAL;
    case AL_PLAYING:
        return LIBQU_VOICE_PLAYING;
    case AL_PAUSED:
        return LIBQU_VOICE_PAUSED;
    case AL_STOPPED:
        return LIBQU_VOICE_STOPPED;
    }

    return LIBQU_VOICE_INVALID;
}

static qu_handle audio_openal_get_voice_buffer(qu_handle voice_id)
{
    if (voice_id < 0 || voice_id >= arrlen(priv.sources)) {
        LIBQU_LOGE("get_voice_buffer(): out of bounds\n");
        return -1;
    }

    ALuint source = priv.sources[voice_id];

    ALint buffer;
    _AL(alGetSourcei(source, AL_BUFFER, &buffer));

    return (qu_handle) buffer;
}

static void audio_openal_set_voice_buffer(qu_handle voice_id, qu_handle buffer_id, int loop)
{
    if (voice_id < 0 || voice_id >= arrlen(priv.sources)) {
        LIBQU_LOGE("set_voice_buffer(): out of bounds\n");
        return;
    }

    ALuint source = priv.sources[voice_id];
    ALuint buffer = (buffer_id == -1) ? 0 : (ALuint) buffer_id;

    ALint state;
    _AL(alGetSourcei(source, AL_SOURCE_STATE, &state));

    if (state == AL_PAUSED) {
        _AL(alSourceStop(source));
    }

    _AL(alSourcei(source, AL_BUFFER, buffer));
    _AL(alSourcei(source, AL_LOOPING, (loop == -1) ? AL_TRUE : AL_FALSE));
}

static int audio_openal_start_voice(qu_handle voice_id)
{
    if (voice_id < 0 || voice_id >= arrlen(priv.sources)) {
        LIBQU_LOGE("start_voice(): out of bounds\n");
        return -1;
    }

    _AL(alSourcePlay(priv.sources[voice_id]));

    ALint state;
    _AL(alGetSourcei(priv.sources[voice_id], AL_SOURCE_STATE, &state));

    return (state == AL_PLAYING) ? 0 : -1;
}

static int audio_openal_stop_voice(qu_handle voice_id)
{
    if (voice_id < 0 || voice_id >= arrlen(priv.sources)) {
        LIBQU_LOGE("stop_voice(): out of bounds\n");
        return -1;
    }

    _AL(alSourcePause(priv.sources[voice_id]));

    ALint state;
    _AL(alGetSourcei(priv.sources[voice_id], AL_SOURCE_STATE, &state));

    return (state == AL_PAUSED) ? 0 : -1;
}

//------------------------------------------------------------------------------

struct libqu_audio_impl const libqu_audio_openal_impl = {
    audio_openal_check_if_available,
    audio_openal_initialize,
    audio_openal_terminate,
    audio_openal_set_master_volume,
    audio_openal_load_buffer,
    audio_openal_unload_buffer,
    audio_openal_get_voice_state,
    audio_openal_get_voice_buffer,
    audio_openal_set_voice_buffer,
    audio_openal_start_voice,
    audio_openal_stop_voice,
};

//------------------------------------------------------------------------------

#endif // QU_USE_OPENAL
