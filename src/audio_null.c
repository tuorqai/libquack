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

#include "audio.h"
#include "log.h"

//------------------------------------------------------------------------------

static bool audio_null_check_if_available(void)
{
    return true;
}

static bool audio_null_initialize(struct libqu_audio_params const *params)
{
    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void audio_null_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

static void audio_null_set_master_volume(float volume)
{
}

static int audio_null_create_sound(struct libqu_sound *sound)
{
    return 0;
}

static void audio_null_destroy_sound(struct libqu_sound *sound)
{
}

static qu_playback_state audio_null_get_sound_state(struct libqu_sound *sound)
{
    return QU_PLAYBACK_STOPPED;
}

static void audio_null_set_sound_loop(struct libqu_sound *sound, int loop)
{
}

static void audio_null_set_sound_state(struct libqu_sound *sound, qu_playback_state state)
{
}

static int audio_null_create_music(struct libqu_music *music)
{
    return 0;
}

static void audio_null_destroy_music(struct libqu_music *music)
{
}

static int audio_null_enqueue_music_buffer(struct libqu_music *music, int16_t const *buffer, size_t samples)
{
    return 0;
}

static int audio_null_dequeue_played_music_buffers(struct libqu_music *music)
{
    return 0;
}

static void audio_null_dequeue_all_music_buffers(struct libqu_music *music)
{
}

static qu_playback_state audio_null_get_music_state(struct libqu_music *music)
{
    return QU_PLAYBACK_STOPPED;
}

static void audio_null_set_music_state(struct libqu_music *music, qu_playback_state state)
{
}

//------------------------------------------------------------------------------

struct libqu_audio_impl const libqu_audio_null_impl = {
    audio_null_check_if_available,
    audio_null_initialize,
    audio_null_terminate,
    audio_null_set_master_volume,
    audio_null_create_sound,
    audio_null_destroy_sound,
    audio_null_get_sound_state,
    audio_null_set_sound_loop,
    audio_null_set_sound_state,
    audio_null_create_music,
    audio_null_destroy_music,
    audio_null_enqueue_music_buffer,
    audio_null_dequeue_played_music_buffers,
    audio_null_dequeue_all_music_buffers,
    audio_null_get_music_state,
    audio_null_set_music_state,
};
