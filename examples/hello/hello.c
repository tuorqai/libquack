
#include <libqu/libqu.h>

int main(int argc, char *argv[])
{
    qu_initialize();

    while (qu_process()) {
        qu_clear(0x103050FF);
        qu_present();
    }

    qu_terminate();

    return 0;
}

