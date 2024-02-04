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
#include <libquack.h>

//------------------------------------------------------------------------------

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------------

static bool point_init;
static qu_vec2f point_pos[256];
static qu_color point_col[256];

static void test_points(void)
{
    if (!point_init) {
        srand(0xdeadcafe);

        for (int i = 0; i < 256; i++) {
            point_pos[i].x = 128 + rand() % 256;
            point_pos[i].y = 128 + rand() % 256;

            if ((rand() % 2) == 0) {
                point_col[i] = QU_COLOR(255, 0, 0, 255);
            } else {
                point_col[i] = QU_COLOR(255, 255, 255, 255);
            }
        }

        point_init = true;
    }

    qu_clear(QU_COLOR(0, 0, 0, 255));

    for (int i = 0; i < 256; i++) {
        qu_draw_point(point_pos[i].x, point_pos[i].y, point_col[i]);
    }

    qu_present();
}

//------------------------------------------------------------------------------

static void test_lines(void)
{
    qu_clear(QU_COLOR(0, 0, 0, 255));

    for (int i = 0; i < 8; i++) {
        float f = 16.f + (60.f * (i + 1));
        qu_draw_line(16.f, f, f, 16.f, QU_COLOR(255, 255, 0, 255));
    }

    for (int i = 0; i < 7; i++) {
        float f = 496.f - (60.f * (i + 1));
        qu_draw_line(f, 496.f, 496.f, f, QU_COLOR(128, 0, 255, 255));
    }

    qu_present();
}

//------------------------------------------------------------------------------

static void calc_equilateral_triangle(float x, float y, float r,
    qu_vec2f *a, qu_vec2f *b, qu_vec2f *c)
{
    a->x = x;
    a->y = y - r;

    b->x = x + r * sqrtf(3.f) / 2.f;
    b->y = y + r / 2.f;

    c->x = x - r * sqrtf(3.f) / 2.f;
    c->y = y + r / 2.f;
}

static void draw_equilateral_triangle(float x, float y, float r,
    qu_color outline, qu_color fill)
{
    qu_vec2f a, b, c;
    calc_equilateral_triangle(x, y, r, &a, &b, &c);

    qu_draw_triangle(a.x, a.y, b.x, b.y, c.x, c.y, outline, fill);
}

static void test_triangles(void)
{
    qu_vec2f a, b, c;
    calc_equilateral_triangle(256.f, 272.f, 64.f, &a, &b, &c);

    qu_clear(QU_COLOR(0, 0, 0, 255));

    draw_equilateral_triangle(256.f, 272.f, 144.f, QU_COLOR(255, 0, 0, 255), 0);
    draw_equilateral_triangle(256.f, 272.f, 160.f, QU_COLOR(255, 0, 0, 255), 0);

    draw_equilateral_triangle(a.x, a.y, 64.f, 0, QU_COLOR(255, 0, 0, 255));
    draw_equilateral_triangle(b.x, b.y, 64.f, 0, QU_COLOR(255, 0, 0, 255));
    draw_equilateral_triangle(c.x, c.y, 64.f, 0, QU_COLOR(255, 0, 0, 255));

    qu_present();
}

//------------------------------------------------------------------------------

static void test_rectangles(void)
{
    qu_clear(QU_COLOR(0, 0, 0, 255));

    qu_draw_rectangle(56.f, 56.f, 400.f, 400.f, QU_COLOR(255, 0, 255, 255), 0);
    qu_draw_rectangle(64.f, 64.f, 384.f, 384.f, QU_COLOR(255, 0, 255, 255), 0);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i % 2) != (j % 2)) {
                continue;
            }

            float x = 64.f + 48.f * i;
            float y = 64.f + 48.f * j;

            qu_draw_rectangle(x, y, 48.f, 48.f, 0, QU_COLOR(255, 0, 255, 255));
        }
    }

    qu_present();
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] primitives: points");
    qu_set_window_size(512, 512);

    qu_initialize();
    atexit(qu_terminate);

    int mode = 0;

    while (qu_process()) {
        if (qu_is_key_pressed(QU_KEY_1)) {
            if (mode != 0) {
                qu_set_window_title("[libquack] primitives: points");
                mode = 0;
            }
        } else if (qu_is_key_pressed(QU_KEY_2)) {
            if (mode != 1) {
                qu_set_window_title("[libquack] primitives: lines");
                mode = 1;
            }
        } else if (qu_is_key_pressed(QU_KEY_3)) {
            if (mode != 2) {
                qu_set_window_title("[libquack] primitives: triangles");
                mode = 2;
            }
        } else if (qu_is_key_pressed(QU_KEY_4)) {
            if (mode != 3) {
                qu_set_window_title("[libquack] primitives: rectangles");
                mode = 3;
            }
        }

        switch (mode) {
        case 0:
            test_points();
            break;
        case 1:
            test_lines();
            break;
        case 2:
            test_triangles();
            break;
        case 3:
            test_rectangles();
            break;
        default:
            qu_clear(QU_COLOR(255, 0, 0, 255));
            qu_present();
            break;
        }
    }

    return 0;
}
