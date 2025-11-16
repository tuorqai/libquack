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

#include "qu_renderer.h"
#include "log.h"

//------------------------------------------------------------------------------

static r_impl const *impl_list[] = {
#ifdef QU_USE_OPENGL
    &r_impl_gl3,
#endif
};

//------------------------------------------------------------------------------

static r_impl const *impl;

//------------------------------------------------------------------------------

bool r_initialize(struct libqu_graphics_params const *params)
{
    int count = sizeof(impl_list) / sizeof(impl_list[0]);

    for (int i = 0; i < count; i++) {
        if (impl_list[i]->check_if_available()) {
            impl = impl_list[i];
        }
    }

    if (!impl) {
        return false;
    }

    if (!impl->initialize(params)) {
        impl = NULL;
        return false;
    }

    return true;
}

void r_terminate(void)
{
    if (impl) {
        impl->terminate();
    }
}

void r_upload_vertices(struct libqu_vertex *vertices, size_t count)
{
    if (impl) {
        impl->upload_vertices(vertices, count);
    }
}

void r_clear(qu_color color)
{
    if (impl) {
        impl->clear(color);
    }
}

void r_draw(enum libqu_draw_mode mode, size_t vertex, size_t count)
{
    if (impl) {
        impl->draw(mode, vertex, count);
    }
}

int r_load_texture(struct libqu_texture *texture)
{
    if (impl) {
        return impl->load_texture(texture);
    }

    return 0;
}

void r_destroy_texture(struct libqu_texture *texture)
{
    if (impl) {
        impl->destroy_texture(texture);
    }
}

void r_update_texture_flags(struct libqu_texture *texture)
{
    if (impl) {
        impl->update_texture_flags(texture);
    }
}

void r_apply_texture(struct libqu_texture *texture)
{
    if (impl) {
        impl->apply_texture(texture);
    }
}

void r_apply_ortho_proj(float l, float r, float b, float t)
{
    if (impl) {
        impl->apply_ortho_proj(l, r, b, t);
    }
}

void r_apply_blend_mode(qu_blend_mode const *mode)
{
    if (impl) {
        impl->apply_blend_mode(mode);
    }
}

int r_capture_screen(struct libqu_image *image)
{
    if (impl) {
        return impl->capture_screen(image);
    }

    return 0;
}

void r_set_transform(mat4_t const *transform)
{
    if (impl->set_transform) {
        impl->set_transform(transform);
    }
}
