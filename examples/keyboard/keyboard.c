
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
    qu_set_window_title("[libquack] keyboard.c");
    qu_set_window_size(512, 512);

    qu_initialize();

    while (qu_process()) {
        qu_clear(0x202020FF);

        qu_draw_rectangle(240.f, 220.f, 32.f, 32.f, 0xE00000FF,
            key_state_color(qu_get_key_state(QU_KEY_UP)));

        qu_draw_rectangle(204.f, 260.f, 32.f, 32.f, 0xE00000FF,
            key_state_color(qu_get_key_state(QU_KEY_LEFT)));

        qu_draw_rectangle(240.f, 260.f, 32.f, 32.f, 0xE00000FF,
            key_state_color(qu_get_key_state(QU_KEY_DOWN)));

        qu_draw_rectangle(276.f, 260.f, 32.f, 32.f, 0xE00000FF,
            key_state_color(qu_get_key_state(QU_KEY_RIGHT)));

        qu_present();
    }

    qu_terminate();

    return 0;
}