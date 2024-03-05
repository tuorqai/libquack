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

#ifndef LIBQU_DYN_OPENAL_H_INC
#define LIBQU_DYN_OPENAL_H_INC

//------------------------------------------------------------------------------

#define AL_NO_PROTOTYPES

//------------------------------------------------------------------------------

#include <al.h>
#include <alc.h>
#include "log.h"
#include "platform.h"

//------------------------------------------------------------------------------

#define PROC_LIST                                           \
    PROC(LPALENABLE,                alEnable)               \
    PROC(LPALDISABLE,               alDisable)              \
    PROC(LPALISENABLED,             alIsEnabled)            \
    PROC(LPALGETSTRING,             alGetString)            \
    PROC(LPALGETBOOLEANV,           alGetBooleanv)          \
    PROC(LPALGETINTEGERV,           alGetIntegerv)          \
    PROC(LPALGETFLOATV,             alGetFloatv)            \
    PROC(LPALGETDOUBLEV,            alGetDoublev)           \
    PROC(LPALGETBOOLEAN,            alGetBoolean)           \
    PROC(LPALGETINTEGER,            alGetInteger)           \
    PROC(LPALGETFLOAT,              alGetFloat)             \
    PROC(LPALGETDOUBLE,             alGetDouble)            \
    PROC(LPALGETERROR,              alGetError)             \
    PROC(LPALISEXTENSIONPRESENT,    alIsExtensionPresent)   \
    PROC(LPALGETPROCADDRESS,        alGetProcAddress)       \
    PROC(LPALGETENUMVALUE,          alGetEnumValue)         \
    PROC(LPALLISTENERF,             alListenerf)            \
    PROC(LPALLISTENER3F,            alListener3f)           \
    PROC(LPALLISTENERFV,            alListenerfv)           \
    PROC(LPALLISTENERI,             alListeneri)            \
    PROC(LPALLISTENER3I,            alListener3i)           \
    PROC(LPALLISTENERIV,            alListeneriv)           \
    PROC(LPALGETLISTENERF,          alGetListenerf)         \
    PROC(LPALGETLISTENER3F,         alGetListener3f)        \
    PROC(LPALGETLISTENERFV,         alGetListenerfv)        \
    PROC(LPALGETLISTENERI,          alGetListeneri)         \
    PROC(LPALGETLISTENER3I,         alGetListener3i)        \
    PROC(LPALGETLISTENERIV,         alGetListeneriv)        \
    PROC(LPALGENSOURCES,            alGenSources)           \
    PROC(LPALDELETESOURCES,         alDeleteSources)        \
    PROC(LPALISSOURCE,              alIsSource)             \
    PROC(LPALSOURCEF,               alSourcef)              \
    PROC(LPALSOURCE3F,              alSource3f)             \
    PROC(LPALSOURCEFV,              alSourcefv)             \
    PROC(LPALSOURCEI,               alSourcei)              \
    PROC(LPALSOURCE3I,              alSource3i)             \
    PROC(LPALSOURCEIV,              alSourceiv)             \
    PROC(LPALGETSOURCEF,            alGetSourcef)           \
    PROC(LPALGETSOURCE3F,           alGetSource3f)          \
    PROC(LPALGETSOURCEFV,           alGetSourcefv)          \
    PROC(LPALGETSOURCEI,            alGetSourcei)           \
    PROC(LPALGETSOURCE3I,           alGetSource3i)          \
    PROC(LPALGETSOURCEIV,           alGetSourceiv)          \
    PROC(LPALSOURCEPLAYV,           alSourcePlayv)          \
    PROC(LPALSOURCESTOPV,           alSourceStopv)          \
    PROC(LPALSOURCEREWINDV,         alSourceRewindv)        \
    PROC(LPALSOURCEPAUSEV,          alSourcePausev)         \
    PROC(LPALSOURCEPLAY,            alSourcePlay)           \
    PROC(LPALSOURCESTOP,            alSourceStop)           \
    PROC(LPALSOURCEREWIND,          alSourceRewind)         \
    PROC(LPALSOURCEPAUSE,           alSourcePause)          \
    PROC(LPALSOURCEQUEUEBUFFERS,    alSourceQueueBuffers)   \
    PROC(LPALSOURCEUNQUEUEBUFFERS,  alSourceUnqueueBuffers) \
    PROC(LPALGENBUFFERS,            alGenBuffers)           \
    PROC(LPALDELETEBUFFERS,         alDeleteBuffers)        \
    PROC(LPALISBUFFER,              alIsBuffer)             \
    PROC(LPALBUFFERDATA,            alBufferData)           \
    PROC(LPALBUFFERF,               alBufferf)              \
    PROC(LPALBUFFER3F,              alBuffer3f)             \
    PROC(LPALBUFFERFV,              alBufferfv)             \
    PROC(LPALBUFFERI,               alBufferi)              \
    PROC(LPALBUFFER3I,              alBuffer3i)             \
    PROC(LPALBUFFERIV,              alBufferiv)             \
    PROC(LPALGETBUFFERF,            alGetBufferf)           \
    PROC(LPALGETBUFFER3F,           alGetBuffer3f)          \
    PROC(LPALGETBUFFERFV,           alGetBufferfv)          \
    PROC(LPALGETBUFFERI,            alGetBufferi)           \
    PROC(LPALGETBUFFER3I,           alGetBuffer3i)          \
    PROC(LPALGETBUFFERIV,           alGetBufferiv)          \
    PROC(LPALDOPPLERFACTOR,         alDopplerFactor)        \
    PROC(LPALDOPPLERVELOCITY,       alDopplerVelocity)      \
    PROC(LPALSPEEDOFSOUND,          alSpeedOfSound)         \
    PROC(LPALDISTANCEMODEL,         alDistanceModel)        \
    PROC(LPALCCREATECONTEXT,        alcCreateContext)       \
    PROC(LPALCMAKECONTEXTCURRENT,   alcMakeContextCurrent)  \
    PROC(LPALCPROCESSCONTEXT,       alcProcessContext)      \
    PROC(LPALCSUSPENDCONTEXT,       alcSuspendContext)      \
    PROC(LPALCDESTROYCONTEXT,       alcDestroyContext)      \
    PROC(LPALCGETCURRENTCONTEXT,    alcGetCurrentContext)   \
    PROC(LPALCGETCONTEXTSDEVICE,    alcGetContextsDevice)   \
    PROC(LPALCOPENDEVICE,           alcOpenDevice)          \
    PROC(LPALCCLOSEDEVICE,          alcCloseDevice)         \
    PROC(LPALCGETERROR,             alcGetError)            \
    PROC(LPALCISEXTENSIONPRESENT,   alcIsExtensionPresent)  \
    PROC(LPALCGETPROCADDRESS,       alcGetProcAddress)      \
    PROC(LPALCGETENUMVALUE,         alcGetEnumValue)        \
    PROC(LPALCGETSTRING,            alcGetString)           \
    PROC(LPALCGETINTEGERV,          alcGetIntegerv)         \
    PROC(LPALCCAPTUREOPENDEVICE,    alcCaptureOpenDevice)   \
    PROC(LPALCCAPTURECLOSEDEVICE,   alcCaptureCloseDevice)  \
    PROC(LPALCCAPTURESTART,         alcCaptureStart)        \
    PROC(LPALCCAPTURESTOP,          alcCaptureStop)         \
    PROC(LPALCCAPTURESAMPLES,       alcCaptureSamples)

//------------------------------------------------------------------------------

#define PROC(type, name) \
    static type dyn_##name;

PROC_LIST

#undef PROC

//------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4706)
#endif

#define PROC(type, name) \
    if (!(dyn_##name = pl_get_dll_proc(dll, #name))) { \
        LIBQU_LOGE("Failed to load function: %s.\n", #name); \
        pl_close_dll(dll); \
        return NULL; \
    }

static void *dyn_load_openal(void)
{
    char const *names[] = {
        "libopenal.so.1",
        "libopenal.so",
    };

    void *dll = NULL;

    for (size_t i = 0; i < sizeof(names) / sizeof(*names); i++) {
        dll = pl_open_dll(names[i]);

        if (dll) {
            LIBQU_LOGI("OpenAL is opened from %s.\n", names[i]);
            break;
        }
    }

    if (!dll) {
        LIBQU_LOGE("Failed to open OpenAL dynamic library.\n");
        return NULL;
    }

    PROC_LIST

    return dll;
}

#undef PROC

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//------------------------------------------------------------------------------

#define alEnable                    dyn_alEnable
#define alDisable                   dyn_alDisable
#define alIsEnabled                 dyn_alIsEnabled
#define alGetString                 dyn_alGetString
#define alGetBooleanv               dyn_alGetBooleanv
#define alGetIntegerv               dyn_alGetIntegerv
#define alGetFloatv                 dyn_alGetFloatv
#define alGetDoublev                dyn_alGetDoublev
#define alGetBoolean                dyn_alGetBoolean
#define alGetInteger                dyn_alGetInteger
#define alGetFloat                  dyn_alGetFloat
#define alGetDouble                 dyn_alGetDouble
#define alGetError                  dyn_alGetError
#define alIsExtensionPresent        dyn_alIsExtensionPresent
#define alGetProcAddress            dyn_alGetProcAddress
#define alGetEnumValue              dyn_alGetEnumValue
#define alListenerf                 dyn_alListenerf
#define alListener3f                dyn_alListener3f
#define alListenerfv                dyn_alListenerfv
#define alListeneri                 dyn_alListeneri
#define alListener3i                dyn_alListener3i
#define alListeneriv                dyn_alListeneriv
#define alGetListenerf              dyn_alGetListenerf
#define alGetListener3f             dyn_alGetListener3f
#define alGetListenerfv             dyn_alGetListenerfv
#define alGetListeneri              dyn_alGetListeneri
#define alGetListener3i             dyn_alGetListener3i
#define alGetListeneriv             dyn_alGetListeneriv
#define alGenSources                dyn_alGenSources
#define alDeleteSources             dyn_alDeleteSources
#define alIsSource                  dyn_alIsSource
#define alSourcef                   dyn_alSourcef
#define alSource3f                  dyn_alSource3f
#define alSourcefv                  dyn_alSourcefv
#define alSourcei                   dyn_alSourcei
#define alSource3i                  dyn_alSource3i
#define alSourceiv                  dyn_alSourceiv
#define alGetSourcef                dyn_alGetSourcef
#define alGetSource3f               dyn_alGetSource3f
#define alGetSourcefv               dyn_alGetSourcefv
#define alGetSourcei                dyn_alGetSourcei
#define alGetSource3i               dyn_alGetSource3i
#define alGetSourceiv               dyn_alGetSourceiv
#define alSourcePlayv               dyn_alSourcePlayv
#define alSourceStopv               dyn_alSourceStopv
#define alSourceRewindv             dyn_alSourceRewindv
#define alSourcePausev              dyn_alSourcePausev
#define alSourcePlay                dyn_alSourcePlay
#define alSourceStop                dyn_alSourceStop
#define alSourceRewind              dyn_alSourceRewind
#define alSourcePause               dyn_alSourcePause
#define alSourceQueueBuffers        dyn_alSourceQueueBuffers
#define alSourceUnqueueBuffers      dyn_alSourceUnqueueBuffers
#define alGenBuffers                dyn_alGenBuffers
#define alDeleteBuffers             dyn_alDeleteBuffers
#define alIsBuffer                  dyn_alIsBuffer
#define alBufferData                dyn_alBufferData
#define alBufferf                   dyn_alBufferf
#define alBuffer3f                  dyn_alBuffer3f
#define alBufferfv                  dyn_alBufferfv
#define alBufferi                   dyn_alBufferi
#define alBuffer3i                  dyn_alBuffer3i
#define alBufferiv                  dyn_alBufferiv
#define alGetBufferf                dyn_alGetBufferf
#define alGetBuffer3f               dyn_alGetBuffer3f
#define alGetBufferfv               dyn_alGetBufferfv
#define alGetBufferi                dyn_alGetBufferi
#define alGetBuffer3i               dyn_alGetBuffer3i
#define alGetBufferiv               dyn_alGetBufferiv
#define alDopplerFactor             dyn_alDopplerFactor
#define alDopplerVelocity           dyn_alDopplerVelocity
#define alSpeedOfSound              dyn_alSpeedOfSound
#define alDistanceModel             dyn_alDistanceModel
#define alcCreateContext            dyn_alcCreateContext
#define alcMakeContextCurrent       dyn_alcMakeContextCurrent
#define alcProcessContext           dyn_alcProcessContext
#define alcSuspendContext           dyn_alcSuspendContext
#define alcDestroyContext           dyn_alcDestroyContext
#define alcGetCurrentContext        dyn_alcGetCurrentContext
#define alcGetContextsDevice        dyn_alcGetContextsDevice
#define alcOpenDevice               dyn_alcOpenDevice
#define alcCloseDevice              dyn_alcCloseDevice
#define alcGetError                 dyn_alcGetError
#define alcIsExtensionPresent       dyn_alcIsExtensionPresent
#define alcGetProcAddress           dyn_alcGetProcAddress
#define alcGetEnumValue             dyn_alcGetEnumValue
#define alcGetString                dyn_alcGetString
#define alcGetIntegerv              dyn_alcGetIntegerv
#define alcCaptureOpenDevice        dyn_alcCaptureOpenDevice
#define alcCaptureCloseDevice       dyn_alcCaptureCloseDevice
#define alcCaptureStart             dyn_alcCaptureStart
#define alcCaptureStop              dyn_alcCaptureStop
#define alcCaptureSamples           dyn_alcCaptureSamples

//------------------------------------------------------------------------------

#endif // LIBQU_DYN_OPENAL_H_INC
