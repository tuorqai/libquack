
#include <libqu/libqu.h>

int main(int argc, char *argv[])
{
    qu_vec2f pos = { 256.f, 256.f };
    qu_vec2f size = { 32.f, 32.f };

    qu_set_window_title("[libquack] keyboard.c");
    qu_set_window_size(512, 512);

    qu_initialize();

    while (qu_process()) {
        if (qu_is_key_pressed(QU_KEY_LEFT)) {
            pos.x -= 1.f;
        }

        if (qu_is_key_pressed(QU_KEY_RIGHT)) {
            pos.x += 1.f;
        }
    
        if (qu_is_key_pressed(QU_KEY_UP)) {
            pos.y -= 1.f;
        }

        if (qu_is_key_pressed(QU_KEY_DOWN)) {
            pos.y += 1.f;
        }

        qu_clear(0x202020FF);

        float x = pos.x - size.x / 2.f;
        float y = pos.y - size.y / 2.f;

        qu_draw_rectangle(x, y, size.x, size.y, 0xE00000FF, 0xF0F0F0FF);
        qu_present();
    }

    qu_terminate();

    return 0;
}