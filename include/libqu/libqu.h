//------------------------------------------------------------------------------
// Copyright (c) 2021-2024 tuorqai
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//------------------------------------------------------------------------------

#ifndef LIBQU_H_INC
#define LIBQU_H_INC

//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#ifdef QU_SHARED
    #ifdef _WIN32
        #ifdef QU_BUILD
            #define QU_API __declspec(dllexport)
        #else
            #define QU_API __declspec(dllimport)
        #endif
        #define QU_CALL __cdecl
    #else
        #ifdef __GNUC__
            #define QU_API __attribute__((visibility("default")))
        #endif
    #endif
#endif

#ifndef QU_API
    #define QU_API
#endif

#ifndef QU_CALL
    #define QU_CALL
#endif

//------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {
#endif

//------------------------------------------------------------------------------

#define QU_COLOR(r, g, b, a)        ((r) << 24 | (g) << 16 | (b) << 8 | (a))

#define QU_EXTRACT_RED(color)       (((color) >> 24) & 255)
#define QU_EXTRACT_GREEN(color)     (((color) >> 16) & 255)
#define QU_EXTRACT_BLUE(color)      (((color) >> 8) & 255)
#define QU_EXTRACT_ALPHA(color)     ((color) & 255)

//------------------------------------------------------------------------------

typedef uint32_t qu_color;

typedef enum qu_key
{
    QU_KEY_INVALID = -1,        /*!< Special value for invalid key */
    QU_KEY_0 = 0,               /*!< '0' key */
    QU_KEY_1,                   /*!< '1' key */
    QU_KEY_2,                   /*!< '2' key */
    QU_KEY_3,                   /*!< '3' key */
    QU_KEY_4,                   /*!< '4' key */
    QU_KEY_5,                   /*!< '5' key */
    QU_KEY_6,                   /*!< '6' key */
    QU_KEY_7,                   /*!< '7' key */
    QU_KEY_8,                   /*!< '8' key */
    QU_KEY_9,                   /*!< '9' key */
    QU_KEY_A,                   /*!< 'A' key */
    QU_KEY_B,                   /*!< 'B' key */
    QU_KEY_C,                   /*!< 'C' key */
    QU_KEY_D,                   /*!< 'D' key */
    QU_KEY_E,                   /*!< 'E' key */
    QU_KEY_F,                   /*!< 'F' key */
    QU_KEY_G,                   /*!< 'G' key */
    QU_KEY_H,                   /*!< 'H' key */
    QU_KEY_I,                   /*!< 'I' key */
    QU_KEY_J,                   /*!< 'J' key */
    QU_KEY_K,                   /*!< 'K' key */
    QU_KEY_L,                   /*!< 'L' key */
    QU_KEY_M,                   /*!< 'M' key */
    QU_KEY_N,                   /*!< 'N' key */
    QU_KEY_O,                   /*!< 'O' key */
    QU_KEY_P,                   /*!< 'P' key */
    QU_KEY_Q,                   /*!< 'Q' key */
    QU_KEY_R,                   /*!< 'R' key */
    QU_KEY_S,                   /*!< 'S' key */
    QU_KEY_T,                   /*!< 'T' key */
    QU_KEY_U,                   /*!< 'U' key */
    QU_KEY_V,                   /*!< 'V' key */
    QU_KEY_W,                   /*!< 'W' key */
    QU_KEY_X,                   /*!< 'X' key */
    QU_KEY_Y,                   /*!< 'Y' key */
    QU_KEY_Z,                   /*!< 'Z' key */
    QU_KEY_GRAVE,               /*!< Backtick (tilde) key */
    QU_KEY_APOSTROPHE,          /*!< Quote key */
    QU_KEY_MINUS,               /*!< Minus (underscore) key */
    QU_KEY_EQUAL,               /*!< Equal (plus) key */
    QU_KEY_LBRACKET,            /*!< Left square bracket key */
    QU_KEY_RBRACKET,            /*!< Right square bracket key */
    QU_KEY_COMMA,               /*!< Comma key */
    QU_KEY_PERIOD,              /*!< Period key */
    QU_KEY_SEMICOLON,           /*!< Semicolon key */
    QU_KEY_SLASH,               /*!< Slash key */
    QU_KEY_BACKSLASH,           /*!< Backslash key */
    QU_KEY_SPACE,               /*!< Space key */
    QU_KEY_ESCAPE,              /*!< Escape key */
    QU_KEY_BACKSPACE,           /*!< Backspace key */
    QU_KEY_TAB,                 /*!< Tab key */
    QU_KEY_ENTER,               /*!< Enter key */
    QU_KEY_F1,                  /*!< F1 key */
    QU_KEY_F2,                  /*!< F2 key */
    QU_KEY_F3,                  /*!< F3 key */
    QU_KEY_F4,                  /*!< F4 key */
    QU_KEY_F5,                  /*!< F5 key */
    QU_KEY_F6,                  /*!< F6 key */
    QU_KEY_F7,                  /*!< F7 key */
    QU_KEY_F8,                  /*!< F8 key */
    QU_KEY_F9,                  /*!< F9 key */
    QU_KEY_F10,                 /*!< F10 key */
    QU_KEY_F11,                 /*!< F11 key */
    QU_KEY_F12,                 /*!< F12 key */
    QU_KEY_UP,                  /*!< Up arrow key */
    QU_KEY_DOWN,                /*!< Down arrow key */
    QU_KEY_LEFT,                /*!< Left arrow key */
    QU_KEY_RIGHT,               /*!< Right arrow key */
    QU_KEY_LSHIFT,              /*!< Left Shift key */
    QU_KEY_RSHIFT,              /*!< Right Shift key */
    QU_KEY_LCTRL,               /*!< Left Control key */
    QU_KEY_RCTRL,               /*!< Right Control key */
    QU_KEY_LALT,                /*!< Left Alt key */
    QU_KEY_RALT,                /*!< Right Alt (AltGr) key */
    QU_KEY_LSUPER,              /*!< Left OS (Windows) key */
    QU_KEY_RSUPER,              /*!< Right OS (Windows) key */
    QU_KEY_MENU,                /*!< Context Menu key */
    QU_KEY_PGUP,                /*!< PageUp key */
    QU_KEY_PGDN,                /*!< PageDown key */
    QU_KEY_HOME,                /*!< Home key */
    QU_KEY_END,                 /*!< End key */
    QU_KEY_INSERT,              /*!< Insert key */
    QU_KEY_DELETE,              /*!< Delete key */
    QU_KEY_PRINTSCREEN,         /*!< PrintScreen key */
    QU_KEY_PAUSE,               /*!< Pause (Break) key */
    QU_KEY_CAPSLOCK,            /*!< Caps Lock key */
    QU_KEY_SCROLLLOCK,          /*!< Scroll Lock key */
    QU_KEY_NUMLOCK,             /*!< Num Lock key */
    QU_KEY_KP_0,                /*!< Numpad 0 key */
    QU_KEY_KP_1,                /*!< Numpad 1 key */
    QU_KEY_KP_2,                /*!< Numpad 2 key */
    QU_KEY_KP_3,                /*!< Numpad 3 key */
    QU_KEY_KP_4,                /*!< Numpad 4 key */
    QU_KEY_KP_5,                /*!< Numpad 5 key */
    QU_KEY_KP_6,                /*!< Numpad 6 key */
    QU_KEY_KP_7,                /*!< Numpad 7 key */
    QU_KEY_KP_8,                /*!< Numpad 8 key */
    QU_KEY_KP_9,                /*!< Numpad 9 key */
    QU_KEY_KP_MUL,              /*!< Numpad Multiply key */
    QU_KEY_KP_ADD,              /*!< Numpad Add key */
    QU_KEY_KP_SUB,              /*!< Numpad Subtract key */
    QU_KEY_KP_POINT,            /*!< Numpad Decimal Point key */
    QU_KEY_KP_DIV,              /*!< Numpad Divide key */
    QU_KEY_KP_ENTER,            /*!< Numpad Enter */
    QU_TOTAL_KEYS,              /*!< Total count of supported keys */
} qu_key;

typedef enum qu_key_state
{
    QU_KEY_STATE_IDLE,
    QU_KEY_STATE_PRESSED,
    QU_KEY_STATE_RELEASED,
} qu_key_state;

typedef struct qu_vec2i
{
    int x;
    int y;
} qu_vec2i;

typedef struct qu_vec2f
{
    float x;
    float y;
} qu_vec2f;

//------------------------------------------------------------------------------

QU_API void QU_CALL qu_initialize(void);
QU_API void QU_CALL qu_terminate(void);
QU_API bool QU_CALL qu_process(void);
QU_API void QU_CALL qu_present(void);

QU_API char const * QU_CALL qu_get_window_title(void);
QU_API qu_vec2i QU_CALL qu_get_window_size(void);

QU_API void QU_CALL qu_set_window_title(char const *title);
QU_API void QU_CALL qu_set_window_size(int w, int h);

QU_API qu_key_state const * QU_CALL qu_get_keyboard_state(void);
QU_API qu_key_state QU_CALL qu_get_key_state(qu_key key);
QU_API bool QU_CALL qu_is_key_idle(qu_key key);
QU_API bool QU_CALL qu_is_key_pressed(qu_key key);
QU_API bool QU_CALL qu_is_key_released(qu_key key);

QU_API void QU_CALL qu_clear(qu_color color);
QU_API void QU_CALL qu_draw_point(float x, float y, qu_color color);
QU_API void QU_CALL qu_draw_line(float ax, float ay, float bx, float by, qu_color color);
QU_API void QU_CALL qu_draw_triangle(float ax, float ay, float bx, float by, float cx, float cy, qu_color outline, qu_color fill);
QU_API void QU_CALL qu_draw_rectangle(float x, float y, float w, float h, qu_color outline, qu_color fill);

//------------------------------------------------------------------------------

#if defined(__cplusplus)
} // extern "C"
#endif

//------------------------------------------------------------------------------

#endif // LIBQU_H_INC

