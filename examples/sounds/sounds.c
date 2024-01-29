
#include <stdio.h>
#include <libqu/libqu.h>

static qu_color key_state_color(qu_key_state state)
{
    if (state == QU_KEY_STATE_PRESSED) {
        return 0x606060FF;
    } else if (state == QU_KEY_STATE_RELEASED) {
        return 0x00E000FF;
    }

    return 0xF0F0F0FF;
}

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] sounds.c");
    qu_set_window_size(512, 512);

    qu_initialize();

    qu_set_master_volume(0.5f);

    qu_sound blip[3], coin[3], shotgun[3];

    blip[0] = qu_load_sound_from_file("assets/sfx_sounds_Blip1.wav");
    blip[1] = qu_load_sound_from_file("assets/sfx_sounds_Blip2.wav");
    blip[2] = qu_load_sound_from_file("assets/sfx_sounds_Blip3.wav");

    coin[0] = qu_load_sound_from_file("assets/sfx_coin_single1.wav");
    coin[1] = qu_load_sound_from_file("assets/sfx_coin_single2.wav");
    coin[2] = qu_load_sound_from_file("assets/sfx_coin_single3.wav");

    shotgun[0] = qu_load_sound_from_file("assets/sfx_weapon_shotgun1.wav");
    shotgun[1] = qu_load_sound_from_file("assets/sfx_weapon_shotgun2.wav");
    shotgun[2] = qu_load_sound_from_file("assets/sfx_weapon_shotgun3.wav");

    while (qu_process()) {
        if (qu_is_key_released(QU_KEY_1)) {
            qu_play_sound(blip[rand() % 3]);
        }

        if (qu_is_key_released(QU_KEY_2)) {
            qu_play_sound(coin[rand() % 3]);
        }

        if (qu_is_key_released(QU_KEY_3)) {
            qu_play_sound(shotgun[rand() % 3]);
        }

        qu_clear(0x202020FF);

        // TODO: draw something

        qu_present();
    }

    qu_terminate();

    return 0;
}