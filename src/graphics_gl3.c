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

#ifdef QU_USE_OPENGL

//------------------------------------------------------------------------------

#include "graphics.h"
#include "log.h"

//------------------------------------------------------------------------------

static bool graphics_gl3_check_if_available(void)
{
    return true;
}

static bool graphics_gl3_initialize(struct libqu_graphics_params const *params)
{
    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void graphics_gl3_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

//------------------------------------------------------------------------------

struct libqu_graphics_impl const libqu_graphics_gl3_impl = {
    graphics_gl3_check_if_available,
    graphics_gl3_initialize,
    graphics_gl3_terminate,
};

//------------------------------------------------------------------------------

#endif // QU_USE_OPENGL

