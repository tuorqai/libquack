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

#ifdef QU_USE_OPENGL

//------------------------------------------------------------------------------

#include <GL/gl.h>
#include <GL/glext.h>
#include "graphics.h"
#include "log.h"

//------------------------------------------------------------------------------

#ifdef NDEBUG
    #define _GL(call) call
#else
    #define _GL(call) \
        do { \
            call; \
            _gl(#call, __FILE__, __LINE__); \
        } while (0);

    static char const *_glerrstr(GLenum error)
    {
        switch (error) {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return NULL;
        }
    }

    static void _gl(char const *call, char const *file, int line)
    {
        GLenum error = glGetError();

        if (error == GL_NO_ERROR) {
            return;
        }

        LIBQU_LOGE("OpenGL error(s) occurred in %s:\n", file);
        LIBQU_LOGE("%4d: %s\n", line, call);

        do {
            char const *str = _glerrstr(error);
            if (str) {
                LIBQU_LOGE("-- %s\n", str);
            } else {
                LIBQU_LOGE("-- 0x%04x\n", error);
            }
        } while ((error = glGetError()) != GL_NO_ERROR);
    }
#endif

//------------------------------------------------------------------------------

static void unpack_color(qu_color color, GLfloat *out)
{
    out[0] = QU_EXTRACT_RED(color) / 255.f;
    out[1] = QU_EXTRACT_GREEN(color) / 255.f;
    out[2] = QU_EXTRACT_BLUE(color) / 255.f;
    out[3] = QU_EXTRACT_ALPHA(color) / 255.f;
}

//------------------------------------------------------------------------------

static bool graphics_gl3_check_if_available(void)
{
    return true;
}

static bool graphics_gl3_initialize(struct libqu_graphics_params const *params)
{
    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void graphics_gl3_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

static void graphics_gl3_upload_vertices(struct libqu_vertex *vertices, size_t count)
{
}

static void graphics_gl3_clear(qu_color color)
{
    GLfloat c[4];
    unpack_color(color, c);

    _GL(glClearColor(c[0], c[1], c[2], c[3]));
    _GL(glClear(GL_COLOR_BUFFER_BIT));
}

static void graphics_gl3_draw(enum libqu_draw_mode mode, size_t vertex, size_t count)
{
}

//------------------------------------------------------------------------------

struct libqu_graphics_impl const libqu_graphics_gl3_impl = {
    graphics_gl3_check_if_available,
    graphics_gl3_initialize,
    graphics_gl3_terminate,
    graphics_gl3_upload_vertices,
    graphics_gl3_clear,
    graphics_gl3_draw,
};

//------------------------------------------------------------------------------

#endif // QU_USE_OPENGL

