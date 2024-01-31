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
#include <stb_ds.h>
#include "algebra.h"
#include "core.h"
#include "graphics.h"
#include "log.h"
#include "platform.h"

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

enum
{
    SHADER_VERT_GENERIC,
    SHADER_FRAG_PRIMITIVE,
    SHADER_FRAG_TEXTURED,
    TOTAL_SHADERS,
};

enum
{
    PROGRAM_PRIMITIVE,
    PROGRAM_TEXTURED,
    TOTAL_PROGRAMS,
};

enum
{
    UNIFORM_PROJECTION,
    UNIFORM_MODELVIEW,
    TOTAL_UNIFORMS,
};

struct shader_info
{
    char const *src;
    GLenum type;
};

struct program_info
{
    int vsh;
    int fsh;
};

struct shader
{
    GLuint id;
};

struct program
{
    GLuint id;
    GLint uniloc[TOTAL_UNIFORMS];
    unsigned int dirty;
};

//------------------------------------------------------------------------------

static GLenum const mode_map[LIBQU_TOTAL_DRAW_MODES] = {
    GL_POINTS,
    GL_LINES,
    GL_LINE_LOOP,
    GL_LINE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
};

static struct shader_info const shader_info[TOTAL_SHADERS] = {
    {
        "#version 330 core\n"
        "in vec2 a_position;\n"
        "in vec4 a_color;\n"
        "in vec2 a_texCoord;\n"
        "out vec4 v_color;\n"
        "out vec2 v_texCoord;\n"
        "uniform mat4 u_projection;\n"
        "uniform mat4 u_modelView;\n"
        "void main()\n"
        "{\n"
        "    v_texCoord = a_texCoord;\n"
        "    v_color = a_color;\n"
        "    vec4 position = vec4(a_position, 0.0, 1.0);\n"
        "    gl_Position = u_projection * u_modelView * position;\n"
        "}\n",
        GL_VERTEX_SHADER,
    },
    {
        "#version 330 core\n"
        "in vec4 v_color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = v_color;\n"
        "}\n",
        GL_FRAGMENT_SHADER,
    },
    {
        "#version 330 core\n"
        "in vec4 v_color;\n"
        "in vec2 v_texCoord;\n"
        "uniform sampler2D u_texture;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(u_texture, v_texCoord) * v_color;\n"
        "}\n",
        GL_FRAGMENT_SHADER,
    },
};

static struct program_info const program_info[TOTAL_PROGRAMS] = {
    { SHADER_VERT_GENERIC, SHADER_FRAG_PRIMITIVE },
    { SHADER_VERT_GENERIC, SHADER_FRAG_TEXTURED },
};

//------------------------------------------------------------------------------

static struct
{
    GLfloat *vertbuf;
    GLuint vao;
    GLuint vbo;

    struct shader shaders[TOTAL_SHADERS];
    struct program programs[TOTAL_PROGRAMS];

    mat4_t projection;
    mat4_t modelview;

    int current_program;
    struct libqu_texture *current_texture;

    struct {
        PFNGLATTACHSHADERPROC glAttachShader;
        PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
        PFNGLCOMPILESHADERPROC glCompileShader;
        PFNGLCREATEPROGRAMPROC glCreateProgram;
        PFNGLCREATESHADERPROC glCreateShader;
        PFNGLDELETEPROGRAMPROC glDeleteProgram;
        PFNGLDELETESHADERPROC glDeleteShader;
        PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
        PFNGLGETPROGRAMIVPROC glGetProgramiv;
        PFNGLGETSHADERIVPROC glGetShaderiv;
        PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
        PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
        PFNGLLINKPROGRAMPROC glLinkProgram;
        PFNGLSHADERSOURCEPROC glShaderSource;
        PFNGLUNIFORM4FVPROC glUniform4fv;
        PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
        PFNGLUSEPROGRAMPROC glUseProgram;

        PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
        PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
        PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

        PFNGLBINDBUFFERPROC glBindBuffer;
        PFNGLBUFFERDATAPROC glBufferData;
        PFNGLBUFFERSUBDATAPROC glBufferSubData;
        PFNGLDELETEBUFFERSPROC glDeleteBuffers;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
        PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
        PFNGLGENBUFFERSPROC glGenBuffers;
        PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    } ext;
} priv;

//------------------------------------------------------------------------------

static void unpack_color(qu_color color, GLfloat *out)
{
    out[0] = QU_EXTRACT_RED(color) / 255.f;
    out[1] = QU_EXTRACT_GREEN(color) / 255.f;
    out[2] = QU_EXTRACT_BLUE(color) / 255.f;
    out[3] = QU_EXTRACT_ALPHA(color) / 255.f;
}

static int choose_texture_format(struct libqu_texture *texture,
    GLenum *iformat, GLenum *format)
{
    switch (texture->image->format) {
    case QU_PIXFMT_Y8:
        *iformat = GL_R8;
        *format = GL_RED;
        return 0;
    case QU_PIXFMT_Y8A8:
        *iformat = GL_RG8;
        *format = GL_RG;
        return 0;
    case QU_PIXFMT_R8G8B8:
        *iformat = GL_RGB8;
        *format = GL_RGB;
        return 0;
    case QU_PIXFMT_R8G8B8A8:
        *iformat = GL_RGBA8;
        *format = GL_RGBA;
        return 0;
    default:
        return -1;
    }
}

static void set_texture_parameters(unsigned int flags)
{
    GLenum mag_filter = (flags & QU_TEXTURE_SMOOTH) ? GL_LINEAR : GL_NEAREST;
    GLenum min_filter = GL_LINEAR;

    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));

    GLenum wrap_s = (flags & QU_TEXTURE_REPEAT) ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    GLenum wrap_t = wrap_s;

    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
}

static void get_texture_swizzle(struct libqu_texture *texture, GLenum *swizzle)
{
    switch (texture->image->format) {
    case QU_PIXFMT_Y8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_ONE;
        break;
    case QU_PIXFMT_Y8A8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_RED;
        swizzle[2] = GL_RED;
        swizzle[3] = GL_GREEN;
        break;
    case QU_PIXFMT_R8G8B8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_GREEN;
        swizzle[2] = GL_BLUE;
        swizzle[3] = GL_ONE;
        break;
    case QU_PIXFMT_R8G8B8A8:
        swizzle[0] = GL_RED;
        swizzle[1] = GL_GREEN;
        swizzle[2] = GL_BLUE;
        swizzle[3] = GL_ALPHA;
        break;
    default:
        break;
    }
}

static void load_gl_functions(void)
{
    priv.ext.glAttachShader = libqu_gl_get_proc_address("glAttachShader");
    priv.ext.glBindAttribLocation = libqu_gl_get_proc_address("glBindAttribLocation");
    priv.ext.glCompileShader = libqu_gl_get_proc_address("glCompileShader");
    priv.ext.glCreateProgram = libqu_gl_get_proc_address("glCreateProgram");
    priv.ext.glCreateShader = libqu_gl_get_proc_address("glCreateShader");
    priv.ext.glDeleteProgram = libqu_gl_get_proc_address("glDeleteProgram");
    priv.ext.glDeleteShader = libqu_gl_get_proc_address("glDeleteShader");
    priv.ext.glGetProgramInfoLog = libqu_gl_get_proc_address("glGetProgramInfoLog");
    priv.ext.glGetProgramiv = libqu_gl_get_proc_address("glGetProgramiv");
    priv.ext.glGetShaderiv = libqu_gl_get_proc_address("glGetShaderiv");
    priv.ext.glGetUniformLocation = libqu_gl_get_proc_address("glGetUniformLocation");
    priv.ext.glGetShaderInfoLog = libqu_gl_get_proc_address("glGetShaderInfoLog");
    priv.ext.glLinkProgram = libqu_gl_get_proc_address("glLinkProgram");
    priv.ext.glShaderSource = libqu_gl_get_proc_address("glShaderSource");
    priv.ext.glUniform4fv = libqu_gl_get_proc_address("glUniform4fv");
    priv.ext.glUniformMatrix4fv = libqu_gl_get_proc_address("glUniformMatrix4fv");
    priv.ext.glUseProgram = libqu_gl_get_proc_address("glUseProgram");

    priv.ext.glBindVertexArray = libqu_gl_get_proc_address("glBindVertexArray");
    priv.ext.glDeleteVertexArrays = libqu_gl_get_proc_address("glDeleteVertexArrays");
    priv.ext.glGenVertexArrays = libqu_gl_get_proc_address("glGenVertexArrays");

    priv.ext.glBindBuffer = libqu_gl_get_proc_address("glBindBuffer");
    priv.ext.glBufferData = libqu_gl_get_proc_address("glBufferData");
    priv.ext.glBufferSubData = libqu_gl_get_proc_address("glBufferSubData");
    priv.ext.glDeleteBuffers = libqu_gl_get_proc_address("glDeleteBuffers");
    priv.ext.glDisableVertexAttribArray = libqu_gl_get_proc_address("glDisableVertexAttribArray");
    priv.ext.glEnableVertexAttribArray = libqu_gl_get_proc_address("glEnableVertexAttribArray");
    priv.ext.glGenBuffers = libqu_gl_get_proc_address("glGenBuffers");
    priv.ext.glVertexAttribPointer = libqu_gl_get_proc_address("glVertexAttribPointer");
}

static GLuint compile_shader(char const *src, GLenum type)
{
    GLuint id = priv.ext.glCreateShader(type);

    _GL(priv.ext.glShaderSource(id, 1, &src, NULL));
    _GL(priv.ext.glCompileShader(id));

    GLint success;
    _GL(priv.ext.glGetShaderiv(id, GL_COMPILE_STATUS, &success));

    if (!success) {
        return 0;
    }

    return id;
}

static GLuint link_program(GLuint vsh, GLuint fsh)
{
    GLuint id = priv.ext.glCreateProgram();

    _GL(priv.ext.glAttachShader(id, vsh));
    _GL(priv.ext.glAttachShader(id, fsh));

    _GL(priv.ext.glBindAttribLocation(id, 0, "a_position"));
    _GL(priv.ext.glBindAttribLocation(id, 1, "a_color"));
    _GL(priv.ext.glBindAttribLocation(id, 2, "a_texCoord"));

    _GL(priv.ext.glLinkProgram(id));

    GLint success;
    _GL(priv.ext.glGetProgramiv(id, GL_LINK_STATUS, &success));

    if (!success) {
        return 0;
    }

    return id;
}

static bool load_shaders(void)
{
    for (int i = 0; i < TOTAL_SHADERS; i++) {
        char const *src = shader_info[i].src;
        GLenum type = shader_info[i].type;

        priv.shaders[i].id = compile_shader(src, type);

        if (!priv.shaders[i].id) {
            return false;
        }
    }

    return true;
}

static bool load_programs(void)
{
    for (int i = 0; i < TOTAL_PROGRAMS; i++) {
        GLuint vsh = priv.shaders[program_info[i].vsh].id;
        GLuint fsh = priv.shaders[program_info[i].fsh].id;

        priv.programs[i].id = link_program(vsh, fsh);

        if (!priv.programs[i].id) {
            return false;
        }

        priv.programs[i].uniloc[UNIFORM_PROJECTION] =
            priv.ext.glGetUniformLocation(priv.programs[i].id, "u_projection");

        priv.programs[i].uniloc[UNIFORM_MODELVIEW] =
            priv.ext.glGetUniformLocation(priv.programs[i].id, "u_modelView");
        
        priv.programs[i].dirty = 0xFFFFFFFF;
    }

    return true;
}

static void apply_program(int program)
{
    if (priv.current_program == program) {
        return;
    }

    priv.current_program = program;

    _GL(priv.ext.glUseProgram(priv.programs[program].id));

    if (priv.programs[program].dirty & (1 << UNIFORM_PROJECTION)) {
        _GL(priv.ext.glUniformMatrix4fv(
            priv.programs[program].uniloc[UNIFORM_PROJECTION],
            1, GL_FALSE, priv.projection.m
        ));
    }

    if (priv.programs[program].dirty & (1 << UNIFORM_MODELVIEW)) {
        _GL(priv.ext.glUniformMatrix4fv(
            priv.programs[program].uniloc[UNIFORM_MODELVIEW],
            1, GL_FALSE, priv.modelview.m
        ));
    }

    priv.programs[program].dirty = 0;
}

static void apply_texture(struct libqu_texture *texture)
{
    if (priv.current_texture == texture) {
        return;
    }

    priv.current_texture = texture;

    if (priv.current_texture) {
        _GL(glBindTexture(GL_TEXTURE_2D, texture->priv[0]));

        if (priv.current_program != PROGRAM_TEXTURED) {
            apply_program(PROGRAM_TEXTURED);
        }
    } else {
        _GL(glBindTexture(GL_TEXTURE_2D, 0));

        if (priv.current_program != PROGRAM_PRIMITIVE) {
            apply_program(PROGRAM_PRIMITIVE);
        }
    }
}

static void push_vertex(size_t index, struct libqu_vertex const *vertex)
{
    GLfloat *d = &priv.vertbuf[8 * index];

    *d++ = vertex->pos.x;
    *d++ = vertex->pos.y;

    *d++ = QU_EXTRACT_RED(vertex->color) / 255.f;
    *d++ = QU_EXTRACT_GREEN(vertex->color) / 255.f;
    *d++ = QU_EXTRACT_BLUE(vertex->color) / 255.f;
    *d++ = QU_EXTRACT_ALPHA(vertex->color) / 255.f;

    *d++ = vertex->texcoord.x;
    *d++ = 1.f - vertex->texcoord.y;
}

//------------------------------------------------------------------------------

static bool graphics_gl3_check_if_available(void)
{
    return libqu_gl_get_version() >= 330;
}

static bool graphics_gl3_initialize(struct libqu_graphics_params const *params)
{
    load_gl_functions();

    if (!load_shaders()) {
        LIBQU_LOGE("Failed to compile GLSL shaders.\n");
        return false;
    }

    if (!load_programs()) {
        LIBQU_LOGE("Failed to build GLSL programs.\n");
        return false;
    }

    priv.current_program = -1;
    priv.current_texture = NULL;

    _GL(priv.ext.glGenVertexArrays(1, &priv.vao));
    _GL(priv.ext.glGenBuffers(1, &priv.vbo));

    _GL(priv.ext.glBindVertexArray(priv.vao));
    _GL(priv.ext.glEnableVertexAttribArray(0));
    _GL(priv.ext.glEnableVertexAttribArray(1));
    _GL(priv.ext.glEnableVertexAttribArray(2));

    int width = params->window_size.x;
    int height = params->window_size.y;

    _GL(glViewport(0, 0, width, height));

    mat4_ortho(&priv.projection, 0.f, width, height, 0.f);
    mat4_identity(&priv.modelview);

    _GL(glEnable(GL_BLEND));
    _GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void graphics_gl3_terminate(void)
{
    arrfree(priv.vertbuf);

    LIBQU_LOGI("Terminated.\n");
}

static void graphics_gl3_upload_vertices(struct libqu_vertex *vertices, size_t count)
{
    arrsetlen(priv.vertbuf, 8 * count);

    for (size_t v = 0; v < count; v++) {
        push_vertex(v, &vertices[v]);
    }

    _GL(priv.ext.glBindBuffer(GL_ARRAY_BUFFER, priv.vbo));
    _GL(priv.ext.glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * arrlen(priv.vertbuf), priv.vertbuf, GL_STREAM_DRAW));

    _GL(priv.ext.glBindVertexArray(priv.vao));
    _GL(priv.ext.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) 0));
    _GL(priv.ext.glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (sizeof(GLfloat) * 2)));
    _GL(priv.ext.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) (sizeof(GLfloat) * 6)));
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
    _GL(glDrawArrays(mode_map[mode], (GLint) vertex, (GLsizei) count));
}

static int graphics_gl3_load_texture(struct libqu_texture *texture)
{
    GLenum iformat, format;

    if (choose_texture_format(texture, &iformat, &format) == -1) {
        return -1;
    }

    GLuint id;
    _GL(glGenTextures(1, &id));

    if (id == 0) {
        return -1;
    }

    priv.current_texture = texture;
    _GL(glBindTexture(GL_TEXTURE_2D, id));

    struct libqu_image *flipped = libqu_image_copy_flipped(texture->image);

    _GL(glTexImage2D(GL_TEXTURE_2D,
        0, iformat,
        texture->image->size.x,
        texture->image->size.y,
        0, format,
        GL_UNSIGNED_BYTE, flipped->pixels));
    
    libqu_image_destroy(flipped);

    set_texture_parameters(texture->flags);

    GLenum swizzle[4];
    get_texture_swizzle(texture, swizzle);

    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, swizzle[0]));
    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, swizzle[1]));
    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, swizzle[2]));
    _GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, swizzle[3]));

    texture->priv[0] = (uintptr_t) id;

    return 0;
}

static void graphics_gl3_destroy_texture(struct libqu_texture *texture)
{
    if (priv.current_texture == texture) {
        apply_texture(NULL);
    }

    GLuint id = (GLuint) texture->priv[0];
    _GL(glDeleteTextures(1, &id));
}

static void graphics_gl3_update_texture_flags(struct libqu_texture *texture)
{
    apply_texture(texture);
    set_texture_parameters(texture->flags);
}

static void graphics_gl3_apply_texture(struct libqu_texture *texture)
{
    apply_texture(texture);
}

static int graphics_gl3_capture_screen(struct libqu_image *image)
{
    _GL(glReadPixels(0, 0, image->size.x, image->size.y,
        GL_RGB, GL_UNSIGNED_BYTE, image->pixels));

    libqu_image_flip(image);

    return 0;
}

//------------------------------------------------------------------------------

struct libqu_graphics_impl const libqu_graphics_gl3_impl = {
    graphics_gl3_check_if_available,
    graphics_gl3_initialize,
    graphics_gl3_terminate,
    graphics_gl3_upload_vertices,
    graphics_gl3_clear,
    graphics_gl3_draw,
    graphics_gl3_load_texture,
    graphics_gl3_destroy_texture,
    graphics_gl3_update_texture_flags,
    graphics_gl3_apply_texture,
    graphics_gl3_capture_screen,
};

//------------------------------------------------------------------------------

#endif // QU_USE_OPENGL

