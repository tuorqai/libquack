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

#include "core.h"
#include "graphics.h"
#include "log.h"

//------------------------------------------------------------------------------

static struct
{
    int refcount;

    struct {
        struct libqu_core_params core;
        struct libqu_graphics_params graphics;
    } params;
} priv;

//------------------------------------------------------------------------------

void qu_initialize(void)
{
    if (++priv.refcount != 1) {
        LIBQU_LOGW("Already initialized.\n");
        return;
    }

    libqu_core_initialize(&priv.params.core);
    libqu_graphics_initialize(&priv.params.graphics);

    LIBQU_LOGI("Initialized.\n");
}

void qu_terminate(void)
{
    if (--priv.refcount != 0) {
        return;
    }

    libqu_graphics_terminate();
    libqu_core_terminate();

    LIBQU_LOGI("Terminated.\n");
}

bool qu_process(void)
{
    return libqu_core_process();
}

void qu_present(void)
{
    libqu_graphics_flush();
    libqu_core_swap();
}

void qu_clear(qu_color color)
{
    libqu_graphics_clear(color);
}

void qu_draw_point(float x, float y, qu_color color)
{
    qu_vec2f xy = { x, y };

    libqu_graphics_draw_point(xy, color);
}

void qu_draw_line(float ax, float ay, float bx, float by, qu_color color)
{
    qu_vec2f a = { ax, ay };
    qu_vec2f b = { bx, by };

    libqu_graphics_draw_line(a, b, color);
}

void qu_draw_triangle(float ax, float ay, float bx, float by, float cx, float cy, qu_color outline, qu_color fill)
{
    qu_vec2f a = { ax, ay };
    qu_vec2f b = { bx, by };
    qu_vec2f c = { cx, cy };

    libqu_graphics_draw_triangle(a, b, c, outline, fill);
}

void qu_draw_rectangle(float x, float y, float w, float h, qu_color outline, qu_color fill)
{
    qu_vec2f xy = { x, y };
    qu_vec2f wh = { w, h };

    libqu_graphics_draw_rectangle(xy, wh, outline, fill);
}

