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

#include <string.h>
#include "audio.h"
#include "core.h"
#include "graphics.h"
#include "handle.h"
#include "log.h"
#include "wave.h"

//------------------------------------------------------------------------------

enum
{
    EXTRA_MODULE_AUDIO = (1 << 0),
};

//------------------------------------------------------------------------------

static struct
{
    int refcount;
    unsigned int extra;

    struct {
        struct libqu_core_params core;
        struct libqu_graphics_params graphics;
        struct libqu_audio_params audio;
    } params;
} priv;

//------------------------------------------------------------------------------

static void sanitize_core_params(void)
{
    struct libqu_core_params *p = &priv.params.core;

    if (p->window_title[0] == '\0') {
        strncpy(p->window_title, "libquack application", LIBQU_WINDOW_TITLE_LENGTH - 1);
    }

    if (p->window_size.x == 0 || p->window_size.y == 0) {
        p->window_size.x = 1280;
        p->window_size.y = 720;
    }
}

static void sanitize_graphics_params(void)
{
    struct libqu_graphics_params *p = &priv.params.graphics;

    if (p->window_size.x == 0 || p->window_size.y == 0) {
        p->window_size.x = priv.params.core.window_size.x;
        p->window_size.y = priv.params.core.window_size.y;
    }
}

static void initialize_audio(void)
{
    libqu_audio_initialize(&priv.params.audio);

    priv.extra |= EXTRA_MODULE_AUDIO;
}

//------------------------------------------------------------------------------

void qu_initialize(void)
{
    if (++priv.refcount != 1) {
        LIBQU_LOGW("Already initialized.\n");
        return;
    }

    sanitize_core_params();
    sanitize_graphics_params();

    libqu_core_initialize(&priv.params.core);
    libqu_graphics_initialize(&priv.params.graphics);

    LIBQU_LOGI("Initialized.\n");
}

void qu_terminate(void)
{
    if (--priv.refcount != 0) {
        return;
    }

    libqu_graphics_terminate();
    libqu_core_terminate();

    LIBQU_LOGI("Terminated.\n");
}

bool qu_process(void)
{
    return libqu_core_process();
}

void qu_present(void)
{
    libqu_graphics_flush();
    libqu_core_swap();
}

char const *qu_get_window_title(void)
{
    if (priv.refcount > 0) {
        return libqu_core_get_window_title();
    } else {
        if (priv.params.core.window_title[0] == '\0') {
            return priv.params.core.window_title;
        } else {
            return NULL;
        }
    }
}

qu_vec2i qu_get_window_size(void)
{
    if (priv.refcount > 0) {
        return libqu_core_get_window_size();
    } else {
        return priv.params.core.window_size;
    }
}

void qu_set_window_title(char const *title)
{
    strncpy(priv.params.core.window_title, title, LIBQU_WINDOW_TITLE_LENGTH - 1);

    if (priv.refcount > 0) {
        libqu_core_set_window_title(priv.params.core.window_title);
    }
}

void qu_set_window_size(int w, int h)
{
    priv.params.core.window_size.x = w;
    priv.params.core.window_size.y = h;

    if (priv.refcount > 0) {
        libqu_core_set_window_size(priv.params.core.window_size);
    }
}

bool qu_is_window_active(void)
{
    return libqu_core_is_window_active();
}

qu_key_state const *qu_get_keyboard_state(void)
{
    return libqu_core_get_keyboard_state();
}

qu_key_state qu_get_key_state(qu_key key)
{
    return libqu_core_get_keyboard_state()[key];
}

bool qu_is_key_idle(qu_key key)
{
    return libqu_core_get_keyboard_state()[key] == QU_KEY_STATE_IDLE;
}

bool qu_is_key_pressed(qu_key key)
{
    return libqu_core_get_keyboard_state()[key] == QU_KEY_STATE_PRESSED;
}

bool qu_is_key_released(qu_key key)
{
    return libqu_core_get_keyboard_state()[key] == QU_KEY_STATE_RELEASED;
}

void qu_clear(qu_color color)
{
    libqu_graphics_clear(color);
}

void qu_draw_point(float x, float y, qu_color color)
{
    qu_vec2f xy = { x, y };

    libqu_graphics_draw_point(xy, color);
}

void qu_draw_line(float ax, float ay, float bx, float by, qu_color color)
{
    qu_vec2f a = { ax, ay };
    qu_vec2f b = { bx, by };

    libqu_graphics_draw_line(a, b, color);
}

void qu_draw_triangle(float ax, float ay, float bx, float by, float cx, float cy, qu_color outline, qu_color fill)
{
    qu_vec2f a = { ax, ay };
    qu_vec2f b = { bx, by };
    qu_vec2f c = { cx, cy };

    libqu_graphics_draw_triangle(a, b, c, outline, fill);
}

void qu_draw_rectangle(float x, float y, float w, float h, qu_color outline, qu_color fill)
{
    qu_vec2f xy = { x, y };
    qu_vec2f wh = { w, h };

    libqu_graphics_draw_rectangle(xy, wh, outline, fill);
}

qu_wave qu_create_wave(int16_t channels, int64_t samples, int64_t sample_rate)
{
    qu_wave wave = { 0 };
    struct libqu_wave *wave_p = libqu_wave_create(channels, samples, sample_rate);

    if (wave_p) {
        wave.id = libqu_handle_create(LIBQU_HANDLE_WAVE, wave_p);
    }

    return wave;
}

qu_wave qu_load_wave(char const *path)
{
    qu_wave wave = { 0 };
    struct libqu_file *file = libqu_fopen(path);

    if (file) {
        struct libqu_wave *wave_p = libqu_wave_load(file);

        if (wave_p) {
            wave.id = libqu_handle_create(LIBQU_HANDLE_WAVE, wave_p);
        }

        libqu_fclose(file);
    }

    return wave;
}

void qu_destroy_wave(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        libqu_wave_destroy(wave_p);
        libqu_handle_destroy(LIBQU_HANDLE_WAVE, wave.id);
    }
}

int16_t qu_get_wave_channel_count(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->channel_count;
    }

    return 0;
}

int64_t qu_get_wave_sample_count(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->sample_count;
    }

    return 0;
}

int64_t qu_get_wave_sample_rate(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->sample_rate;
    }

    return 0;
}

int16_t *qu_get_wave_samples(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->samples;
    }

    return 0;
}

void qu_set_master_volume(float volume)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        initialize_audio();
    }

    libqu_audio_set_master_volume(volume);
}

qu_sound qu_load_sound_from_file(char const *path)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        initialize_audio();
    }

    qu_sound sound = { 0 };
    struct libqu_file *file = libqu_fopen(path);

    if (file) {
        struct libqu_wave *wave_p = libqu_wave_load(file);

        if (wave_p) {
            struct libqu_sound *sound_p = libqu_audio_load_sound(wave_p);

            if (sound_p) {
                sound.id = libqu_handle_create(LIBQU_HANDLE_SOUND, sound_p);
            }
        }

        libqu_fclose(file);
    }

    return sound;
}

qu_sound qu_load_sound_from_wave(qu_wave wave)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        initialize_audio();
    }

    qu_sound sound = { 0 };
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        struct libqu_sound *sound_p = libqu_audio_load_sound(wave_p);

        if (sound_p) {
            sound.id = libqu_handle_create(LIBQU_HANDLE_SOUND, sound_p);
        }
    }

    return sound;
}

void qu_delete_sound(qu_sound sound)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return;
    }

    struct libqu_sound *sound_p = libqu_handle_get(LIBQU_HANDLE_SOUND, sound.id);

    if (sound_p) {
        libqu_audio_delete_sound(sound_p);
    }
}

qu_voice qu_play_sound(qu_sound sound)
{
    qu_voice voice = { 0 };

    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return voice;
    }

    struct libqu_sound *sound_p = libqu_handle_get(LIBQU_HANDLE_SOUND, sound.id);

    if (sound_p) {
        voice.id = libqu_audio_play_sound(sound_p, 0);
    }

    return voice;
}

qu_voice qu_loop_sound(qu_sound sound)
{
    qu_voice voice = { 0 };

    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return voice;
    }

    struct libqu_sound *sound_p = libqu_handle_get(LIBQU_HANDLE_SOUND, sound.id);

    if (sound_p) {
        voice.id = libqu_audio_play_sound(sound_p, 0);
    }

    return voice;
}

void qu_pause_voice(qu_voice voice)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return;
    }

    libqu_audio_pause_voice(voice.id);
}

void qu_unpause_voice(qu_voice voice)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return;
    }

    libqu_audio_unpause_voice(voice.id);
}

void qu_stop_voice(qu_voice voice)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        return;
    }

    libqu_audio_stop_voice(voice.id);
}
