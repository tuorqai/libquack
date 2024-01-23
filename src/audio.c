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
#include "platform.h"

//------------------------------------------------------------------------------

static struct libqu_audio_impl const *impl_list[] = {
    &libqu_audio_null_impl,
};

//------------------------------------------------------------------------------

static struct
{
    struct libqu_audio_impl const *impl;
} priv;

//------------------------------------------------------------------------------

static struct libqu_audio_impl const *choose_impl(void)
{
    int count = sizeof(impl_list) / sizeof(impl_list[0]);

    for (int i = 0; i < count; i++) {
        if (impl_list[i]->check_if_available()) {
            return impl_list[i];
        }
    }

    abort();
}

//------------------------------------------------------------------------------

void libqu_audio_initialize(struct libqu_audio_params const *params)
{
    priv.impl = choose_impl();

    if (!priv.impl->initialize(params)) {
        LIBQU_LOGE("Failed to initialize libqu::audio implementation.\n");
        abort();
    }

    LIBQU_LOGI("Initialized.\n");
}

void libqu_audio_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

void libqu_audio_set_master_volume(float volume)
{
    priv.impl->set_master_volume(volume);
}

struct libqu_sound *libqu_audio_load_sound(struct libqu_wave *wave)
{
    struct libqu_sound *sound = pl_calloc(1, sizeof(*sound));

    sound->wave = wave;

    if (priv.impl->load_sound(sound) == 0) {
        return sound;
    }

    libqu_wave_destroy(sound->wave);
    pl_free(sound);

    return NULL;
}

void libqu_audio_delete_sound(struct libqu_sound *sound)
{
    priv.impl->delete_sound(sound);

    libqu_wave_destroy(sound->wave);
    pl_free(sound);
}

qu_handle libqu_audio_play_sound(struct libqu_sound *sound, int loop)
{
    return priv.impl->play_sound(sound, loop);
}

void libqu_audio_pause_voice(qu_handle voice_id)
{
    priv.impl->pause_voice(voice_id);
}

void libqu_audio_unpause_voice(qu_handle voice_id)
{
    priv.impl->unpause_voice(voice_id);
}

void libqu_audio_stop_voice(qu_handle voice_id)
{
    priv.impl->stop_voice(voice_id);
}
