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

int main(int argc, char *argv[])
{
    int mode = 0;

    qu_set_window_title("[libquack] blend-modes: none");
    qu_set_window_size(512, 512);

    qu_initialize();
    atexit(qu_terminate);

    float x0, y0, x1, y1, x2, y2;

    qu_set_blend_mode(QU_BLEND_MODE_NONE);

    while (qu_process()) {
        int update = 0;

        if (qu_is_key_pressed(QU_KEY_1)) {
            if (mode != 0) {
                qu_set_window_title("[libquack] blend-modes: none");
                mode = 0;
                update = 1;
            }
        } else if (qu_is_key_pressed(QU_KEY_2)) {
            if (mode != 1) {
                qu_set_window_title("[libquack] blend-modes: alpha");
                mode = 1;
                update = 1;
            }
        } else if (qu_is_key_pressed(QU_KEY_3)) {
            if (mode != 2) {
                qu_set_window_title("[libquack] blend-modes: add");
                mode = 2;
                update = 1;
            }
        } else if (qu_is_key_pressed(QU_KEY_4)) {
            if (mode != 3) {
                qu_set_window_title("[libquack] blend-modes: mul");
                mode = 3;
                update = 1;
            }
        }

        double t = qu_get_time_mediump();

        x0 = 256.f - 128.f * sinf(t);
        y0 = 256.f + 128.f * cosf(t);

        x1 = 256.f - 128.f * sinf(t * 1.5f);
        y1 = 256.f + 128.f * cosf(t * 1.5f);

        x2 = 256.f - 128.f * sinf(t * 2.0f);
        y2 = 256.f + 128.f * cosf(t * 2.0f);

        qu_clear(QU_COLOR(0, 0, 0, 255));

        if (update) {
            switch (mode) {
            case 0:
                qu_set_blend_mode(QU_BLEND_MODE_NONE);
                break;
            case 1:
                qu_set_blend_mode(QU_BLEND_MODE_ALPHA);
                break;
            case 2:
                qu_set_blend_mode(QU_BLEND_MODE_ADD);
                break;
            case 3:
                qu_set_blend_mode(QU_BLEND_MODE_MUL);
                break;
            }
        }

        qu_draw_rectangle(x0 - 32.f, y0 - 32.f, 64.f, 64.f,
            QU_COLOR(255, 255, 255, 255), QU_COLOR(255, 0, 0, 128));

        qu_draw_rectangle(x1 - 32.f, y1 - 32.f, 64.f, 64.f,
            QU_COLOR(255, 255, 255, 255), QU_COLOR(0, 255, 0, 128));

        qu_draw_rectangle(x2 - 32.f, y2 - 32.f, 64.f, 64.f,
            QU_COLOR(255, 255, 255, 255), QU_COLOR(0, 0, 255, 128));

        qu_present();
    }

    return 0;
}
