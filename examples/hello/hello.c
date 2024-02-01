
#include <math.h>
#include <libqu/libqu.h>

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] hello.c");
    qu_set_window_size(512, 512);

    qu_initialize();

    while (qu_process()) {
        double x = qu_get_time_mediump();

        int r = 128.0 * sin(x) + 127.0;
        int g = 128.0 * sin(x + (2.0 * M_PI) / 3.0) + 127.0;
        int b = 128.0 * sin(x + (4.0 * M_PI) / 3.0) + 127.0;

        qu_clear(QU_COLOR(0, 0, 0, 255));
        qu_draw_rectangle(128.f, 128.f, 256.f, 256.f, 0, QU_COLOR(r, g, b, 255));
        qu_draw_rectangle(144.f, 144.f, 224.f, 224.f, 0, QU_COLOR(0, 0, 0, 255));
        qu_present();
    }

    qu_terminate();

    return 0;
}

