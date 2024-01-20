
#include <libqu/libqu.h>

int main(int argc, char *argv[])
{
    qu_initialize();

    while (qu_process()) {
        qu_present();
    }

    qu_terminate();

    return 0;
}

