
# Types

## qu_key

```c
typedef enum {
    QU_KEY_INVALID = -1,
    QU_KEY_0, QU_KEY_1, QU_KEY_2, QU_KEY_3,
    QU_KEY_4, QU_KEY_5, QU_KEY_6, QU_KEY_7,
    QU_KEY_8, QU_KEY_9,
    QU_KEY_A, QU_KEY_B, QU_KEY_C, QU_KEY_D,
    QU_KEY_E, QU_KEY_F, QU_KEY_G, QU_KEY_H,
    QU_KEY_I, QU_KEY_J, QU_KEY_K, QU_KEY_L,
    QU_KEY_M, QU_KEY_N, QU_KEY_O, QU_KEY_P,
    QU_KEY_Q, QU_KEY_R, QU_KEY_S, QU_KEY_T,
    QU_KEY_U, QU_KEY_V, QU_KEY_W, QU_KEY_X,
    QU_KEY_Y, QU_KEY_Z,
    QU_KEY_GRAVE, QU_KEY_APOSTROPHE,
    QU_KEY_MINUS, QU_KEY_EQUAL,
    QU_KEY_LBRACKET, QU_KEY_RBRACKET,
    QU_KEY_COMMA, QU_KEY_PERIOD,
    QU_KEY_SEMICOLON, QU_KEY_SLASH,
    QU_KEY_BACKSLASH, QU_KEY_SPACE,
    QU_KEY_ESCAPE, QU_KEY_BACKSPACE,
    QU_KEY_TAB, QU_KEY_ENTER,
    QU_KEY_F1, QU_KEY_F2, QU_KEY_F3,
    QU_KEY_F4, QU_KEY_F5, QU_KEY_F6,
    QU_KEY_F7, QU_KEY_F8, QU_KEY_F9,
    QU_KEY_F10, QU_KEY_F11, QU_KEY_F12,
    QU_KEY_UP, QU_KEY_DOWN,
    QU_KEY_LEFT, QU_KEY_RIGHT,
    QU_KEY_LSHIFT, QU_KEY_RSHIFT,
    QU_KEY_LCTRL, QU_KEY_RCTRL,
    QU_KEY_LALT, QU_KEY_RALT,
    QU_KEY_LSUPER, QU_KEY_RSUPER,
    QU_KEY_MENU,
    QU_KEY_PGUP, QU_KEY_PGDN,
    QU_KEY_HOME, QU_KEY_END,
    QU_KEY_INSERT, QU_KEY_DELETE,
    QU_KEY_PRINTSCREEN,
    QU_KEY_PAUSE,
    QU_KEY_CAPSLOCK,
    QU_KEY_SCROLLLOCK,
    QU_KEY_NUMLOCK,
    QU_KEY_KP_0, QU_KEY_KP_1, QU_KEY_KP_2,
    QU_KEY_KP_3, QU_KEY_KP_4, QU_KEY_KP_5,
    QU_KEY_KP_6, QU_KEY_KP_7, QU_KEY_KP_8,
    QU_KEY_KP_9,
    QU_KEY_KP_MUL, QU_KEY_KP_ADD,
    QU_KEY_KP_SUB, QU_KEY_KP_POINT,
    QU_KEY_KP_DIV, QU_KEY_KP_ENTER,
    QU_TOTAL_KEYS,
} qu_key;
```

## qu_key_state

```c
typedef enum {
    QU_KEY_STATE_IDLE,
    QU_KEY_STATE_PRESSED,
    QU_KEY_STATE_RELEASED,
} qu_key_state;
```

# Functions

## qu_get_window_title

```c
const char *qu_get_window_title(void);
```

It's safe to call this function before `qu_initialize()`.

## qu_get_window_size

```c
qu_vec2i qu_get_window_size(void);
```

It's safe to call this function before `qu_initialize()`.

## qu_set_window_title

```c
void qu_set_window_title(char const *title);
```

It's safe to call this function before `qu_initialize()`.

## qu_set_window_size

```c
void qu_set_window_size(int w, int h);
```

It's safe to call this function before `qu_initialize()`.

## qu_is_window_active

```c
bool qu_is_window_active(void);
```

## qu_get_keyboard_state

```c
const qu_key_state *qu_get_keyboard_state(void);
```

## qu_get_key_state

```c
qu_key_state qu_get_key_state(qu_key key);
```

## qu_is_key_idle

```c
bool qu_is_key_idle(qu_key key);
```

## qu_is_key_pressed

```c
bool qu_is_key_pressed(qu_key key);
```

## qu_is_key_released

```c
bool qu_is_key_released(qu_key key);
```
