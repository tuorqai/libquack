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

#ifndef LIBQU_H_INC
#define LIBQU_H_INC

//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

#ifdef QU_SHARED
    #ifdef _WIN32
        #ifdef QU_BUILD
            #define QU_API __declspec(dllexport)
        #else
            #define QU_API __declspec(dllimport)
        #endif
        #define QU_CALL __cdecl
    #else
        #ifdef __GNUC__
            #define QU_API __attribute__((visibility("default")))
        #endif
    #endif
#endif

#ifndef QU_API
    #define QU_API
#endif

#ifndef QU_CALL
    #define QU_CALL
#endif

//------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {
#endif

//------------------------------------------------------------------------------

#define QU_COLOR(r, g, b, a)        ((r) << 24 | (g) << 16 | (b) << 8 | (a))

#define QU_EXTRACT_RED(color)       (((color) >> 24) & 255)
#define QU_EXTRACT_GREEN(color)     (((color) >> 16) & 255)
#define QU_EXTRACT_BLUE(color)      (((color) >> 8) & 255)
#define QU_EXTRACT_ALPHA(color)     ((color) & 255)

//------------------------------------------------------------------------------

typedef uint32_t qu_color;

typedef struct qu_vec2f
{
    float x;
    float y;
} qu_vec2f;

//------------------------------------------------------------------------------

QU_API void QU_CALL qu_initialize(void);
QU_API void QU_CALL qu_terminate(void);
QU_API bool QU_CALL qu_process(void);
QU_API void QU_CALL qu_present(void);

QU_API void QU_CALL qu_clear(qu_color color);
QU_API void QU_CALL qu_draw_point(float x, float y, qu_color color);
QU_API void QU_CALL qu_draw_line(float ax, float ay, float bx, float by, qu_color color);
QU_API void QU_CALL qu_draw_triangle(float ax, float ay, float bx, float by, float cx, float cy, qu_color outline, qu_color fill);
QU_API void QU_CALL qu_draw_rectangle(float x, float y, float w, float h, qu_color outline, qu_color fill);

//------------------------------------------------------------------------------

#if defined(__cplusplus)
} // extern "C"
#endif

//------------------------------------------------------------------------------

#endif // LIBQU_H_INC

