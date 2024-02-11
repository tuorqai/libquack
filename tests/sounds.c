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
#include <stdio.h>

//------------------------------------------------------------------------------

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] hello");
    qu_set_window_size(512, 512);

    qu_initialize();
    atexit(qu_terminate);

    qu_sound sine = qu_load_sound_from_file("assets/sounds/sine1000.wav");
    qu_sound square = qu_load_sound_from_file("assets/sounds/square250.wav");
    qu_sound noise = qu_load_sound_from_file("assets/sounds/noise.wav");

    printf("sine -> %d\n", sine.id);
    printf("square -> %d\n", square.id);
    printf("noise -> %d\n", noise.id);

    while (qu_process()) {
        qu_playback_state sine_state = qu_get_sound_state(sine);
        qu_playback_state square_state = qu_get_sound_state(square);
        qu_playback_state noise_state = qu_get_sound_state(noise);

        if (qu_is_key_pressed(QU_KEY_1)) {
            if (sine_state != QU_PLAYBACK_PLAYING) {
                qu_play_sound(sine);
            }
        }

        if (qu_is_key_pressed(QU_KEY_2)) {
            if (square_state != QU_PLAYBACK_PLAYING) {
                qu_play_sound(square);
            }
        }

        if (qu_is_key_pressed(QU_KEY_3)) {
            if (noise_state != QU_PLAYBACK_PLAYING) {
                qu_play_sound(noise);
            }
        }

        double x = qu_get_time_mediump() * 10.f;

        int r = 128.0 * sin(x) + 127.0;
        int g = 128.0 * sin(x + (2.0 * M_PI) / 3.0) + 127.0;
        int b = 128.0 * sin(x + (4.0 * M_PI) / 3.0) + 127.0;

        qu_clear(QU_COLOR(0, 0, 0, 255));

        qu_draw_rectangle(96.f, 208.f, 64.f, 64.f,
            0, (sine_state == QU_PLAYBACK_PLAYING)
                ? QU_COLOR(r, g, b, 255)
                : QU_COLOR(32, 32, 32, 255));

        qu_draw_rectangle(224.f, 208.f, 64.f, 64.f,
            0, (square_state == QU_PLAYBACK_PLAYING)
                ? QU_COLOR(r, g, b, 255)
                : QU_COLOR(32, 32, 32, 255));

        qu_draw_rectangle(352.f, 208.f, 64.f, 64.f,
            0, (noise_state == QU_PLAYBACK_PLAYING)
                ? QU_COLOR(r, g, b, 255)
                : QU_COLOR(32, 32, 32, 255));

        qu_present();
    }

    return 0;
}
