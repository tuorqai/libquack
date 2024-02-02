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

#ifndef LIBQU_AUDIO_OPENAL_H_INC
#define LIBQU_AUDIO_OPENAL_H_INC

//------------------------------------------------------------------------------

#include <al.h>
#include <alc.h>
#include "audio.h"

//------------------------------------------------------------------------------

struct libqu_openal_func
{
    LPALENABLE                          _alEnable;
    LPALDISABLE                         _alDisable;
    LPALISENABLED                       _alIsEnabled;
    LPALGETSTRING                       _alGetString;
    LPALGETBOOLEANV                     _alGetBooleanv;
    LPALGETINTEGERV                     _alGetIntegerv;
    LPALGETFLOATV                       _alGetFloatv;
    LPALGETDOUBLEV                      _alGetDoublev;
    LPALGETBOOLEAN                      _alGetBoolean;
    LPALGETINTEGER                      _alGetInteger;
    LPALGETFLOAT                        _alGetFloat;
    LPALGETDOUBLE                       _alGetDouble;
    LPALGETERROR                        _alGetError;
    LPALISEXTENSIONPRESENT              _alIsExtensionPresent;
    LPALGETPROCADDRESS                  _alGetProcAddress;
    LPALGETENUMVALUE                    _alGetEnumValue;
    LPALLISTENERF                       _alListenerf;
    LPALLISTENER3F                      _alListener3f;
    LPALLISTENERFV                      _alListenerfv;
    LPALLISTENERI                       _alListeneri;
    LPALLISTENER3I                      _alListener3i;
    LPALLISTENERIV                      _alListeneriv;
    LPALGETLISTENERF                    _alGetListenerf;
    LPALGETLISTENER3F                   _alGetListener3f;
    LPALGETLISTENERFV                   _alGetListenerfv;
    LPALGETLISTENERI                    _alGetListeneri;
    LPALGETLISTENER3I                   _alGetListener3i;
    LPALGETLISTENERIV                   _alGetListeneriv;
    LPALGENSOURCES                      _alGenSources;
    LPALDELETESOURCES                   _alDeleteSources;
    LPALISSOURCE                        _alIsSource;
    LPALSOURCEF                         _alSourcef;
    LPALSOURCE3F                        _alSource3f;
    LPALSOURCEFV                        _alSourcefv;
    LPALSOURCEI                         _alSourcei;
    LPALSOURCE3I                        _alSource3i;
    LPALSOURCEIV                        _alSourceiv;
    LPALGETSOURCEF                      _alGetSourcef;
    LPALGETSOURCE3F                     _alGetSource3f;
    LPALGETSOURCEFV                     _alGetSourcefv;
    LPALGETSOURCEI                      _alGetSourcei;
    LPALGETSOURCE3I                     _alGetSource3i;
    LPALGETSOURCEIV                     _alGetSourceiv;
    LPALSOURCEPLAYV                     _alSourcePlayv;
    LPALSOURCESTOPV                     _alSourceStopv;
    LPALSOURCEREWINDV                   _alSourceRewindv;
    LPALSOURCEPAUSEV                    _alSourcePausev;
    LPALSOURCEPLAY                      _alSourcePlay;
    LPALSOURCESTOP                      _alSourceStop;
    LPALSOURCEREWIND                    _alSourceRewind;
    LPALSOURCEPAUSE                     _alSourcePause;
    LPALSOURCEQUEUEBUFFERS              _alSourceQueueBuffers;
    LPALSOURCEUNQUEUEBUFFERS            _alSourceUnqueueBuffers;
    LPALGENBUFFERS                      _alGenBuffers;
    LPALDELETEBUFFERS                   _alDeleteBuffers;
    LPALISBUFFER                        _alIsBuffer;
    LPALBUFFERDATA                      _alBufferData;
    LPALBUFFERF                         _alBufferf;
    LPALBUFFER3F                        _alBuffer3f;
    LPALBUFFERFV                        _alBufferfv;
    LPALBUFFERI                         _alBufferi;
    LPALBUFFER3I                        _alBuffer3i;
    LPALBUFFERIV                        _alBufferiv;
    LPALGETBUFFERF                      _alGetBufferf;
    LPALGETBUFFER3F                     _alGetBuffer3f;
    LPALGETBUFFERFV                     _alGetBufferfv;
    LPALGETBUFFERI                      _alGetBufferi;
    LPALGETBUFFER3I                     _alGetBuffer3i;
    LPALGETBUFFERIV                     _alGetBufferiv;
    LPALDOPPLERFACTOR                   _alDopplerFactor;
    LPALDOPPLERVELOCITY                 _alDopplerVelocity;
    LPALSPEEDOFSOUND                    _alSpeedOfSound;
    LPALDISTANCEMODEL                   _alDistanceModel;

    LPALCCREATECONTEXT                  _alcCreateContext;
    LPALCMAKECONTEXTCURRENT             _alcMakeContextCurrent;
    LPALCPROCESSCONTEXT                 _alcProcessContext;
    LPALCSUSPENDCONTEXT                 _alcSuspendContext;
    LPALCDESTROYCONTEXT                 _alcDestroyContext;
    LPALCGETCURRENTCONTEXT              _alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE              _alcGetContextsDevice;
    LPALCOPENDEVICE                     _alcOpenDevice;
    LPALCCLOSEDEVICE                    _alcCloseDevice;
    LPALCGETERROR                       _alcGetError;
    LPALCISEXTENSIONPRESENT             _alcIsExtensionPresent;
    LPALCGETPROCADDRESS                 _alcGetProcAddress;
    LPALCGETENUMVALUE                   _alcGetEnumValue;
    LPALCGETSTRING                      _alcGetString;
    LPALCGETINTEGERV                    _alcGetIntegerv;
    LPALCCAPTUREOPENDEVICE              _alcCaptureOpenDevice;
    LPALCCAPTURECLOSEDEVICE             _alcCaptureCloseDevice;
    LPALCCAPTURESTART                   _alcCaptureStart;
    LPALCCAPTURESTOP                    _alcCaptureStop;
    LPALCCAPTURESAMPLES                 _alcCaptureSamples;
};

//------------------------------------------------------------------------------

#define alEnable                        priv.func._alEnable
#define alDisable                       priv.func._alDisable
#define alIsEnabled                     priv.func._alIsEnabled
#define alGetString                     priv.func._alGetString
#define alGetBooleanv                   priv.func._alGetBooleanv
#define alGetIntegerv                   priv.func._alGetIntegerv
#define alGetFloatv                     priv.func._alGetFloatv
#define alGetDoublev                    priv.func._alGetDoublev
#define alGetBoolean                    priv.func._alGetBoolean
#define alGetInteger                    priv.func._alGetInteger
#define alGetFloat                      priv.func._alGetFloat
#define alGetDouble                     priv.func._alGetDouble
#define alGetError                      priv.func._alGetError
#define alIsExtensionPresent            priv.func._alIsExtensionPresent
#define alGetProcAddress                priv.func._alGetProcAddress
#define alGetEnumValue                  priv.func._alGetEnumValue
#define alListenerf                     priv.func._alListenerf
#define alListener3f                    priv.func._alListener3f
#define alListenerfv                    priv.func._alListenerfv
#define alListeneri                     priv.func._alListeneri
#define alListener3i                    priv.func._alListener3i
#define alListeneriv                    priv.func._alListeneriv
#define alGetListenerf                  priv.func._alGetListenerf
#define alGetListener3f                 priv.func._alGetListener3f
#define alGetListenerfv                 priv.func._alGetListenerfv
#define alGetListeneri                  priv.func._alGetListeneri
#define alGetListener3i                 priv.func._alGetListener3i
#define alGetListeneriv                 priv.func._alGetListeneriv
#define alGenSources                    priv.func._alGenSources
#define alDeleteSources                 priv.func._alDeleteSources
#define alIsSource                      priv.func._alIsSource
#define alSourcef                       priv.func._alSourcef
#define alSource3f                      priv.func._alSource3f
#define alSourcefv                      priv.func._alSourcefv
#define alSourcei                       priv.func._alSourcei
#define alSource3i                      priv.func._alSource3i
#define alSourceiv                      priv.func._alSourceiv
#define alGetSourcef                    priv.func._alGetSourcef
#define alGetSource3f                   priv.func._alGetSource3f
#define alGetSourcefv                   priv.func._alGetSourcefv
#define alGetSourcei                    priv.func._alGetSourcei
#define alGetSource3i                   priv.func._alGetSource3i
#define alGetSourceiv                   priv.func._alGetSourceiv
#define alSourcePlayv                   priv.func._alSourcePlayv
#define alSourceStopv                   priv.func._alSourceStopv
#define alSourceRewindv                 priv.func._alSourceRewindv
#define alSourcePausev                  priv.func._alSourcePausev
#define alSourcePlay                    priv.func._alSourcePlay
#define alSourceStop                    priv.func._alSourceStop
#define alSourceRewind                  priv.func._alSourceRewind
#define alSourcePause                   priv.func._alSourcePause
#define alSourceQueueBuffers            priv.func._alSourceQueueBuffers
#define alSourceUnqueueBuffers          priv.func._alSourceUnqueueBuffers
#define alGenBuffers                    priv.func._alGenBuffers
#define alDeleteBuffers                 priv.func._alDeleteBuffers
#define alIsBuffer                      priv.func._alIsBuffer
#define alBufferData                    priv.func._alBufferData
#define alBufferf                       priv.func._alBufferf
#define alBuffer3f                      priv.func._alBuffer3f
#define alBufferfv                      priv.func._alBufferfv
#define alBufferi                       priv.func._alBufferi
#define alBuffer3i                      priv.func._alBuffer3i
#define alBufferiv                      priv.func._alBufferiv
#define alGetBufferf                    priv.func._alGetBufferf
#define alGetBuffer3f                   priv.func._alGetBuffer3f
#define alGetBufferfv                   priv.func._alGetBufferfv
#define alGetBufferi                    priv.func._alGetBufferi
#define alGetBuffer3i                   priv.func._alGetBuffer3i
#define alGetBufferiv                   priv.func._alGetBufferiv
#define alDopplerFactor                 priv.func._alDopplerFactor
#define alDopplerVelocity               priv.func._alDopplerVelocity
#define alSpeedOfSound                  priv.func._alSpeedOfSound
#define alDistanceModel                 priv.func._alDistanceModel

#define alcCreateContext                priv.func._alcCreateContext
#define alcMakeContextCurrent           priv.func._alcMakeContextCurrent
#define alcProcessContext               priv.func._alcProcessContext
#define alcSuspendContext               priv.func._alcSuspendContext
#define alcDestroyContext               priv.func._alcDestroyContext
#define alcGetCurrentContext            priv.func._alcGetCurrentContext
#define alcGetContextsDevice            priv.func._alcGetContextsDevice
#define alcOpenDevice                   priv.func._alcOpenDevice
#define alcCloseDevice                  priv.func._alcCloseDevice
#define alcGetError                     priv.func._alcGetError
#define alcIsExtensionPresent           priv.func._alcIsExtensionPresent
#define alcGetProcAddress               priv.func._alcGetProcAddress
#define alcGetEnumValue                 priv.func._alcGetEnumValue
#define alcGetString                    priv.func._alcGetString
#define alcGetIntegerv                  priv.func._alcGetIntegerv
#define alcCaptureOpenDevice            priv.func._alcCaptureOpenDevice
#define alcCaptureCloseDevice           priv.func._alcCaptureCloseDevice
#define alcCaptureStart                 priv.func._alcCaptureStart
#define alcCaptureStop                  priv.func._alcCaptureStop
#define alcCaptureSamples               priv.func._alcCaptureSamples

//------------------------------------------------------------------------------

#endif // LIBQU_AUDIO_OPENAL_H_INC
