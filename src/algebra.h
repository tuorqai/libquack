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

#ifndef LIBQU_ALGEBRA_H_INC
#define LIBQU_ALGEBRA_H_INC

//------------------------------------------------------------------------------

#include <math.h>
#include "libquack.h"

//------------------------------------------------------------------------------

typedef struct mat4_s
{
    float m[16];
} mat4_t;

//------------------------------------------------------------------------------

void mat4_identity(mat4_t *mat);
void mat4_copy(mat4_t *dst, mat4_t const *src);
void mat4_multiply(mat4_t *a, mat4_t const *b);
void mat4_ortho(mat4_t *mat, float l, float r, float b, float t);
void mat4_translate(mat4_t *mat, float x, float y, float z);
void mat4_scale(mat4_t *mat, float x, float y, float z);
void mat4_rotate(mat4_t *mat, float rad, float x, float y, float z);
void mat4_inverse(mat4_t *dst, mat4_t const *src);
qu_vec2f mat4_transform_point(mat4_t const *mat, qu_vec2f p);

//------------------------------------------------------------------------------

#endif // LIBQU_ALGEBRA_H_INC

