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

#include <stb_ds.h>
#include "graphics.h"
#include "log.h"

//------------------------------------------------------------------------------

static struct libqu_graphics_impl const *impl_list[] = {
#ifdef QU_USE_OPENGL
    &libqu_graphics_gl3_impl,
#endif

    &libqu_graphics_null_impl,
};

//------------------------------------------------------------------------------

enum renderop
{
    RENDEROP_CLEAR,
    RENDEROP_DRAW,
};

struct rendercmd
{
    enum renderop op;

    union {
        struct {
            qu_color color;
        } clear;

        struct {
            enum libqu_draw_mode mode;
            size_t vertex;
            size_t count;
        } draw;
    } args;
};

static struct
{
    struct libqu_graphics_impl const *impl;
    struct libqu_vertex *vertbuf;
    struct rendercmd *rendercmds;
} priv;

//------------------------------------------------------------------------------

static struct libqu_graphics_impl const *choose_impl(void)
{
    int count = sizeof(impl_list) / sizeof(impl_list[0]);

    for (int i = 0; i < count; i++) {
        if (impl_list[i]->check_if_available()) {
            return impl_list[i];
        }
    }

    abort();
}

static void exec_cmd(struct rendercmd const *cmd)
{
    switch (cmd->op) {
    case RENDEROP_CLEAR:
        priv.impl->clear(cmd->args.clear.color);
        break;
    case RENDEROP_DRAW:
        priv.impl->draw(cmd->args.draw.mode, cmd->args.draw.vertex, cmd->args.draw.count);
        break;
    default:
        break;
    }
}

static size_t append_vertices(struct libqu_vertex const *vertices, size_t count)
{
    size_t offset = arrlenu(priv.vertbuf);

    struct libqu_vertex *ptr = arraddnptr(priv.vertbuf, (int) count);
    memcpy(ptr, vertices, sizeof(*ptr) * count);

    return offset;
}

//------------------------------------------------------------------------------

void libqu_graphics_initialize(struct libqu_graphics_params const *params)
{
    priv.impl = choose_impl();

    if (!priv.impl->initialize(params)) {
        LIBQU_LOGE("Failed to initialize libqu::graphics implementation.\n");
        abort();
    }

    LIBQU_LOGI("Initialized.\n");
}

void libqu_graphics_terminate(void)
{
    arrfree(priv.rendercmds);
    priv.impl->terminate();

    memset(&priv, 0, sizeof(priv));

    LIBQU_LOGI("Terminated.\n");
}

void libqu_graphics_flush(void)
{
    priv.impl->upload_vertices(priv.vertbuf, arrlenu(priv.vertbuf));

    for (size_t i = 0; i < arrlenu(priv.rendercmds); i++) {
        exec_cmd(&priv.rendercmds[i]);
    }

    arrsetlen(priv.vertbuf, 0);
    arrsetlen(priv.rendercmds, 0);
}

void libqu_graphics_clear(qu_color color)
{
    struct rendercmd cmd = {
        .op = RENDEROP_CLEAR,
        .args = {
            .clear = {
                .color = color,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_draw_point(qu_vec2f pos, qu_color color)
{
    struct libqu_vertex vertices[] = {
        { pos, color },
    };

    struct rendercmd cmd = {
        .op = RENDEROP_DRAW,
        .args = {
            .draw = {
                .mode = LIBQU_DRAW_MODE_POINTS,
                .vertex = append_vertices(vertices, 1),
                .count = 1,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_draw_line(qu_vec2f a, qu_vec2f b, qu_color color)
{
    struct libqu_vertex vertices[] = {
        { a, color },
        { b, color },
    };

    struct rendercmd cmd = {
        .op = RENDEROP_DRAW,
        .args = {
            .draw = {
                .mode = LIBQU_DRAW_MODE_LINES,
                .vertex = append_vertices(vertices, 2),
                .count = 2,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_draw_triangle(qu_vec2f a, qu_vec2f b, qu_vec2f c, qu_color outline, qu_color fill)
{
    if (QU_EXTRACT_ALPHA(fill) > 0) {
        struct libqu_vertex vertices[] = {
            { a, fill },
            { b, fill },
            { c, fill },
        };

        struct rendercmd cmd = {
            .op = RENDEROP_DRAW,
            .args = {
                .draw = {
                    .mode = LIBQU_DRAW_MODE_TRIANGLE_FAN,
                    .vertex = append_vertices(vertices, 3),
                    .count = 3,
                },
            },
        };

        arrput(priv.rendercmds, cmd);
    }

    if (QU_EXTRACT_ALPHA(outline) > 0) {
        struct libqu_vertex vertices[] = {
            { a, outline },
            { b, outline },
            { c, outline },
        };

        struct rendercmd cmd = {
            .op = RENDEROP_DRAW,
            .args = {
                .draw = {
                    .mode = LIBQU_DRAW_MODE_LINE_LOOP,
                    .vertex = append_vertices(vertices, 3),
                    .count = 3,
                },
            },
        };

        arrput(priv.rendercmds, cmd);
    }
}

void libqu_graphics_draw_rectangle(qu_vec2f pos, qu_vec2f size, qu_color outline, qu_color fill)
{
    qu_vec2f a = { pos.x, pos.y };
    qu_vec2f b = { pos.x + size.x, pos.y };
    qu_vec2f c = { pos.x + size.x, pos.y + size.y };
    qu_vec2f d = { pos.x, pos.y + size.y };

    if (QU_EXTRACT_ALPHA(fill) > 0) {
        struct libqu_vertex vertices[] = {
            { a, fill },
            { b, fill },
            { c, fill },
            { d, fill },
        };

        struct rendercmd cmd = {
            .op = RENDEROP_DRAW,
            .args = {
                .draw = {
                    .mode = LIBQU_DRAW_MODE_TRIANGLE_FAN,
                    .vertex = append_vertices(vertices, 4),
                    .count = 4,
                },
            },
        };

        arrput(priv.rendercmds, cmd);
    }

    if (QU_EXTRACT_ALPHA(outline) > 0) {
        struct libqu_vertex vertices[] = {
            { a, outline },
            { b, outline },
            { c, outline },
            { d, outline },
        };

        struct rendercmd cmd = {
            .op = RENDEROP_DRAW,
            .args = {
                .draw = {
                    .mode = LIBQU_DRAW_MODE_LINE_LOOP,
                    .vertex = append_vertices(vertices, 4),
                    .count = 4,
                },
            },
        };

        arrput(priv.rendercmds, cmd);
    }
}

