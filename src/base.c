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
#include "platform.h"

//------------------------------------------------------------------------------

#define EXPAND_VERSION_STRING(major, minor, patch) \
    #major "." #minor "." #patch

#define VERSION_STRING(major, minor, patch) \
    EXPAND_VERSION_STRING(major, minor, patch)

#define QU_SOUND_NONE \
    (qu_sound) { .id = 0 }

#define QU_WAVE_NONE \
    (qu_wave) { .id = 0 }

#define QU_SOUND(handle) \
    (qu_sound) { .id = handle }

#define QU_WAVE(handle) \
    (qu_wave) { .id = handle }

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

    uint32_t start_ticks_mediump;
    uint64_t start_ticks_highp;

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

static void initialize_extra(unsigned int extra)
{
    if ((priv.extra & extra) == extra) {
        return;
    }

    switch (extra) {
    case EXTRA_MODULE_AUDIO:
        libqu_audio_initialize(&priv.params.audio);
        break;
    default:
        break;
    }

    priv.extra |= extra;
}

//------------------------------------------------------------------------------

char const *qu_get_version(void)
{
    return VERSION_STRING(QU_VERSION_MAJOR, QU_VERSION_MINOR, QU_VERSION_PATCH);
}

void qu_initialize(void)
{
    if (++priv.refcount != 1) {
        LIBQU_LOGW("Already initialized.\n");
        return;
    }

    LIBQU_LOGI("libquack version %s\n", qu_get_version());

    priv.start_ticks_mediump = pl_get_ticks_mediump();
    priv.start_ticks_highp = pl_get_ticks_highp();

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

    libqu_release_handles();

    if (priv.extra & EXTRA_MODULE_AUDIO) {
        libqu_audio_terminate();
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

//------------------------------------------------------------------------------

uint32_t qu_get_ticks_msec(void)
{
    return pl_get_ticks_mediump() - priv.start_ticks_mediump;
}

uint64_t qu_get_ticks_nsec(void)
{
    return pl_get_ticks_highp() - priv.start_ticks_highp;
}

float qu_get_time_mediump(void)
{
    return qu_get_ticks_msec() / 1000.f;
}

double qu_get_time_highp(void)
{
    return qu_get_ticks_nsec() / 1000000000.0;
}

qu_date_time qu_get_date_time(void)
{
    pl_date_time pdt;
    qu_date_time qdt;

    pl_get_date_time(&pdt);

    qdt.year            = pdt.year;
    qdt.month           = pdt.month;
    qdt.day             = pdt.day;
    qdt.weekday         = pdt.weekday;
    qdt.hours           = pdt.hours;
    qdt.minutes         = pdt.minutes;
    qdt.seconds         = pdt.seconds;
    qdt.milliseconds    = pdt.milliseconds;

    return qdt;
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

qu_image qu_create_image(int width, int height, qu_pixel_format format)
{
    qu_image handle = { 0 };
    qu_vec2i size = { width, height };
    struct libqu_image *image = libqu_image_create(format, size);

    if (image) {
        handle.id = libqu_handle_create(LIBQU_HANDLE_IMAGE, image);
    }

    return handle;
}

qu_image qu_load_image_from_file(char const *path)
{
    qu_image handle = { 0 };
    struct libqu_file *file = libqu_fopen(path);

    if (file) {
        struct libqu_image *image = libqu_image_load(file);

        if (image) {
            handle.id = libqu_handle_create(LIBQU_HANDLE_IMAGE, image);
        }

        libqu_fclose(file);
    }

    return handle;
}

qu_image qu_load_image_from_buffer(void *buffer, size_t size)
{
    qu_image handle = { 0 };
    struct libqu_file *file = libqu_fopen_buffer(buffer, size);

    if (file) {
        struct libqu_image *image = libqu_image_load(file);

        if (image) {
            handle.id = libqu_handle_create(LIBQU_HANDLE_IMAGE, image);
        }

        libqu_fclose(file);
    }

    return handle;
}

void qu_destroy_image(qu_image handle)
{
    libqu_handle_destroy(LIBQU_HANDLE_IMAGE, handle.id);
}

qu_vec2i qu_get_image_size(qu_image handle)
{
    qu_vec2i size = { -1, -1 };
    struct libqu_image *image = libqu_handle_get(LIBQU_HANDLE_IMAGE, handle.id);

    if (image) {
        size.x = image->size.x;
        size.y = image->size.y;
    }

    return size;
}

qu_pixel_format qu_get_image_format(qu_image handle)
{
    struct libqu_image *image = libqu_handle_get(LIBQU_HANDLE_IMAGE, handle.id);

    if (image) {
        return image->format;
    }

    return QU_PIXFMT_INVALID;
}

unsigned char *qu_get_image_pixels(qu_image handle)
{
    struct libqu_image *image = libqu_handle_get(LIBQU_HANDLE_IMAGE, handle.id);

    if (image) {
        return image->pixels;
    }

    return NULL;
}

void qu_set_default_texture_flags(unsigned int flags)
{
    libqu_graphics_set_default_texture_flags(flags);
}

qu_texture qu_load_texture_from_file(char const *path)
{
    qu_texture texture_h = { 0 };

    struct libqu_file *file = libqu_fopen(path);

    if (file) {
        struct libqu_image *image = libqu_image_load(file);

        if (image) {
            struct libqu_texture *texture = libqu_graphics_load_texture(image);

            if (texture) {
                texture_h.id =
                    libqu_handle_create(LIBQU_HANDLE_TEXTURE, texture);
            }
        }

        libqu_fclose(file);
    }

    return texture_h;
}

qu_texture qu_load_texture_from_buffer(void *buffer, size_t size)
{
    qu_texture texture_h = { 0 };

    struct libqu_file *file = libqu_fopen_buffer(buffer, size);

    if (file) {
        struct libqu_image *image = libqu_image_load(file);

        if (image) {
            struct libqu_texture *texture = libqu_graphics_load_texture(image);

            if (texture) {
                texture_h.id =
                    libqu_handle_create(LIBQU_HANDLE_TEXTURE, texture);
            }
        }

        libqu_fclose(file);
    }

    return texture_h;
}

qu_texture qu_load_texture_from_image(qu_image image_h)
{
    qu_texture texture_h = { 0 };

    struct libqu_image *image =
        libqu_handle_get(LIBQU_HANDLE_IMAGE, image_h.id);
    
    if (image) {
        struct libqu_texture *texture = libqu_graphics_load_texture(image);

        if (texture) {
            texture_h.id = libqu_handle_create(LIBQU_HANDLE_TEXTURE, texture);
        }
    }

    return texture_h;
}

void qu_destroy_texture(qu_texture texture_h)
{
    libqu_handle_destroy(LIBQU_HANDLE_TEXTURE, texture_h.id);
}

qu_vec2i qu_get_texture_size(qu_texture texture_h)
{
    qu_vec2i size = { -1, -1 };

    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);

    if (texture) {
        size = texture->image->size;
    }

    return size;
}

qu_pixel_format qu_get_texture_format(qu_texture texture_h)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        return texture->image->format;
    }

    return QU_PIXFMT_INVALID;
}

unsigned int qu_get_texture_flags(qu_texture texture_h)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        return texture->flags;
    }

    return 0;
}

void qu_set_texture_flags(qu_texture texture_h, unsigned int flags)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        libqu_graphics_set_texture_flags(texture, flags);
    }
}

void qu_draw_texture(qu_texture texture_h, float x, float y, float w, float h)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        qu_rectf rect = { x, y, w, h };
        libqu_graphics_draw_texture(texture, rect);
    }
}

void qu_draw_texture_r(qu_texture texture_h, qu_rectf rect)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        libqu_graphics_draw_texture(texture, rect);
    }
}

void qu_draw_subtexture(qu_texture texture_h,
    float x, float y, float w, float h,
    float s, float t, float u, float v)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        qu_rectf rect = { x, y, w, h };
        qu_rectf sub = { s, t, u, v };
        libqu_graphics_draw_subtexture(texture, rect, sub);
    }
}

void qu_draw_subtexture_r(qu_texture texture_h, qu_rectf rect, qu_rectf sub)
{
    struct libqu_texture *texture =
        libqu_handle_get(LIBQU_HANDLE_TEXTURE, texture_h.id);
    
    if (texture) {
        libqu_graphics_draw_subtexture(texture, rect, sub);
    }
}

qu_image qu_capture_screen(void)
{
    qu_image image_h = { 0 };
    struct libqu_image *image = libqu_graphics_capture_screen();

    if (image) {
        image_h.id = libqu_handle_create(LIBQU_HANDLE_IMAGE, image);
    }

    return image_h;
}

void qu_set_blend_mode(qu_blend_mode mode)
{
    libqu_graphics_set_blend_mode(mode);
}

//------------------------------------------------------------------------------

qu_wave qu_create_wave(int16_t channels, int64_t samples, int64_t sample_rate)
{
    struct libqu_wave *wave = libqu_wave_create(channels, samples, sample_rate);

    if (!wave) {
        return (qu_wave) { 0 };
    }

    return (qu_wave) { libqu_handle_create(LIBQU_HANDLE_WAVE, wave) };
}

qu_wave qu_load_wave(char const *path)
{
    struct libqu_file *file = libqu_fopen(path);

    if (!file) {
        return (qu_wave) { 0 };
    }

    struct libqu_wave *wave = libqu_wave_load(file);

    libqu_fclose(file);

    if (!wave) {
        return (qu_wave) { 0 };
    }

    return (qu_wave) { libqu_handle_create(LIBQU_HANDLE_WAVE, wave) };
}

void qu_destroy_wave(qu_wave wave)
{
    libqu_handle_destroy(LIBQU_HANDLE_WAVE, wave.id);
}

int16_t qu_get_wave_channel_count(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->format.channels;
    }

    return 0;
}

int64_t qu_get_wave_sample_count(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->size / wave_p->format.channels;
    }

    return 0;
}

int64_t qu_get_wave_sample_rate(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->format.rate;
    }

    return 0;
}

int16_t *qu_get_wave_samples(qu_wave wave)
{
    struct libqu_wave *wave_p = libqu_handle_get(LIBQU_HANDLE_WAVE, wave.id);

    if (wave_p) {
        return wave_p->buffer;
    }

    return 0;
}

void qu_set_master_volume(float volume)
{
    initialize_extra(EXTRA_MODULE_AUDIO);

    if (priv.extra & EXTRA_MODULE_AUDIO) {
        libqu_audio_set_master_volume(volume);
    }
}

qu_sound qu_load_sound_from_file(char const *path)
{
    initialize_extra(EXTRA_MODULE_AUDIO);

    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_file *file = libqu_fopen(path);

        if (!file) {
            return QU_SOUND_NONE;
        }

        struct libqu_wave *wave = libqu_wave_load(file);

        libqu_fclose(file);

        if (!wave) {
            return QU_SOUND_NONE;
        }

        struct libqu_sound *sound = libqu_audio_load_sound(wave);

        if (!sound) {
            libqu_wave_destroy(wave);
            return QU_SOUND_NONE;
        }

        return QU_SOUND(libqu_handle_create(LIBQU_HANDLE_SOUND, sound));
    }

    return QU_SOUND_NONE;
}

qu_sound qu_load_sound_from_wave(qu_wave handle)
{
    initialize_extra(EXTRA_MODULE_AUDIO);

    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_wave *wave = libqu_handle_get(LIBQU_HANDLE_WAVE, handle.id);

        if (!wave) {
            return QU_SOUND_NONE;
        }

        wave->refcount++;

        struct libqu_sound *sound = libqu_audio_load_sound(wave);

        if (!sound) {
            wave->refcount--;
            return QU_SOUND_NONE;
        }

        return QU_SOUND(libqu_handle_create(LIBQU_HANDLE_SOUND, sound));
    }

    return QU_SOUND_NONE;
}

void qu_destroy_sound(qu_sound handle)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        libqu_handle_destroy(LIBQU_HANDLE_SOUND, handle.id);
    }
}

qu_playback_state qu_get_sound_state(qu_sound handle)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_sound *sound = libqu_handle_get(LIBQU_HANDLE_SOUND, handle.id);

        if (!sound) {
            return QU_PLAYBACK_INVALID;
        }

        return libqu_audio_get_sound_state(sound);
    }

    return QU_PLAYBACK_INVALID;
}

void qu_set_sound_loop(qu_sound handle, int loop)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_sound *sound = libqu_handle_get(LIBQU_HANDLE_SOUND, handle.id);

        if (!sound) {
            return;
        }

        libqu_audio_set_sound_loop(sound, loop);
    }
}

void qu_play_sound(qu_sound handle)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_sound *sound = libqu_handle_get(LIBQU_HANDLE_SOUND, handle.id);

        if (!sound) {
            return;
        }

        libqu_audio_set_sound_state(sound, QU_PLAYBACK_PLAYING);
    }
}

void qu_pause_sound(qu_sound handle)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_sound *sound = libqu_handle_get(LIBQU_HANDLE_SOUND, handle.id);

        if (!sound) {
            return;
        }

        libqu_audio_set_sound_state(sound, QU_PLAYBACK_PAUSED);
    }
}

void qu_stop_sound(qu_sound handle)
{
    if (priv.extra & EXTRA_MODULE_AUDIO) {
        struct libqu_sound *sound = libqu_handle_get(LIBQU_HANDLE_SOUND, handle.id);

        if (!sound) {
            return;
        }

        libqu_audio_set_sound_state(sound, QU_PLAYBACK_STOPPED);
    }
}

qu_music qu_open_music_from_file(char const *path)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        initialize_extra(EXTRA_MODULE_AUDIO);
    }

    struct libqu_file *file = libqu_fopen(path);

    if (!file) {
        return (qu_music) { .id = -1 };
    }

    struct libqu_music *music = libqu_audio_open_music(file);

    if (!music) {
        libqu_fclose(file);
        return (qu_music) { .id = -1 };
    }

    return (qu_music) { .id = libqu_handle_create(LIBQU_HANDLE_MUSIC, music) };
}

qu_music qu_open_music_from_buffer(void *buffer, size_t size)
{
    if (!(priv.extra & EXTRA_MODULE_AUDIO)) {
        initialize_extra(EXTRA_MODULE_AUDIO);
    }

    struct libqu_file *file = libqu_fopen_buffer(buffer, size);

    if (!file) {
        return (qu_music) { .id = -1 };
    }

    struct libqu_music *music = libqu_audio_open_music(file);

    if (!music) {
        libqu_fclose(file);
        return (qu_music) { .id = -1 };
    }

    return (qu_music) { .id = libqu_handle_create(LIBQU_HANDLE_MUSIC, music) };
}

void qu_close_music(qu_music handle)
{
    libqu_handle_destroy(LIBQU_HANDLE_MUSIC, handle.id);
}

qu_playback_state qu_get_music_state(qu_music handle)
{
    struct libqu_music *music = libqu_handle_get(LIBQU_HANDLE_MUSIC, handle.id);

    if (!music) {
        return QU_PLAYBACK_INVALID;
    }

    return libqu_audio_get_music_state(music);
}

void qu_set_music_loop(qu_music handle, int loop)
{
    struct libqu_music *music = libqu_handle_get(LIBQU_HANDLE_MUSIC, handle.id);

    if (!music) {
        return;
    }

    libqu_audio_set_music_loop(music, loop);
}

void qu_play_music(qu_music handle)
{
    struct libqu_music *music = libqu_handle_get(LIBQU_HANDLE_MUSIC, handle.id);

    if (!music) {
        return;
    }

    libqu_audio_set_music_state(music, QU_PLAYBACK_PLAYING);
}

void qu_pause_music(qu_music handle)
{
    struct libqu_music *music = libqu_handle_get(LIBQU_HANDLE_MUSIC, handle.id);

    if (!music) {
        return;
    }

    libqu_audio_set_music_state(music, QU_PLAYBACK_PAUSED);
}

void qu_stop_music(qu_music handle)
{
    struct libqu_music *music = libqu_handle_get(LIBQU_HANDLE_MUSIC, handle.id);

    if (!music) {
        return;
    }

    libqu_audio_set_music_state(music, QU_PLAYBACK_STOPPED);
}
