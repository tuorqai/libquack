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

#include <math.h>
#include <stdio.h>
#include <libquack.h>

//------------------------------------------------------------------------------

enum
{
    TEXTURE_SKY,
    TEXTURE_MOUNTAINS_BG,
    TEXTURE_MOUNTAINS_FG,
    TEXTURE_TREES_BG,
    TEXTURE_TREES_FG,
    TOTAL_TEXTURES,
};

//------------------------------------------------------------------------------

static float x_camera;
static float dx_camera;

static qu_texture textures[TOTAL_TEXTURES];

static bool smooth_key;
static bool smooth_flag;

static bool move_key;

//------------------------------------------------------------------------------

static void load_textures(void)
{
    char const *paths[TOTAL_TEXTURES] = {
        "assets/textures/sky.png",
        "assets/textures/mountains-bg.png",
        "assets/textures/mountains-fg.png",
        "assets/textures/trees-bg.png",
        "assets/textures/trees-fg.png",
    };

    qu_set_default_texture_flags(0);

    for (int i = 0; i < TOTAL_TEXTURES; i++) {
        textures[i] = qu_load_texture_from_file(paths[i]);
        printf("%s -> %d\n", paths[i], textures[i].id);
    }
}

static void update(double dt)
{
    x_camera += dx_camera * (float) dt;

    if (qu_is_key_pressed(QU_KEY_S) && !smooth_key) {
        smooth_flag = !smooth_flag;

        unsigned int f = (smooth_flag ? QU_TEXTURE_SMOOTH : 0);

        for (int i = 0; i < TOTAL_TEXTURES; i++) {
            qu_set_texture_flags(textures[i], f);
        }

        smooth_key = true;
    } else if (qu_is_key_released(QU_KEY_S)) {
        smooth_key = false;
    }

    if (qu_is_key_pressed(QU_KEY_SPACE) && !move_key) {
        if (dx_camera > 0.f) {
            dx_camera = 0.f;
        } else {
            dx_camera = 300.f;
        }

        move_key = true;
    } else if (qu_is_key_released(QU_KEY_SPACE)) {
        move_key = false;
    }
}

static void draw_layer(int texture, float w, float h, float f)
{
    float x = fmodf(x_camera / f, w);
    qu_draw_texture(textures[texture], -x, 0.f, w, h);
    qu_draw_texture(textures[texture], w - x, 0.f, w, h);
}

static void draw(void)
{
    qu_clear(0x202020FF);

    qu_draw_texture(textures[TEXTURE_SKY], -48.f, 0.f, 816.f, 480.f);

    draw_layer(TEXTURE_MOUNTAINS_BG, 816.f, 480.f, 4.f);
    draw_layer(TEXTURE_MOUNTAINS_FG, 1632.f, 480.f, 3.f);
    draw_layer(TEXTURE_TREES_BG, 1632.f, 480.f, 2.f);
    draw_layer(TEXTURE_TREES_FG, 1632.f, 480.f, 1.f);

    qu_present();
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] textures");
    qu_set_window_size(720, 480);

    qu_initialize();
    atexit(qu_terminate);

    x_camera = 0.f;
    dx_camera = 300.f;

    load_textures();

    double then = qu_get_time_highp();

    while (qu_process()) {
        double now = qu_get_time_highp();

        update(now - then);
        draw();

        then = now;
    }

    return 0;
}
