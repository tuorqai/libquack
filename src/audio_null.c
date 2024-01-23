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

static int audio_null_load_sound(struct libqu_sound *sound)
{
    return 0;
}

static void audio_null_delete_sound(struct libqu_sound *sound)
{
}

static qu_handle audio_null_play_sound(struct libqu_sound *sound, int loop)
{
    return 0;
}

static int audio_null_pause_voice(qu_handle voice_id)
{
    return 0;
}

static int audio_null_unpause_voice(qu_handle voice_id)
{
    return 0;
}

static int audio_null_stop_voice(qu_handle voice_id)
{
    return 0;
}

//------------------------------------------------------------------------------

struct libqu_audio_impl const libqu_audio_null_impl = {
    audio_null_check_if_available,
    audio_null_initialize,
    audio_null_terminate,
    audio_null_set_master_volume,
    audio_null_load_sound,
    audio_null_delete_sound,
    audio_null_play_sound,
    audio_null_pause_voice,
    audio_null_unpause_voice,
    audio_null_stop_voice,
};
