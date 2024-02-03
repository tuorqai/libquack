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

#include <math.h>
#include <libqu/libqu.h>

//------------------------------------------------------------------------------

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------------

static qu_rectf const key_geometry[QU_TOTAL_KEYS] = {
    [QU_KEY_0]              = { 360.f,  52.f,   36.f,   36.f },
    [QU_KEY_1]              = { 36.f,   52.f,   36.f,   36.f },
    [QU_KEY_2]              = { 72.f,   52.f,   36.f,   36.f },
    [QU_KEY_3]              = { 108.f,  52.f,   36.f,   36.f },
    [QU_KEY_4]              = { 144.f,  52.f,   36.f,   36.f },
    [QU_KEY_5]              = { 180.f,  52.f,   36.f,   36.f },
    [QU_KEY_6]              = { 216.f,  52.f,   36.f,   36.f },
    [QU_KEY_7]              = { 252.f,  52.f,   36.f,   36.f },
    [QU_KEY_8]              = { 288.f,  52.f,   36.f,   36.f },
    [QU_KEY_9]              = { 324.f,  52.f,   36.f,   36.f },
    [QU_KEY_A]              = { 70.f,   124.f,  36.f,   36.f },
    [QU_KEY_B]              = { 230.f,  160.f,  36.f,   36.f },
    [QU_KEY_C]              = { 158.f,  160.f,  36.f,   36.f },
    [QU_KEY_D]              = { 142.f,  124.f,  36.f,   36.f },
    [QU_KEY_E]              = { 126.f,  88.f,   36.f,   36.f },
    [QU_KEY_F]              = { 178.f,  124.f,  36.f,   36.f },
    [QU_KEY_G]              = { 214.f,  124.f,  36.f,   36.f },
    [QU_KEY_H]              = { 250.f,  124.f,  36.f,   36.f },
    [QU_KEY_I]              = { 306.f,  88.f,   36.f,   36.f },
    [QU_KEY_J]              = { 286.f,  124.f,  36.f,   36.f },
    [QU_KEY_K]              = { 322.f,  124.f,  36.f,   36.f },
    [QU_KEY_L]              = { 358.f,  124.f,  36.f,   36.f },
    [QU_KEY_M]              = { 302.f,  160.f,  36.f,   36.f },
    [QU_KEY_N]              = { 266.f,  160.f,  36.f,   36.f },
    [QU_KEY_O]              = { 342.f,  88.f,   36.f,   36.f },
    [QU_KEY_P]              = { 378.f,  88.f,   36.f,   36.f },
    [QU_KEY_Q]              = { 54.f,   88.f,   36.f,   36.f },
    [QU_KEY_R]              = { 162.f,  88.f,   36.f,   36.f },
    [QU_KEY_S]              = { 106.f,  124.f,  36.f,   36.f },
    [QU_KEY_T]              = { 198.f,  88.f,   36.f,   36.f },
    [QU_KEY_U]              = { 270.f,  88.f,   36.f,   36.f },
    [QU_KEY_V]              = { 194.f,  160.f,  36.f,   36.f },
    [QU_KEY_W]              = { 90.f,   88.f,   36.f,   36.f },
    [QU_KEY_X]              = { 122.f,  160.f,  36.f,   36.f },
    [QU_KEY_Y]              = { 234.f,  88.f,   36.f,   36.f },
    [QU_KEY_Z]              = { 86.f,   160.f,  36.f,   36.f },
    [QU_KEY_GRAVE]          = { 0.f,    52.f,   36.f,   36.f },
    [QU_KEY_APOSTROPHE]     = { 430.f,  124.f,  36.f,   36.f },
    [QU_KEY_MINUS]          = { 396.f,  52.f,   36.f,   36.f },
    [QU_KEY_EQUAL]          = { 432.f,  52.f,   36.f,   36.f },
    [QU_KEY_LBRACKET]       = { 414.f,  88.f,   36.f,   36.f },
    [QU_KEY_RBRACKET]       = { 450.f,  88.f,   36.f,   36.f },
    [QU_KEY_COMMA]          = { 338.f,  160.f,  36.f,   36.f },
    [QU_KEY_PERIOD]         = { 374.f,  160.f,  36.f,   36.f },
    [QU_KEY_SEMICOLON]      = { 394.f,  124.f,  36.f,   36.f },
    [QU_KEY_SLASH]          = { 410.f,  160.f,  36.f,   36.f },
    [QU_KEY_BACKSLASH]      = { 486.f,  88.f,   54.f,   36.f },
    [QU_KEY_SPACE]          = { 142.f,  196.f,  212.f,  36.f },
    [QU_KEY_ESCAPE]         = { 0.f,    0.f,    36.f,   36.f },
    [QU_KEY_BACKSPACE]      = { 468.f,  52.f,   72.f,   36.f },
    [QU_KEY_TAB]            = { 0.f,    88.f,   54.f,   36.f },
    [QU_KEY_ENTER]          = { 466.f,  124.f,  74.f,   36.f },
    [QU_KEY_F1]             = { 72.f,   0.f,    36.f,   36.f },
    [QU_KEY_F2]             = { 108.f,  0.f,    36.f,   36.f },
    [QU_KEY_F3]             = { 144.f,  0.f,    36.f,   36.f },
    [QU_KEY_F4]             = { 180.f,  0.f,    36.f,   36.f },
    [QU_KEY_F5]             = { 232.f,  0.f,    36.f,   36.f },
    [QU_KEY_F6]             = { 268.f,  0.f,    36.f,   36.f },
    [QU_KEY_F7]             = { 304.f,  0.f,    36.f,   36.f },
    [QU_KEY_F8]             = { 340.f,  0.f,    36.f,   36.f },
    [QU_KEY_F9]             = { 396.f,  0.f,    36.f,   36.f },
    [QU_KEY_F10]            = { 432.f,  0.f,    36.f,   36.f },
    [QU_KEY_F11]            = { 468.f,  0.f,    36.f,   36.f },
    [QU_KEY_F12]            = { 504.f,  0.f,    36.f,   36.f },
    [QU_KEY_UP]             = { 592.f,  160.f,  36.f,   36.f },
    [QU_KEY_DOWN]           = { 592.f,  196.f,  36.f,   36.f },
    [QU_KEY_LEFT]           = { 556.f,  196.f,  36.f,   36.f },
    [QU_KEY_RIGHT]          = { 628.f,  196.f,  36.f,   36.f },
    [QU_KEY_LSHIFT]         = { 0.f,    160.f,  86.f,   36.f },
    [QU_KEY_RSHIFT]         = { 446.f,  160.f,  94.f,   36.f },
    [QU_KEY_LCTRL]          = { 0.f,    196.f,  54.f,   36.f },
    [QU_KEY_RCTRL]          = { 486.f,  196.f,  54.f,   36.f },
    [QU_KEY_LALT]           = { 98.f,   196.f,  44.f,   36.f },
    [QU_KEY_RALT]           = { 354.f,  196.f,  44.f,   36.f },
    [QU_KEY_LSUPER]         = { 54.f,   196.f,  44.f,   36.f },
    [QU_KEY_RSUPER]         = { 398.f,  196.f,  44.f,   36.f },
    [QU_KEY_MENU]           = { 442.f,  196.f,  44.f,   36.f },
    [QU_KEY_PGUP]           = { 628.f,  52.f,   36.f,   36.f },
    [QU_KEY_PGDN]           = { 628.f,  88.f,   36.f,   36.f },
    [QU_KEY_HOME]           = { 592.f,  52.f,   36.f,   36.f },
    [QU_KEY_END]            = { 592.f,  88.f,   36.f,   36.f },
    [QU_KEY_INSERT]         = { 556.f,  52.f,   36.f,   36.f },
    [QU_KEY_DELETE]         = { 556.f,  88.f,   36.f,   36.f },
    [QU_KEY_PRINTSCREEN]    = { 556.f,  0.f,    36.f,   36.f },
    [QU_KEY_PAUSE]          = { 628.f,  0.f,    36.f,   36.f },
    [QU_KEY_CAPSLOCK]       = { 0.f,    124.f,  70.f,   36.f },
    [QU_KEY_SCROLLLOCK]     = { 592.f,  0.f,    36.f,   36.f },
    [QU_KEY_NUMLOCK]        = { 680.f,  52.f,   36.f,   36.f },
    [QU_KEY_KP_0]           = { 680.f,  196.f,  72.f,   36.f },
    [QU_KEY_KP_1]           = { 680.f,  160.f,  36.f,   36.f },
    [QU_KEY_KP_2]           = { 716.f,  160.f,  36.f,   36.f },
    [QU_KEY_KP_3]           = { 752.f,  160.f,  36.f,   36.f },
    [QU_KEY_KP_4]           = { 680.f,  124.f,  36.f,   36.f },
    [QU_KEY_KP_5]           = { 716.f,  124.f,  36.f,   36.f },
    [QU_KEY_KP_6]           = { 752.f,  124.f,  36.f,   36.f },
    [QU_KEY_KP_7]           = { 680.f,  88.f,   36.f,   36.f },
    [QU_KEY_KP_8]           = { 716.f,  88.f,   36.f,   36.f },
    [QU_KEY_KP_9]           = { 752.f,  88.f,   36.f,   36.f },
    [QU_KEY_KP_MUL]         = { 752.f,  52.f,   36.f,   36.f },
    [QU_KEY_KP_ADD]         = { 788.f,  88.f,   36.f,   72.f },
    [QU_KEY_KP_SUB]         = { 788.f,  52.f,   36.f,   36.f },
    [QU_KEY_KP_POINT]       = { 752.f,  196.f,  36.f,   36.f },
    [QU_KEY_KP_DIV]         = { 716.f,  52.f,   36.f,   36.f },
    [QU_KEY_KP_ENTER]       = { 788.f,  160.f,  36.f,   72.f },
};

static qu_color const key_colors[QU_TOTAL_KEYS] = {
    [QU_KEY_0]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_1]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_2]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_3]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_4]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_5]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_6]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_7]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_8]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_9]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_A]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_B]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_C]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_D]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_E]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_F]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_G]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_H]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_I]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_J]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_K]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_L]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_M]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_N]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_O]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_P]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_Q]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_R]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_S]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_T]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_U]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_V]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_W]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_X]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_Y]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_Z]              = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_GRAVE]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_APOSTROPHE]     = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_MINUS]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_EQUAL]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_LBRACKET]       = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_RBRACKET]       = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_COMMA]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_PERIOD]         = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_SEMICOLON]      = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_SLASH]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_BACKSLASH]      = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_SPACE]          = QU_COLOR(255, 255, 255, 255),
    [QU_KEY_ESCAPE]         = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_BACKSPACE]      = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_TAB]            = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_ENTER]          = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_F1]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F2]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F3]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F4]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F5]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F6]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F7]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F8]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F9]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F10]            = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F11]            = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_F12]            = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_UP]             = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_DOWN]           = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_LEFT]           = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_RIGHT]          = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_LSHIFT]         = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_RSHIFT]         = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_LCTRL]          = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_RCTRL]          = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_LALT]           = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_RALT]           = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_LSUPER]         = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_RSUPER]         = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_MENU]           = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_PGUP]           = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_PGDN]           = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_HOME]           = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_END]            = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_INSERT]         = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_DELETE]         = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_PRINTSCREEN]    = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_PAUSE]          = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_CAPSLOCK]       = QU_COLOR(255, 255, 0, 255),
    [QU_KEY_SCROLLLOCK]     = QU_COLOR(0, 255, 128, 255),
    [QU_KEY_NUMLOCK]        = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_0]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_1]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_2]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_3]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_4]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_5]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_6]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_7]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_8]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_9]           = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_MUL]         = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_ADD]         = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_SUB]         = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_POINT]       = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_DIV]         = QU_COLOR(255, 0, 128, 255),
    [QU_KEY_KP_ENTER]       = QU_COLOR(255, 0, 128, 255),
};

//------------------------------------------------------------------------------

static qu_key_state const   *keyboard_state;
static qu_color             border_color;

//------------------------------------------------------------------------------

static void update(void)
{
    keyboard_state = qu_get_keyboard_state();

    double x = qu_get_time_mediump();

    int r = 128.0 * sin(x) + 127.0;
    int g = 128.0 * sin(x + (2.0 * M_PI) / 3.0) + 127.0;
    int b = 128.0 * sin(x + (4.0 * M_PI) / 3.0) + 127.0;

    border_color = QU_COLOR(r, g, b, 255);
}

static void draw(void)
{
    qu_clear(QU_COLOR(0, 0, 0, 255));
    qu_draw_rectangle(8.f, 8.f, 872.f, 280.f, border_color, 0);

    float ox = 32.f;
    float oy = 32.f;

    for (int i = 0; i < QU_TOTAL_KEYS; i++) {
        float x = ox + key_geometry[i].x + 2.f;
        float y = oy + key_geometry[i].y + 2.f;
        float w = key_geometry[i].w - 4.f;
        float h = key_geometry[i].h - 4.f;

        qu_color c = key_colors[i];

        if (keyboard_state[i] == QU_KEY_STATE_PRESSED) {
            x += 2.f;
            y += 2.f;
            w -= 4.f;
            h -= 4.f;
        } else if (keyboard_state[i] == QU_KEY_STATE_RELEASED) {
            x -= 4.f;
            y -= 4.f;
            w += 8.f;
            h += 8.f;
        }

        qu_draw_rectangle(x, y, w, h, c, 0);
    }

    qu_present();
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    qu_set_window_title("[libquack] keyboard");
    qu_set_window_size(888, 296);

    qu_initialize();
    atexit(qu_terminate);

    while (qu_process()) {
        update();
        draw();
    }

    return 0;
}
