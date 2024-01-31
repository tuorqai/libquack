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

#include "graphics.h"
#include "log.h"

//------------------------------------------------------------------------------

static bool graphics_null_check_if_available(void)
{
    return true;
}

static bool graphics_null_initialize(struct libqu_graphics_params const *params)
{
    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void graphics_null_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

static void graphics_null_upload_vertices(struct libqu_vertex *vertices, size_t count)
{
}

static void graphics_null_clear(qu_color color)
{
}

static void graphics_null_draw(enum libqu_draw_mode mode, size_t vertex, size_t count)
{
}

static int graphics_null_load_texture(struct libqu_texture *texture)
{
    return 0;
}

static void graphics_null_destroy_texture(struct libqu_texture *texture)
{
}

static void graphics_null_update_texture_flags(struct libqu_texture *texture)
{
}

static void graphics_null_apply_texture(struct libqu_texture *texture)
{
}

//------------------------------------------------------------------------------

struct libqu_graphics_impl const libqu_graphics_null_impl = {
    graphics_null_check_if_available,
    graphics_null_initialize,
    graphics_null_terminate,
    graphics_null_upload_vertices,
    graphics_null_clear,
    graphics_null_draw,
    graphics_null_load_texture,
    graphics_null_destroy_texture,
    graphics_null_update_texture_flags,
    graphics_null_apply_texture,
};

