//------------------------------------------------------------------------------
// Copyright (c) 2021-2025 tuorqai
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

#include <string.h>
#include "qu_config.h"

//------------------------------------------------------------------------------

#define MAX_WINDOW_TITLE        256

//------------------------------------------------------------------------------

static struct {
    char window_title[MAX_WINDOW_TITLE];
    qu_vec2i window_size;
} cf;

//------------------------------------------------------------------------------

void cf_initialize(void)
{
    if (cf.window_title[0] == '\0') {
        strcpy(cf.window_title, "libquack");
    }

    if (cf.window_size.x == 0 || cf.window_size.y == 0) {
        cf.window_size.x = 1280;
        cf.window_size.y = 720;
    }
}

char const *cf_get_window_title(void)
{
    return cf.window_title;
}

void cf_set_window_title(char const *title)
{
    strncpy(cf.window_title, title, MAX_WINDOW_TITLE - 1);
}

qu_vec2i cf_get_window_size(void)
{
    return cf.window_size;
}

void cf_set_window_size(qu_vec2i size)
{
    cf.window_size = size;
}
