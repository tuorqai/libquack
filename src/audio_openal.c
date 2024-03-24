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

static ALenum choose_format(struct libqu_audio_format const *format)
{
    if (format->channels == 1) {
        return AL_FORMAT_MONO16;
    } else if (format->channels == 2) {
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

static int audio_openal_create_sound(struct libqu_sound *sound)
{
    ALenum format = choose_format(&sound->wave->format);

    if (format == AL_INVALID_ENUM) {
        return -1;
    }

    ALuint source;
    ALuint buffer;

    _AL(alGenSources(1, &source));
    _AL(alGenBuffers(1, &buffer));

    if (!source || !buffer) {
        _AL(alDeleteSources(1, &source));
        _AL(alDeleteBuffers(1, &buffer));

        return -1;
    }

    ALvoid *data = sound->wave->buffer;
    ALsizei size = sound->wave->size * sizeof(*sound->wave->buffer);
    ALsizei freq = sound->wave->format.rate;

    _AL(alBufferData(buffer, format, data, size, freq));
    _AL(alSourcei(source, AL_BUFFER, buffer));

    sound->priv[0] = source;
    sound->priv[1] = buffer;

    return 0;
}

static void audio_openal_destroy_sound(struct libqu_sound *sound)
{
    ALuint source = sound->priv[0];
    ALuint buffer = sound->priv[1];

    _AL(alDeleteSources(1, &source));
    _AL(alDeleteBuffers(1, &buffer));
}

static qu_playback_state audio_openal_get_sound_state(struct libqu_sound *sound)
{
    ALuint source = sound->priv[0];

    ALint state;
    _AL(alGetSourcei(source, AL_SOURCE_STATE, &state));

    switch (state) {
    case AL_INITIAL:
    case AL_STOPPED:
        return QU_PLAYBACK_STOPPED;
    case AL_PLAYING:
        return QU_PLAYBACK_PLAYING;
    case AL_PAUSED:
        return QU_PLAYBACK_PAUSED;
    }

    return QU_PLAYBACK_INVALID;
}

static void audio_openal_set_sound_loop(struct libqu_sound *sound, int loop)
{
    ALint value = (loop == -1) ? AL_TRUE : AL_FALSE;
    _AL(alSourcei((ALuint) sound->priv[0], AL_LOOPING, value));
}

static void audio_openal_set_sound_state(struct libqu_sound *sound, qu_playback_state state)
{
    switch (state) {
    case QU_PLAYBACK_STOPPED:
        _AL(alSourceStop((ALuint) sound->priv[0]));
        break;
    case QU_PLAYBACK_PLAYING:
        _AL(alSourcePlay((ALuint) sound->priv[0]));
        break;
    case QU_PLAYBACK_PAUSED:
        _AL(alSourcePause((ALuint) sound->priv[0]));
        break;
    default:
        break;
    }
}

static int audio_openal_create_music(struct libqu_music *music)
{
    ALenum format = choose_format(&music->sndfile->format);

    if (format == AL_INVALID_ENUM) {
        LIBQU_LOGE("init_source: invalid format (%d channels, %d rate)\n",
            music->sndfile->format.channels,
            music->sndfile->format.rate);
        return -1;
    }

    ALuint source;
    _AL(alGenSources(1, &source));

    if (source == 0) {
        LIBQU_LOGE("create_music: alGenSources() failed\n");
        return -1;
    }

    _AL(alSourcei(source, AL_LOOPING, AL_FALSE));
    _AL(alSourcei(source, AL_BUFFER, 0));

    music->priv[0] = (intptr_t) source;
    music->priv[1] = (intptr_t) format;

    return 0;
}

static void audio_openal_destroy_music(struct libqu_music *music)
{
    ALuint source = (ALuint) music->priv[0];
    _AL(alDeleteSources(1, &source));
}

static int audio_openal_enqueue_music_buffer(struct libqu_music *music, int16_t const *buffer, size_t samples)
{
    ALuint source = (ALuint) music->priv[0];
    ALenum format = (ALenum) music->priv[1];

    ALuint al_buffer;
    _AL(alGenBuffers(1, &al_buffer));

    if (al_buffer == 0) {
        LIBQU_LOGE("enqueue_music_buffer: alGenBuffers() failed\n");
        return -1;
    }

    _AL(alBufferData(al_buffer, format, buffer, sizeof(*buffer) * samples,
        music->sndfile->format.rate));

    _AL(alSourceQueueBuffers(source, 1, &al_buffer));

    return 0;
}

static int audio_openal_dequeue_played_music_buffers(struct libqu_music *music)
{
    ALuint source = (ALuint) music->priv[0];

    ALint processed;
    _AL(alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed));

    if (processed == 0) {
        return 0;
    }

    ALuint buffers[LIBQU_MUSIC_BUFFERS];

    // Remove processed buffers from the queue.
    _AL(alSourceUnqueueBuffers(source, processed, buffers));

    // Delete those buffer altogether.
    _AL(alDeleteBuffers(processed, buffers));

    // Return the number of buffers that were removed from the queue.
    return processed;
}

static void audio_openal_dequeue_all_music_buffers(struct libqu_music *music)
{
    ALuint source = (ALuint) music->priv[0];

    ALint queued;
    _AL(alGetSourcei(source, AL_BUFFERS_QUEUED, &queued));

    ALuint buffers[LIBQU_MUSIC_BUFFERS];

    _AL(alSourceUnqueueBuffers(source, queued, buffers));
    _AL(alDeleteBuffers(queued, buffers));
}

static qu_playback_state audio_openal_get_music_state(struct libqu_music *music)
{
    ALuint source = (ALuint) music->priv[0];

    if (source == 0) {
        return QU_PLAYBACK_STOPPED;
    }

    ALint state;
    _AL(alGetSourcei(source, AL_SOURCE_STATE, &state));

    switch (state) {
    default:
        return QU_PLAYBACK_STOPPED;
    case AL_PLAYING:
        return QU_PLAYBACK_PLAYING;
    case AL_PAUSED:
        return QU_PLAYBACK_PAUSED;
    }
}

static void audio_openal_set_music_state(struct libqu_music *music, qu_playback_state state)
{
    ALuint source = (ALuint) music->priv[0];

    if (source == 0) {
        return;
    }

    switch (state) {
    case QU_PLAYBACK_STOPPED:
        _AL(alSourceStop(source));
        break;
    case QU_PLAYBACK_PLAYING:
        _AL(alSourcePlay(source));
        break;
    case QU_PLAYBACK_PAUSED:
        _AL(alSourcePause(source));
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

struct libqu_audio_impl const libqu_audio_openal_impl = {
    audio_openal_check_if_available,
    audio_openal_initialize,
    audio_openal_terminate,
    audio_openal_set_master_volume,
    audio_openal_create_sound,
    audio_openal_destroy_sound,
    audio_openal_get_sound_state,
    audio_openal_set_sound_loop,
    audio_openal_set_sound_state,
    audio_openal_create_music,
    audio_openal_destroy_music,
    audio_openal_enqueue_music_buffer,
    audio_openal_dequeue_played_music_buffers,
    audio_openal_dequeue_all_music_buffers,
    audio_openal_get_music_state,
    audio_openal_set_music_state,
};

//------------------------------------------------------------------------------

#endif // QU_USE_OPENAL
