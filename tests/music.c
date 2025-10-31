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
    qu_set_window_title("[libquack] music");
    qu_set_window_size(512, 512);

    qu_initialize();
    atexit(qu_terminate);

    qu_music track = qu_open_music_from_file("assets/music/balda.ogg");

    printf("track -> %d\n", track.id);

    printf("channels = %d\n", qu_get_music_channel_count(track));
    printf("samples = %d\n", qu_get_music_sample_count(track));
    printf("sample rate = %d\n", qu_get_music_sample_rate(track));
    printf("duration = %.2f\n", qu_get_music_duration(track));

    qu_set_music_loop(track, -1);

    while (qu_process()) {
        double dur = qu_get_music_duration(track);
        double pos = qu_get_music_position(track);

        qu_playback_state state = qu_get_music_state(track);

        if (qu_is_key_released(QU_KEY_SPACE)) {
            if (state == QU_PLAYBACK_PLAYING) {
                qu_pause_music(track);
            } else {
                qu_play_music(track);
            }
        }

        if (qu_is_key_released(QU_KEY_BACKSPACE)) {
            qu_stop_music(track);
        }

        if (qu_is_key_released(QU_KEY_LEFT)) {
            qu_seek_music(track, pos - 1.0);
        }

        if (qu_is_key_released(QU_KEY_RIGHT)) {
            qu_seek_music(track, pos + 1.0);
        }

        double m = (state == QU_PLAYBACK_PLAYING) ? 10.0 : 1.0;
        double x = qu_get_time_mediump() * m;

        int r = 128.0 * sin(x) + 127.0;
        int g = 128.0 * sin(x + (2.0 * M_PI) / 3.0) + 127.0;
        int b = 128.0 * sin(x + (4.0 * M_PI) / 3.0) + 127.0;

        qu_clear(QU_COLOR(0, 0, 0, 255));

        float len = (512.f / dur) * pos;

        qu_draw_rectangle(0.f, 496.f, 512.f, 16.f, 0, QU_COLOR(32, 32, 32, 255));
        qu_draw_rectangle(0.f, 496.f, len, 16.f, 0, QU_COLOR(r, g, b, 255));

        qu_present();

        char title[256];

        sprintf(title, "[libquack] music (dur=%.2lf, pos=%.2lf)", dur, pos);
        
        qu_set_window_title(title);
    }

    return 0;
}
