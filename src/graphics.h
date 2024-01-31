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

#include "fs.h"

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
};

struct libqu_image
{
    qu_pixel_format format;
    qu_vec2i size;
    unsigned char *pixels;
};

struct libqu_texture
{
    struct libqu_image *image;
};

struct libqu_graphics_params
{
    qu_vec2i window_size;
};

struct libqu_graphics_impl
{
    bool (*check_if_available)(void);
    bool (*initialize)(struct libqu_graphics_params const *params);
    void (*terminate)(void);
    void (*upload_vertices)(struct libqu_vertex *vertices, size_t count);
    void (*clear)(qu_color color);
    void (*draw)(enum libqu_draw_mode mode, size_t vertex, size_t count);
};

//------------------------------------------------------------------------------

extern struct libqu_graphics_impl const libqu_graphics_null_impl;

#ifdef QU_USE_OPENGL
extern struct libqu_graphics_impl const libqu_graphics_gl3_impl;
#endif

//------------------------------------------------------------------------------

void libqu_graphics_initialize(struct libqu_graphics_params const *params);
void libqu_graphics_terminate(void);
void libqu_graphics_flush(void);
void libqu_graphics_clear(qu_color color);
void libqu_graphics_draw_point(qu_vec2f pos, qu_color color);
void libqu_graphics_draw_line(qu_vec2f a, qu_vec2f b, qu_color color);
void libqu_graphics_draw_triangle(qu_vec2f a, qu_vec2f b, qu_vec2f c, qu_color outline, qu_color fill);
void libqu_graphics_draw_rectangle(qu_vec2f pos, qu_vec2f size, qu_color outline, qu_color fill);

struct libqu_image *libqu_image_create(qu_pixel_format format, qu_vec2i size);
struct libqu_image *libqu_image_load(struct libqu_file *file);
void libqu_image_destroy(struct libqu_image *image);

struct libqu_texture *libqu_graphics_load_texture(struct libqu_image *image);
void libqu_graphics_destroy_texture(struct libqu_texture *texture);
void libqu_graphics_draw_texture(struct libqu_texture *texture, qu_rectf rect);
void libqu_graphics_draw_subtexture(struct libqu_texture *texture, qu_rectf rect, qu_rectf sub);

//------------------------------------------------------------------------------

#endif // LIBQU_GRAPHICS_H_INC

