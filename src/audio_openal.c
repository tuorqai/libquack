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
#include "audio_openal.h"
#include "log.h"
#include "platform.h"

//------------------------------------------------------------------------------

static struct
{
    void *lib;
    struct libqu_openal_func func;

    ALCdevice       *device;
    ALCcontext      *context;
    ALuint          *sources;
} priv;

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

static bool load_openal_library(void)
{
    char const *names[] = {
        "libopenal.so.1",
    };

    for (size_t i = 0; i < sizeof(names) / sizeof(*names); i++) {
        priv.lib = pl_open_dll(names[i]);

        if (priv.lib) {
            break;
        }
    }

    if (!priv.lib) {
        return false;
    }

    alEnable                = pl_get_dll_proc(priv.lib, "alEnable");
    alDisable               = pl_get_dll_proc(priv.lib, "alDisable");
    alIsEnabled             = pl_get_dll_proc(priv.lib, "alIsEnabled");
    alGetString             = pl_get_dll_proc(priv.lib, "alGetString");
    alGetBooleanv           = pl_get_dll_proc(priv.lib, "alGetBooleanv");
    alGetIntegerv           = pl_get_dll_proc(priv.lib, "alGetIntegerv");
    alGetFloatv             = pl_get_dll_proc(priv.lib, "alGetFloatv");
    alGetDoublev            = pl_get_dll_proc(priv.lib, "alGetDoublev");
    alGetBoolean            = pl_get_dll_proc(priv.lib, "alGetBoolean");
    alGetInteger            = pl_get_dll_proc(priv.lib, "alGetInteger");
    alGetFloat              = pl_get_dll_proc(priv.lib, "alGetFloat");
    alGetDouble             = pl_get_dll_proc(priv.lib, "alGetDouble");
    alGetError              = pl_get_dll_proc(priv.lib, "alGetError");
    alIsExtensionPresent    = pl_get_dll_proc(priv.lib, "alIsExtensionPresent");
    alGetProcAddress        = pl_get_dll_proc(priv.lib, "alGetProcAddress");
    alGetEnumValue          = pl_get_dll_proc(priv.lib, "alGetEnumValue");
    alListenerf             = pl_get_dll_proc(priv.lib, "alListenerf");
    alListener3f            = pl_get_dll_proc(priv.lib, "alListener3f");
    alListenerfv            = pl_get_dll_proc(priv.lib, "alListenerfv");
    alListeneri             = pl_get_dll_proc(priv.lib, "alListeneri");
    alListener3i            = pl_get_dll_proc(priv.lib, "alListener3i");
    alListeneriv            = pl_get_dll_proc(priv.lib, "alListeneriv");
    alGetListenerf          = pl_get_dll_proc(priv.lib, "alGetListenerf");
    alGetListener3f         = pl_get_dll_proc(priv.lib, "alGetListener3f");
    alGetListenerfv         = pl_get_dll_proc(priv.lib, "alGetListenerfv");
    alGetListeneri          = pl_get_dll_proc(priv.lib, "alGetListeneri");
    alGetListener3i         = pl_get_dll_proc(priv.lib, "alGetListener3i");
    alGetListeneriv         = pl_get_dll_proc(priv.lib, "alGetListeneriv");
    alGenSources            = pl_get_dll_proc(priv.lib, "alGenSources");
    alDeleteSources         = pl_get_dll_proc(priv.lib, "alDeleteSources");
    alIsSource              = pl_get_dll_proc(priv.lib, "alIsSource");
    alSourcef               = pl_get_dll_proc(priv.lib, "alSourcef");
    alSource3f              = pl_get_dll_proc(priv.lib, "alSource3f");
    alSourcefv              = pl_get_dll_proc(priv.lib, "alSourcefv");
    alSourcei               = pl_get_dll_proc(priv.lib, "alSourcei");
    alSource3i              = pl_get_dll_proc(priv.lib, "alSource3i");
    alSourceiv              = pl_get_dll_proc(priv.lib, "alSourceiv");
    alGetSourcef            = pl_get_dll_proc(priv.lib, "alGetSourcef");
    alGetSource3f           = pl_get_dll_proc(priv.lib, "alGetSource3f");
    alGetSourcefv           = pl_get_dll_proc(priv.lib, "alGetSourcefv");
    alGetSourcei            = pl_get_dll_proc(priv.lib, "alGetSourcei");
    alGetSource3i           = pl_get_dll_proc(priv.lib, "alGetSource3i");
    alGetSourceiv           = pl_get_dll_proc(priv.lib, "alGetSourceiv");
    alSourcePlayv           = pl_get_dll_proc(priv.lib, "alSourcePlayv");
    alSourceStopv           = pl_get_dll_proc(priv.lib, "alSourceStopv");
    alSourceRewindv         = pl_get_dll_proc(priv.lib, "alSourceRewindv");
    alSourcePausev          = pl_get_dll_proc(priv.lib, "alSourcePausev");
    alSourcePlay            = pl_get_dll_proc(priv.lib, "alSourcePlay");
    alSourceStop            = pl_get_dll_proc(priv.lib, "alSourceStop");
    alSourceRewind          = pl_get_dll_proc(priv.lib, "alSourceRewind");
    alSourcePause           = pl_get_dll_proc(priv.lib, "alSourcePause");
    alSourceQueueBuffers    = pl_get_dll_proc(priv.lib, "alSourceQueueBuffers");
    alSourceUnqueueBuffers  = pl_get_dll_proc(priv.lib, "alSourceUnqueueBuffers");
    alGenBuffers            = pl_get_dll_proc(priv.lib, "alGenBuffers");
    alDeleteBuffers         = pl_get_dll_proc(priv.lib, "alDeleteBuffers");
    alIsBuffer              = pl_get_dll_proc(priv.lib, "alIsBuffer");
    alBufferData            = pl_get_dll_proc(priv.lib, "alBufferData");
    alBufferf               = pl_get_dll_proc(priv.lib, "alBufferf");
    alBuffer3f              = pl_get_dll_proc(priv.lib, "alBuffer3f");
    alBufferfv              = pl_get_dll_proc(priv.lib, "alBufferfv");
    alBufferi               = pl_get_dll_proc(priv.lib, "alBufferi");
    alBuffer3i              = pl_get_dll_proc(priv.lib, "alBuffer3i");
    alBufferiv              = pl_get_dll_proc(priv.lib, "alBufferiv");
    alGetBufferf            = pl_get_dll_proc(priv.lib, "alGetBufferf");
    alGetBuffer3f           = pl_get_dll_proc(priv.lib, "alGetBuffer3f");
    alGetBufferfv           = pl_get_dll_proc(priv.lib, "alGetBufferfv");
    alGetBufferi            = pl_get_dll_proc(priv.lib, "alGetBufferi");
    alGetBuffer3i           = pl_get_dll_proc(priv.lib, "alGetBuffer3i");
    alGetBufferiv           = pl_get_dll_proc(priv.lib, "alGetBufferiv");
    alDopplerFactor         = pl_get_dll_proc(priv.lib, "alDopplerFactor");
    alDopplerVelocity       = pl_get_dll_proc(priv.lib, "alDopplerVelocity");
    alSpeedOfSound          = pl_get_dll_proc(priv.lib, "alSpeedOfSound");
    alDistanceModel         = pl_get_dll_proc(priv.lib, "alDistanceModel");

    alcCreateContext        = pl_get_dll_proc(priv.lib, "alcCreateContext");
    alcMakeContextCurrent   = pl_get_dll_proc(priv.lib, "alcMakeContextCurrent");
    alcProcessContext       = pl_get_dll_proc(priv.lib, "alcProcessContext");
    alcSuspendContext       = pl_get_dll_proc(priv.lib, "alcSuspendContext");
    alcDestroyContext       = pl_get_dll_proc(priv.lib, "alcDestroyContext");
    alcGetCurrentContext    = pl_get_dll_proc(priv.lib, "alcGetCurrentContext");
    alcGetContextsDevice    = pl_get_dll_proc(priv.lib, "alcGetContextsDevice");
    alcOpenDevice           = pl_get_dll_proc(priv.lib, "alcOpenDevice");
    alcCloseDevice          = pl_get_dll_proc(priv.lib, "alcCloseDevice");
    alcGetError             = pl_get_dll_proc(priv.lib, "alcGetError");
    alcIsExtensionPresent   = pl_get_dll_proc(priv.lib, "alcIsExtensionPresent");
    alcGetProcAddress       = pl_get_dll_proc(priv.lib, "alcGetProcAddress");
    alcGetEnumValue         = pl_get_dll_proc(priv.lib, "alcGetEnumValue");
    alcGetString            = pl_get_dll_proc(priv.lib, "alcGetString");
    alcGetIntegerv          = pl_get_dll_proc(priv.lib, "alcGetIntegerv");
    alcCaptureOpenDevice    = pl_get_dll_proc(priv.lib, "alcCaptureOpenDevice");
    alcCaptureCloseDevice   = pl_get_dll_proc(priv.lib, "alcCaptureCloseDevice");
    alcCaptureStart         = pl_get_dll_proc(priv.lib, "alcCaptureStart");
    alcCaptureStop          = pl_get_dll_proc(priv.lib, "alcCaptureStop");
    alcCaptureSamples       = pl_get_dll_proc(priv.lib, "alcCaptureSamples");

    return alEnable
        && alDisable
        && alIsEnabled
        && alGetString
        && alGetBooleanv
        && alGetIntegerv
        && alGetFloatv
        && alGetDoublev
        && alGetBoolean
        && alGetInteger
        && alGetFloat
        && alGetDouble
        && alGetError
        && alIsExtensionPresent
        && alGetProcAddress
        && alGetEnumValue
        && alListenerf
        && alListener3f
        && alListenerfv
        && alListeneri
        && alListener3i
        && alListeneriv
        && alGetListenerf
        && alGetListener3f
        && alGetListenerfv
        && alGetListeneri
        && alGetListener3i
        && alGetListeneriv
        && alGenSources
        && alDeleteSources
        && alIsSource
        && alSourcef
        && alSource3f
        && alSourcefv
        && alSourcei
        && alSource3i
        && alSourceiv
        && alGetSourcef
        && alGetSource3f
        && alGetSourcefv
        && alGetSourcei
        && alGetSource3i
        && alGetSourceiv
        && alSourcePlayv
        && alSourceStopv
        && alSourceRewindv
        && alSourcePausev
        && alSourcePlay
        && alSourceStop
        && alSourceRewind
        && alSourcePause
        && alSourceQueueBuffers
        && alSourceUnqueueBuffers
        && alGenBuffers
        && alDeleteBuffers
        && alIsBuffer
        && alBufferData
        && alBufferf
        && alBuffer3f
        && alBufferfv
        && alBufferi
        && alBuffer3i
        && alBufferiv
        && alGetBufferf
        && alGetBuffer3f
        && alGetBufferfv
        && alGetBufferi
        && alGetBuffer3i
        && alGetBufferiv
        && alDopplerFactor
        && alDopplerVelocity
        && alSpeedOfSound
        && alDistanceModel
        && alcCreateContext
        && alcMakeContextCurrent
        && alcProcessContext
        && alcSuspendContext
        && alcDestroyContext
        && alcGetCurrentContext
        && alcGetContextsDevice
        && alcOpenDevice
        && alcCloseDevice
        && alcGetError
        && alcIsExtensionPresent
        && alcGetProcAddress
        && alcGetEnumValue
        && alcGetString
        && alcGetIntegerv
        && alcCaptureOpenDevice
        && alcCaptureCloseDevice
        && alcCaptureStart
        && alcCaptureStop
        && alcCaptureSamples;
}

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
    if (!load_openal_library()) {
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
