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

#ifndef LIBQU_IMAGE_H_INC
#define LIBQU_IMAGE_H_INC

//------------------------------------------------------------------------------

#include "libquack.h"

//------------------------------------------------------------------------------

/**
 * Image object.
 */
typedef struct libqu_image
{
    qu_pixel_format format;     // pixel format
    qu_vec2i size;              // size in pixels
    unsigned char *pixels;      // pixel array, no stride
} libqu_image;

//------------------------------------------------------------------------------

libqu_image *libqu_image_create(qu_pixel_format format, qu_vec2i size);
libqu_image *libqu_image_copy_flipped(libqu_image *image);
libqu_image *libqu_image_load(struct libqu_file *file);
void libqu_image_destroy(libqu_image *image);
void libqu_image_flip(libqu_image *image);

//------------------------------------------------------------------------------

#endif // LIBQU_IMAGE_H_INC
