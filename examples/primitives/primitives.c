
#include <libqu/libqu.h>

#define NUM_POINTS 500

int main(int argc, char *argv[])
{
    qu_vec2f points[NUM_POINTS];

    for (int i = 0; i < NUM_POINTS; i++) {
        points[i].x = rand() % 512;
        points[i].y = rand() % 512;
    }

    qu_initialize();

    while (qu_process()) {
        qu_clear(0x000000FF);

        for (int i = 0; i < NUM_POINTS; i++) {
            qu_draw_point(points[i].x, points[i].y, 0xFFFFFFFF);
        }

        qu_draw_line(0.f, 16.f, 512.f, 16.f, 0xFF0000FF);
        qu_draw_line(0.f, 496.f, 512.f, 496.f, 0xFF0000FF);

        qu_draw_rectangle(8.f, 8.f, 16.f, 16.f, 0xFF0000FF, 0x202020FF);
        qu_draw_rectangle(488.f, 488.f, 16.f, 16.f, 0xFF0000FF, 0x202020FF);

        qu_present();
    }

    qu_terminate();

    return 0;
}

