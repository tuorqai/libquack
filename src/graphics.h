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

#ifndef LIBQU_GRAPHICS_H_INC
#define LIBQU_GRAPHICS_H_INC

//------------------------------------------------------------------------------

#include "algebra.h"
#include "fs.h"
#include "qu_image.h"

//------------------------------------------------------------------------------

enum libqu_draw_mode
{
    LIBQU_DRAW_MODE_POINTS,
    LIBQU_DRAW_MODE_LINES,
    LIBQU_DRAW_MODE_LINE_LOOP,
    LIBQU_DRAW_MODE_LINE_STRIP,
    LIBQU_DRAW_MODE_TRIANGLES,
    LIBQU_DRAW_MODE_TRIANGLE_STRIP,
    LIBQU_DRAW_MODE_TRIANGLE_FAN,
    LIBQU_TOTAL_DRAW_MODES,
};

struct libqu_vertex
{
    qu_vec2f pos;
    qu_color color;
    qu_vec2f texcoord;
};

struct libqu_texture
{
    struct libqu_image *image;
    unsigned int flags;
    uintptr_t priv[4];
};

struct libqu_graphics_params
{
    qu_vec2i window_size;
};

//------------------------------------------------------------------------------

void libqu_graphics_initialize(struct libqu_graphics_params const *params);
void libqu_graphics_terminate(void);
void libqu_graphics_flush(void);
void libqu_graphics_clear(qu_color color);
void libqu_graphics_draw_point(qu_vec2f pos, qu_color color);
void libqu_graphics_draw_line(qu_vec2f a, qu_vec2f b, qu_color color);
void libqu_graphics_draw_triangle(qu_vec2f a, qu_vec2f b, qu_vec2f c, qu_color outline, qu_color fill);
void libqu_graphics_draw_rectangle(qu_vec2f pos, qu_vec2f size, qu_color outline, qu_color fill);

qu_view libqu_graphics_get_default_view(void);
void libqu_graphics_set_view(qu_view view);

void libqu_graphics_push(void);
void libqu_graphics_pop(void);
void libqu_graphics_origin(void);
void libqu_graphics_translate(qu_vec2f v);
void libqu_graphics_scale(qu_vec2f s);
void libqu_graphics_rotate(float degrees);

void libqu_graphics_set_default_texture_flags(unsigned int flags);
struct libqu_texture *libqu_graphics_load_texture(struct libqu_image *image);
void libqu_graphics_destroy_texture(struct libqu_texture *texture);
void libqu_graphics_set_texture_flags(struct libqu_texture *texture, unsigned int flags);
void libqu_graphics_draw_texture(struct libqu_texture *texture, qu_rectf rect);
void libqu_graphics_draw_subtexture(struct libqu_texture *texture, qu_rectf rect, qu_rectf sub);

struct libqu_image *libqu_graphics_capture_screen(void);

void libqu_graphics_set_blend_mode(qu_blend_mode mode);

//------------------------------------------------------------------------------

#endif // LIBQU_GRAPHICS_H_INC

