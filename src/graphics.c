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
#include <stb_image.h>
#include "graphics.h"
#include "log.h"
#include "platform.h"
#include "qu_renderer.h"

//------------------------------------------------------------------------------

#define MAX_MATRICES            256

//------------------------------------------------------------------------------

enum renderop
{
    RENDEROP_CLEAR,
    RENDEROP_DRAW,
    RENDEROP_SET_PROJECTION,
    RENDEROP_MATRIX,
    RENDEROP_SET_BLEND_MODE,
};

enum matrixop
{
    MATRIXOP_PUSH,
    MATRIXOP_POP,
    MATRIXOP_ORIGIN,
    MATRIXOP_TRANSLATE,
    MATRIXOP_SCALE,
    MATRIXOP_ROTATE,
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
            struct libqu_texture *texture;
        } draw;

        struct {
            int matrixop;
            float x;
            float y;
        } matrix;

        struct {
            float l;
            float r;
            float b;
            float t;
        } set_projection;

        struct {
            qu_blend_mode mode;
        } set_blend_mode;
    } args;
};

static struct
{
    struct libqu_vertex *vertbuf;
    struct rendercmd *rendercmds;
    mat4_t matrices[MAX_MATRICES];
    size_t current_matrix;
    unsigned int default_texture_flags;
    qu_vec2i window_size;
} priv;

//------------------------------------------------------------------------------

static void exec_renderop_matrix(int matrixop, float x, float y)
{
    switch (matrixop) {
    case MATRIXOP_PUSH:
        if (priv.current_matrix < MAX_MATRICES - 1) {
            priv.current_matrix++;
            mat4_copy(
                &priv.matrices[priv.current_matrix],
                &priv.matrices[priv.current_matrix - 1]
            );
        } else {
            LIBQU_LOGW("matrix stack overflow!\n");
        }
        break;
    case MATRIXOP_POP:
        if (priv.current_matrix > 0) {
            priv.current_matrix--;
            r_set_transform(&priv.matrices[priv.current_matrix]);
        } else {
            LIBQU_LOGW("matrix stack underflow!\n");
        }
        break;
    case MATRIXOP_ORIGIN:
        mat4_identity(&priv.matrices[priv.current_matrix]);
        r_set_transform(&priv.matrices[priv.current_matrix]);
        break;
    case MATRIXOP_TRANSLATE:
        mat4_translate(&priv.matrices[priv.current_matrix], x, y, 0.f);
        r_set_transform(&priv.matrices[priv.current_matrix]);
        break;
    case MATRIXOP_SCALE:
        mat4_scale(&priv.matrices[priv.current_matrix], x, y, 1.f);
        r_set_transform(&priv.matrices[priv.current_matrix]);
        break;
    case MATRIXOP_ROTATE:
        mat4_rotate(&priv.matrices[priv.current_matrix], x, 0.f, 0.f, 1.f);
        r_set_transform(&priv.matrices[priv.current_matrix]);
        break;
    }
}

static void exec_cmd(struct rendercmd const *cmd)
{
    switch (cmd->op) {
    case RENDEROP_CLEAR:
        r_clear(cmd->args.clear.color);
        break;
    case RENDEROP_DRAW:
        r_apply_texture(cmd->args.draw.texture);
        r_draw(cmd->args.draw.mode, cmd->args.draw.vertex, cmd->args.draw.count);
        break;
    case RENDEROP_SET_PROJECTION:
        r_apply_ortho_proj(
            cmd->args.set_projection.l,
            cmd->args.set_projection.r,
            cmd->args.set_projection.b,
            cmd->args.set_projection.t
        );
        break;
    case RENDEROP_MATRIX:
        exec_renderop_matrix(cmd->args.matrix.matrixop,
            cmd->args.matrix.x, cmd->args.matrix.y);
        break;
    case RENDEROP_SET_BLEND_MODE:
        r_apply_blend_mode(&cmd->args.set_blend_mode.mode);
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
    if (!r_initialize(params)) {
        LIBQU_LOGE("Failed to initialize libqu::renderer implementation.\n");
        LIBQU_LOGE("Expect no graphics at all.\n");
    }

    priv.window_size = params->window_size;

    LIBQU_LOGI("Initialized.\n");
}

void libqu_graphics_terminate(void)
{
    arrfree(priv.rendercmds);
    r_terminate();

    memset(&priv, 0, sizeof(priv));

    LIBQU_LOGI("Terminated.\n");
}

void libqu_graphics_flush(void)
{
    r_upload_vertices(priv.vertbuf, arrlenu(priv.vertbuf));

    for (size_t i = 0; i < arrlenu(priv.rendercmds); i++) {
        exec_cmd(&priv.rendercmds[i]);
    }

    arrsetlen(priv.vertbuf, 0);
    arrsetlen(priv.rendercmds, 0);

    priv.current_matrix = 0;
    mat4_identity(&priv.matrices[0]);
    r_set_transform(&priv.matrices[0]);
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
        { .pos = pos, .color = color },
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
        { .pos = a, .color = color },
        { .pos = b, .color = color },
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
            { .pos = a, .color = fill },
            { .pos = b, .color = fill },
            { .pos = c, .color = fill },
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
            { .pos = a, .color = outline },
            { .pos = b, .color = outline },
            { .pos = c, .color = outline },
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
    float ax = pos.x;
    float ay = pos.y;
    float bx = pos.x + size.x;
    float by = pos.y + size.y;

    if (QU_EXTRACT_ALPHA(fill) > 0) {
        struct libqu_vertex vertices[] = {
            { .pos = { ax, ay }, .color = fill },
            { .pos = { bx, ay }, .color = fill },
            { .pos = { bx, by }, .color = fill },
            { .pos = { ax, by }, .color = fill },
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
            { .pos = { ax, ay }, .color = outline },
            { .pos = { bx, ay }, .color = outline },
            { .pos = { bx, by }, .color = outline },
            { .pos = { ax, by }, .color = outline },
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

qu_view libqu_graphics_get_default_view(void)
{
    return (qu_view) {
        .center = {
            .x = (float) priv.window_size.x / 2.f,
            .y = (float) priv.window_size.y / 2.f,
        },
        .size = {
            .x = (float) priv.window_size.x,
            .y = (float) priv.window_size.y,
        },
    };
}

void libqu_graphics_set_view(qu_view view)
{
    struct rendercmd cmd = {
        .op = RENDEROP_SET_PROJECTION,
        .args = {
            .set_projection = {
                .l = view.center.x - view.size.x / 2.f,
                .r = view.center.x + view.size.x / 2.f,
                .b = view.center.y + view.size.y / 2.f,
                .t = view.center.y - view.size.y / 2.f,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_push(void)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_PUSH,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_pop(void)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_POP,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_origin(void)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_ORIGIN,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_translate(qu_vec2f v)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_TRANSLATE,
                .x = v.x,
                .y = v.y,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_scale(qu_vec2f s)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_SCALE,
                .x = s.x,
                .y = s.y,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_rotate(float degrees)
{
    struct rendercmd cmd = {
        .op = RENDEROP_MATRIX,
        .args = {
            .matrix = {
                .matrixop = MATRIXOP_ROTATE,
                .x = QU_DEG2RAD(degrees),
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

//------------------------------------------------------------------------------

void libqu_graphics_set_default_texture_flags(unsigned int flags)
{
    priv.default_texture_flags = flags;
}

struct libqu_texture *libqu_graphics_load_texture(struct libqu_image *image)
{
    struct libqu_texture *texture = pl_calloc(1, sizeof(*texture));

    if (texture) {
        texture->image = image;
        texture->flags = priv.default_texture_flags;

        if (r_load_texture(texture) == 0) {
            return texture;
        }

        pl_free(texture);
    }

    libqu_image_destroy(image);

    return NULL;
}

void libqu_graphics_destroy_texture(struct libqu_texture *texture)
{
    r_destroy_texture(texture);
    libqu_image_destroy(texture->image);
    pl_free(texture);
}

void libqu_graphics_set_texture_flags(struct libqu_texture *texture,
    unsigned int flags)
{
    texture->flags = flags;
    r_update_texture_flags(texture);
}

void libqu_graphics_draw_texture(struct libqu_texture *texture, qu_rectf rect)
{
    float ax = rect.x;
    float ay = rect.y;
    float bx = rect.x + rect.w;
    float by = rect.y + rect.h;

    struct libqu_vertex vertices[] = {
        { { ax, ay }, 0xFFFFFFFF, { 0.f, 0.f } },
        { { bx, ay }, 0xFFFFFFFF, { 1.f, 0.f } },
        { { bx, by }, 0xFFFFFFFF, { 1.f, 1.f } },
        { { ax, by }, 0xFFFFFFFF, { 0.f, 1.f } },
    };

    struct rendercmd cmd = {
        .op = RENDEROP_DRAW,
        .args = {
            .draw = {
                .mode = LIBQU_DRAW_MODE_TRIANGLE_FAN,
                .vertex = append_vertices(vertices, 4),
                .count = 4,
                .texture = texture,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

void libqu_graphics_draw_subtexture(struct libqu_texture *texture,
    qu_rectf rect, qu_rectf sub)
{
    float ax = rect.x;
    float ay = rect.y;
    float bx = rect.x + rect.w;
    float by = rect.y + rect.h;

    float s = sub.x / texture->image->size.x;
    float t = sub.y / texture->image->size.y;
    float u = (sub.x + sub.w) / texture->image->size.x;
    float v = (sub.y + sub.h) / texture->image->size.y;

    struct libqu_vertex vertices[] = {
        { { ax, ay }, 0xFFFFFFFF, { s, t } },
        { { bx, ay }, 0xFFFFFFFF, { u, t } },
        { { bx, by }, 0xFFFFFFFF, { u, v } },
        { { ax, by }, 0xFFFFFFFF, { s, v } },
    };

    struct rendercmd cmd = {
        .op = RENDEROP_DRAW,
        .args = {
            .draw = {
                .mode = LIBQU_DRAW_MODE_TRIANGLE_FAN,
                .vertex = append_vertices(vertices, 4),
                .count = 4,
                .texture = texture,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}

struct libqu_image *libqu_graphics_capture_screen(void)
{
    struct libqu_image *image =
        libqu_image_create(QU_PIXFMT_R8G8B8, priv.window_size);
    
    if (!image) {
        return NULL;
    }

    if (r_capture_screen(image) == -1) {
        libqu_image_destroy(image);
        return NULL;
    }

    return image;
}

void libqu_graphics_set_blend_mode(qu_blend_mode mode)
{
    struct rendercmd cmd = {
        .op = RENDEROP_SET_BLEND_MODE,
        .args = {
            .set_blend_mode = {
                .mode = mode,
            },
        },
    };

    arrput(priv.rendercmds, cmd);
}
