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

#ifndef LIBQU_AUDIO_H_INC
#define LIBQU_AUDIO_H_INC

//------------------------------------------------------------------------------

#include "fs.h"
#include "wave.h"

//------------------------------------------------------------------------------

struct libqu_sound
{
    struct libqu_wave *wave;
};

struct libqu_audio_params
{
    int unused;
};

struct libqu_audio_impl
{
    bool (*check_if_available)(void);
    bool (*initialize)(struct libqu_audio_params const *params);
    void (*terminate)(void);
    void (*set_master_volume)(float volume);
    int (*load_sound)(struct libqu_sound *sound);
    void (*delete_sound)(struct libqu_sound *sound);
    qu_handle (*play_sound)(struct libqu_sound *sound, int loop);
    int (*pause_voice)(qu_handle voice_id);
    int (*unpause_voice)(qu_handle voice_id);
    int (*stop_voice)(qu_handle voice_id);
};

//------------------------------------------------------------------------------

extern struct libqu_audio_impl const libqu_audio_null_impl;

//------------------------------------------------------------------------------

void libqu_audio_initialize(struct libqu_audio_params const *params);
void libqu_audio_terminate(void);

void libqu_audio_set_master_volume(float volume);
struct libqu_sound *libqu_audio_load_sound(struct libqu_wave *wave);
void libqu_audio_delete_sound(struct libqu_sound *sound);
qu_handle libqu_audio_play_sound(struct libqu_sound *sound, int loop);
void libqu_audio_pause_voice(qu_handle voice_id);
void libqu_audio_unpause_voice(qu_handle voice_id);
void libqu_audio_stop_voice(qu_handle voice_id);

//------------------------------------------------------------------------------

#endif // LIBQU_AUDIO_H_INC
