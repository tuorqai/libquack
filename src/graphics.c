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

//------------------------------------------------------------------------------

static int _stbi_io_read(struct libqu_file *file, char *data, int size)
{
    return (int) libqu_fread(data, size, file);
}

static void _stbi_io_skip(struct libqu_file *file, int n)
{
    libqu_fseek(file, n, SEEK_CUR);
}

static int _stbi_io_eof(struct libqu_file *file)
{
    return libqu_ftell(file) == (int64_t) file->size;
}

typedef int (*_stbi_io_read_func)(void *, char *, int);
typedef void (*_stbi_io_skip_func)(void *, int);
typedef int (*_stbi_io_eof_func)(void *);

static stbi_io_callbacks const _stbi_io = {
    .read = (_stbi_io_read_func) _stbi_io_read,
    .skip = (_stbi_io_skip_func) _stbi_io_skip,
    .eof = (_stbi_io_eof_func) _stbi_io_eof,
};

//------------------------------------------------------------------------------

static int pixfmt_to_channels(qu_pixel_format format)
{
    switch (format) {
    default:
        return 0;
    case QU_PIXFMT_Y8:
        return 1;
    case QU_PIXFMT_Y8A8:
        return 2;
    case QU_PIXFMT_R8G8B8:
        return 3;
    case QU_PIXFMT_R8G8B8A8:
        return 4;
    }
}

static qu_pixel_format channels_to_pixfmt(int channels)
{
    switch (channels) {
    default:
        return QU_PIXFMT_INVALID;
    case 1:
        return QU_PIXFMT_Y8;
    case 2:
        return QU_PIXFMT_Y8A8;
    case 3:
        return QU_PIXFMT_R8G8B8;
    case 4:
        return QU_PIXFMT_R8G8B8A8;
    }
}

struct libqu_image *libqu_image_create(qu_pixel_format format, qu_vec2i size)
{
    struct libqu_image *image = pl_calloc(1, sizeof(*image));

    if (image) {
        int channels = pixfmt_to_channels(format);

        if (channels > 0) {
            image->pixels = pl_malloc(channels * size.x * size.y);

            if (image->pixels) {
                image->format = format;
                image->size.x = size.x;
                image->size.y = size.y;

                return image;
            }
        }

        pl_free(image);
    }

    return NULL;
}

struct libqu_image *libqu_image_load(struct libqu_file *file)
{
    int w, h, c;

    unsigned char *data =
        stbi_load_from_callbacks(&_stbi_io, file, &w, &h, &c, 0);
    
    struct libqu_image *image = NULL;

    if (data) {
        qu_pixel_format format = channels_to_pixfmt(c);

        if (format != QU_PIXFMT_INVALID) {
            qu_vec2i size = { w, h };
            image = libqu_image_create(format, size);

            if (image) {
                memcpy(image->pixels, data, w * h * c);
            }
        }

        stbi_image_free(data);
    }

    return image;
}

void libqu_image_destroy(struct libqu_image *image)
{
    pl_free(image->pixels);
    pl_free(image);
}

//------------------------------------------------------------------------------

struct libqu_texture *libqu_graphics_load_texture(struct libqu_image *image)
{
    libqu_image_destroy(image);

    return NULL;
}

void libqu_graphics_destroy_texture(struct libqu_texture *texture)
{

}

void libqu_graphics_draw_texture(struct libqu_texture *texture, qu_rectf rect)
{

}

void libqu_graphics_draw_subtexture(struct libqu_texture *texture, qu_rectf rect, qu_rectf sub)
{

}
