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

#ifndef LIBQU_GRAPHICS_GL3_H_INC
#define LIBQU_GRAPHICS_GL3_H_INC

//------------------------------------------------------------------------------

#include <GL/gl.h>
#include "graphics.h"

//------------------------------------------------------------------------------
// Legacy OpenGL function typedefs

//----------------------------------------------------------
// OpenGL 1.0

// glBlendFunc
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);

// glClearColor
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

// glClear
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);

// glCopyTexImage2D
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

// glEnable
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);

// glGetError
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);

// glReadPixels
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);

// glTexImage2D
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);

// glTexParameteri
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);

// glViewport
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);

//----------------------------------------------------------
// OpenGL 1.1

// glBindTexture
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);

// glGenTextures
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);

// glDeleteTextures
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);

// glDrawArrays
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);

//------------------------------------------------------------------------------
// Declare struct that holds procedures

#define glAttachShader              priv.gl3._glAttachShader
#define glBindAttribLocation        priv.gl3._glBindAttribLocation
#define glBindBuffer                priv.gl3._glBindBuffer
#define glBindTexture               priv.gl3._glBindTexture
#define glBindVertexArray           priv.gl3._glBindVertexArray
#define glBlendFunc                 priv.gl3._glBlendFunc
#define glBufferData                priv.gl3._glBufferData
#define glBufferSubData             priv.gl3._glBufferSubData
#define glClear                     priv.gl3._glClear
#define glClearColor                priv.gl3._glClearColor
#define glCompileShader             priv.gl3._glCompileShader
#define glCreateProgram             priv.gl3._glCreateProgram
#define glCreateShader              priv.gl3._glCreateShader
#define glDeleteBuffers             priv.gl3._glDeleteBuffers
#define glDeleteProgram             priv.gl3._glDeleteProgram
#define glDeleteShader              priv.gl3._glDeleteShader
#define glDeleteTextures            priv.gl3._glDeleteTextures
#define glDeleteVertexArrays        priv.gl3._glDeleteVertexArrays
#define glDisableVertexAttribArray  priv.gl3._glDisableVertexAttribArray
#define glDrawArrays                priv.gl3._glDrawArrays
#define glEnable                    priv.gl3._glEnable
#define glEnableVertexAttribArray   priv.gl3._glEnableVertexAttribArray
#define glGenBuffers                priv.gl3._glGenBuffers
#define glGenTextures               priv.gl3._glGenTextures
#define glGenVertexArrays           priv.gl3._glGenVertexArrays
#define glGetError                  priv.gl3._glGetError
#define glGetProgramInfoLog         priv.gl3._glGetProgramInfoLog
#define glGetProgramiv              priv.gl3._glGetProgramiv
#define glGetShaderInfoLog          priv.gl3._glGetShaderInfoLog
#define glGetShaderiv               priv.gl3._glGetShaderiv
#define glGetUniformLocation        priv.gl3._glGetUniformLocation
#define glLinkProgram               priv.gl3._glLinkProgram
#define glReadPixels                priv.gl3._glReadPixels
#define glShaderSource              priv.gl3._glShaderSource
#define glTexImage2D                priv.gl3._glTexImage2D
#define glTexParameteri             priv.gl3._glTexParameteri
#define glUniform4fv                priv.gl3._glUniform4fv
#define glUniformMatrix4fv          priv.gl3._glUniformMatrix4fv
#define glUseProgram                priv.gl3._glUseProgram
#define glVertexAttribPointer       priv.gl3._glVertexAttribPointer
#define glViewport                  priv.gl3._glViewport

//------------------------------------------------------------------------------

#endif // LIBQU_GRAPHICS_GL3_H_INC
