//------------------------------------------------------------------------------
// Copyright (c) 2021-2025 tuorqai
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
    qu_set_window_title("[libquack] views");
    qu_set_window_size(512, 512);

    qu_initialize();
    atexit(qu_terminate);

    qu_view view = {
        .center = { 256, 256 },
        .size = { 512, 512 },
    };
    
    float then = (float) qu_get_time_highp();
    float now = then;

    while (qu_process()) {
        then = now;
        now = qu_get_time_highp();

        float dt = now - then;

        float speed = 180.f;

        if (qu_is_key_pressed(QU_KEY_LSHIFT)) {
            speed *= 3.f;
        }

        if (qu_is_key_pressed(QU_KEY_W)) {
            view.center.y -= speed * dt;
        }

        if (qu_is_key_pressed(QU_KEY_S)) {
            view.center.y += speed * dt;
        }

        if (qu_is_key_pressed(QU_KEY_A)) {
            view.center.x -= speed * dt;
        }

        if (qu_is_key_pressed(QU_KEY_D)) {
            view.center.x += speed * dt;
        }

        if (qu_is_key_pressed(QU_KEY_Q)) {
            view.size.x -= speed * dt;
            view.size.y -= speed * dt;
        }

        if (qu_is_key_pressed(QU_KEY_E)) {
            view.size.x += speed * dt;
            view.size.y += speed * dt;
        }

        double x = qu_get_time_mediump();

        int r = 128.0 * sin(x) + 127.0;
        int g = 128.0 * sin(x + (2.0 * M_PI) / 3.0) + 127.0;
        int b = 128.0 * sin(x + (4.0 * M_PI) / 3.0) + 127.0;

        qu_clear(QU_COLOR(0, 0, 0, 255));
        qu_set_view(view);
        qu_draw_rectangle(128.f, 128.f, 256.f, 256.f, 0, QU_COLOR(r, g, b, 255));
        qu_draw_rectangle(144.f, 144.f, 224.f, 224.f, 0, QU_COLOR(0, 0, 0, 255));
        qu_present();
    }

    return 0;
}
