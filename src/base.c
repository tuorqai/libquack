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
        return;
    }

    libqu_core_initialize(&priv.params.core);
    libqu_graphics_initialize(&priv.params.graphics);
}

void qu_terminate(void)
{
    if (--priv.refcount != 0) {
        return;
    }

    libqu_graphics_terminate();
    libqu_core_terminate();
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

