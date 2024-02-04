
## Hello, world!

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

## Basic functions

### `qu_get_version`

#### Signature

```c
const char *qu_get_version(void);
```

#### Return value

Version string, e.g. `"0.1.0"`.

#### Description

This function may be useful to query version of `libquack` if it's dynamically linked.

The returned value is a static string, so there is no need to `free()` it.

### `qu_initialize`

#### Signature

```c
void qu_initialize(void);
```

#### Description

Initialize the `libquack` library.

Call to this function will trigger initialization of the two most important modules of the library: `core` and `graphics`. The window is created and renderer is initialized during initialization.

*If an error occurs, the library will call `abort()` under the hood and the entire program will be terminated.*

### `qu_terminate`

#### Signature

```c
void qu_terminate(void);
```

#### Description

The library is shut down and all resources are freed when this function is called.

It's highly recommended to call this function before the program exits.

### `qu_process`

#### Signature

```c
bool qu_process(void);
```

#### Return value

* `false`: program should exit
* `true`: program can keep running

#### Description

This function processes user input (e.g. key presses) and it's recommended to call it every frame. Normally it returns `true`, but if user wants to exit from the program, this function will return `false`. When that happens, the program should close as soon as possible.

Recommended way to use this function is to put it as a condition in the main `while` loop.

### `qu_present`

#### Signature

```c
void qu_present(void);
```

#### Description

Render everything on screen.

## Time-related functions

### `qu_get_ticks_msec`

#### Signature

```c
uint32_t qu_get_ticks_msec(void);
```

#### Return value

Amount of milliseconds since library initialized.

#### Description

This function returns how many milliseconds passed since call to `qu_initialize()`.

### `qu_get_ticks_nsec`

#### Signature

```c
uint64_t qu_get_ticks_nsec(void);
```

#### Return value

Amount of nanoseconds since library initialized.

#### Description

This function returns how many nanoseconds passed since call to `qu_initialize()`.

### `qu_get_time_mediump`

#### Signature

```c
float qu_get_time_mediump(void);
```

#### Return value

Amount of seconds since library initialized (medium precision).

#### Description

This function returns how many seconds passed since call to `qu_initialize()`. The value is millisecond precise.

### `qu_get_time_highp`

#### Signature

```c
double qu_get_time_highp(void);
```

#### Return value

Amount of seconds since library initialized (high precision).

#### Description

This function returns how many seconds passed since call to `qu_initialize()`. The value is at least microsecond precise.

### `qu_get_date_time`

#### Signature

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

#### Return value

Current system date and time.

#### Description

Use this function to retrieve local date and time.

## Core

[here](./core/)

## Graphics

[here](./graphics/)

## Audio

[here](./audio/)
