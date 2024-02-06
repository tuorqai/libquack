
Cheat sheet
-----------

## Base

| Function          | Description                                           |
|:------------------|:------------------------------------------------------|
| `qu_initialize`   | Initialize libquack: create window, setup renderer    |
| `qu_terminate`    | Terminate libquack                                    |
| `qu_process`      | Process input, returns `false` if app is being closed |
| `qu_present`      | Present on screen what was drawn so far               |

### Time-related functions

| Function              | Return value                                      |
|:----------------------|:--------------------------------------------------|
| `qu_get_ticks_msec`   | Ticks (millisec) since libquack was initialized   |
| `qu_get_ticks_nsec`   | Ticks (nanosec) since libquack was initialized    |
| `qu_get_time_mediump` | Seconds since libquack was initialized            |
| `qu_get_time_highp`   | Seconds since libquack was initialized (precise)  |
| `qu_get_date_time`    | Struct with local date and time                   |

## Core

### Window state and parameters

| Function                  | Description                   |
|:--------------------------|:------------------------------|
| `qu_get_window_title`     | Get window title              |
| `qu_get_window_size`      | Get window size in pixels     |
| `qu_set_window_title`     | Set window title              |
| `qu_set_window_size`      | Set window size in pixels     |
| `qu_is_window_active`     | Check if window is focused    |

### Keyboard input

| Function                  | Description                               |
|:--------------------------|:------------------------------------------|
| `qu_get_keyboard_state`   | Get array of key states for each key      |
| `qu_get_key_state`        | Get key state (idle, pressed, released)   |
| `qu_is_key_idle`          | Check if key is not pressed               |
| `qu_is_key_pressed`       | Check if key is pressed                   |
| `qu_is_key_released`      | Check if key is being released            |

## Graphics

| Function      | Description           |
|:--------------|-----------------------|
| `qu_clear`    | Clear entire screen   |

### Primitives

| Function              | Description       |
|:----------------------|-------------------|
| `qu_draw_point`       | Draw single point |
| `qu_draw_line`        | Draw line         |
| `qu_draw_triangle`    | Draw triangle     |
| `qu_draw_rectangle`   | Draw rectangle    |

### Images

Image is an object that holds pixel data on CPU side. It can't be drawn on
screen, but its pixels can be manipulated directly.

| Function                      | Description                               |
|:------------------------------|:------------------------------------------|
| `qu_create_image`             | Create image object                       | 
| `qu_load_image_from_file`     | Load image from file                      |
| `qu_load_image_from_buffer`   | Load image from memory buffer             |
| `qu_destroy_image`            | Destroy image object                      |
| `qu_get_image_size`           | Get image size in pixels                  |
| `qu_get_image_format`         | Get image pixel format                    |
| `qu_get_image_pixels`         | Get pointer to pixel data (read/write)    |

### Textures

Texture is similar to image, but is stored on GPU side and can be rendered,
but it has a drawback that it can't be changed.

| Function                          | Description                           |
|:----------------------------------|:--------------------------------------|
| `qu_set_default_texture_flags`    | Set default texture flags             |
| `qu_load_texture_from_file`       | Load image from file into texture     |
| `qu_load_texture_from_buffer`     | Load image from memory into texture   |
| `qu_load_texture_from_image`      | Convert image into texture            |
| `qu_destroy_texture`              | Destroy texture object                |
| `qu_get_texture_size`             | Get underlying image size             |
| `qu_get_texture_format`           | Get underlying image format           |
| `qu_get_texture_flags`            | Get texture flags                     |
| `qu_set_texture_flags`            | Set texture flags                     |
| `qu_draw_texture`                 | Draw entire texture on a rectangle    |
| `qu_draw_subtexture`              | Draw part of texture on a rectangle   |

### Miscellaneous

| Function              | Description                       |
|:----------------------|-----------------------------------|
| `qu_capture_screen`   | Capture screen into image object  |

## Audio

| Function                  | Description               |
|:--------------------------|---------------------------|
| `qu_set_master_volume`    | Set volume level (0 to 1) |
