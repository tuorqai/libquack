
Hello, world!
-------------

```c
#include <libqu.h>

int main(int argc, char *argv[])
{
    qu_set_window_title("Hello!");
    qu_set_window_size(512, 512);

    qu_initialize();

    while (qu_process()) {
        qu_clear(QU_COLOR(0, 0, 0, 255));
        qu_present();
    }

    qu_terminate();
}
```

Basic functions
---------------

## `qu_get_version`

```c
const char *qu_get_version(void);
```

- Return value:
    - version string, e.g. `"0.1.0"`.

This function may be useful to query version of `libquack` if it's dynamically linked.

The returned value is a static string, so there is no need to `free()` it.

## `qu_initialize`

```c
void qu_initialize(void);
```

Initialize the `libquack` library.

Call to this function will trigger initialization of the two most important modules of the library: `core` and `graphics`. The window is created and renderer is initialized during initialization.

*If an error occurs, the library will call `abort()` under the hood and the entire program will be terminated.*

## `qu_terminate`

```c
void qu_terminate(void);
```

The library is shut down and all resources are freed when this function is called.

It's highly recommended to call this function before the program exits.

## `qu_process`

```c
bool qu_process(void);
```

- Return value:
    - `true`: program can keep running
    - `false`: program should exit

This function processes user input (e.g. key presses) and it's recommended to call it every frame. Normally it returns `true`, but if user wants to exit from the program, this function will return `false`. When that happens, the program should close as soon as possible.

Recommended way to use this function is to put it as a condition in the main `while` loop.

## `qu_present`

```c
void qu_present(void);
```

Render everything on screen. This function should be called every frame, otherwise nothing will be drawn.

Time-related functions
----------------------

## `qu_get_ticks_msec`

```c
uint32_t qu_get_ticks_msec(void);
```

- Return value:
    - amount of milliseconds since library initialized.

This function returns how many milliseconds passed since call to `qu_initialize()`.

## `qu_get_ticks_nsec`

```c
uint64_t qu_get_ticks_nsec(void);
```

- Return value:
    - amount of nanoseconds since library initialized.

This function returns how many nanoseconds passed since call to `qu_initialize()`.

## `qu_get_time_mediump`

```c
float qu_get_time_mediump(void);
```

- Return value:
    - amount of seconds since library initialized (medium precision).

This function returns how many seconds passed since call to `qu_initialize()`. The value is millisecond precise.

## `qu_get_time_highp`

```c
double qu_get_time_highp(void);
```

- Return value:
    - amount of seconds since library initialized (high precision).

This function returns how many seconds passed since call to `qu_initialize()`. The value is at least microsecond precise.

## `qu_get_date_time`

```c
typedef struct
{
    int year;
    int month;
    int day;
    int weekday;
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
} qu_date_time;

qu_date_time qu_get_date_time(void);
```

- Return value:
    - current system date and time.

Use this function to retrieve local date and time.

Core
----

[see here](./core/)

Graphics
--------

[see here](./graphics/)

Audio
-----

[see here](./audio/)
