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

#include <string.h>
#include "stb_image.h"

#include "fs.h"
#include "platform.h"
#include "qu_image.h"

//------------------------------------------------------------------------------
// custom file-reading callbacks for stb_image

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
// these functions don't make much sense rn

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

//------------------------------------------------------------------------------

libqu_image *libqu_image_create(qu_pixel_format format, qu_vec2i size)
{
    libqu_image *image = pl_calloc(1, sizeof(*image));

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

libqu_image *libqu_image_copy_flipped(libqu_image *image)
{
    libqu_image *copy = libqu_image_create(image->format, image->size);

    if (!copy) {
        return NULL;
    }

    int w = image->size.x;
    int h = image->size.y;
    int c = pixfmt_to_channels(image->format);

    for (int y = 0; y < image->size.y; y++) {
        void *src = &image->pixels[w * c * y];
        void *dst = &copy->pixels[w * c * (h - y - 1)];

        memcpy(dst, src, w * c);
    }

    return copy;
}

libqu_image *libqu_image_load(struct libqu_file *file)
{
    int w, h, c;

    unsigned char *data =
        stbi_load_from_callbacks(&_stbi_io, file, &w, &h, &c, 0);
    
    libqu_image *image = NULL;

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

void libqu_image_destroy(libqu_image *image)
{
    pl_free(image->pixels);
    pl_free(image);
}

void libqu_image_flip(libqu_image *image)
{
    int w = image->size.x;
    int h = image->size.y;
    int c = pixfmt_to_channels(image->format);

    unsigned char *tmp = pl_malloc(w * c);

    if (!tmp) {
        return;
    }

    for (int y = 0; y < h / 2; y++) {
        void *top = &image->pixels[w * c * y];
        void *bottom = &image->pixels[w * c * (h - y - 1)];

        memcpy(tmp, top, w * c);
        memcpy(top, bottom, w * c);
        memcpy(bottom, tmp, w * c);
    }

    pl_free(tmp);
}
