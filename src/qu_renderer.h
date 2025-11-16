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

#ifndef QU_RENDERER_H_INC
#define QU_RENDERER_H_INC

//------------------------------------------------------------------------------

#include "graphics.h"

//------------------------------------------------------------------------------

typedef struct r_impl
{
    bool (*check_if_available)(void);
    bool (*initialize)(struct libqu_graphics_params const *params);
    void (*terminate)(void);
    void (*upload_vertices)(struct libqu_vertex *vertices, size_t count);
    void (*clear)(qu_color color);
    void (*draw)(enum libqu_draw_mode mode, size_t vertex, size_t count);
    int (*load_texture)(struct libqu_texture *texture);
    void (*destroy_texture)(struct libqu_texture *texture);
    void (*update_texture_flags)(struct libqu_texture *texture);
    void (*apply_texture)(struct libqu_texture *texture);
    void (*apply_ortho_proj)(float l, float r, float b, float t);
    void (*apply_blend_mode)(qu_blend_mode const *mode);
    int (*capture_screen)(struct libqu_image *image);
    void (*set_transform)(mat4_t const *transform);
} r_impl;

//------------------------------------------------------------------------------

#ifdef QU_USE_OPENGL
extern r_impl const r_impl_gl3;
#endif

//------------------------------------------------------------------------------

bool r_initialize(struct libqu_graphics_params const *params);
void r_terminate(void);
void r_upload_vertices(struct libqu_vertex *vertices, size_t count);
void r_clear(qu_color color);
void r_draw(enum libqu_draw_mode mode, size_t vertex, size_t count);
int r_load_texture(struct libqu_texture *texture);
void r_destroy_texture(struct libqu_texture *texture);
void r_update_texture_flags(struct libqu_texture *texture);
void r_apply_texture(struct libqu_texture *texture);
void r_apply_ortho_proj(float l, float r, float b, float t);
void r_apply_blend_mode(qu_blend_mode const *mode);
int r_capture_screen(struct libqu_image *image);
void r_set_transform(mat4_t const *transform);

//------------------------------------------------------------------------------

#endif // QU_RENDERER_H_INC
