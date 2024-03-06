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

#ifndef LIBQU_DYN_GL3_H_INC
#define LIBQU_DYN_GL3_H_INC

//------------------------------------------------------------------------------

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include "core.h"
#include "log.h"

//------------------------------------------------------------------------------

typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
typedef void (APIENTRYP PFNGLHINTPROC)(GLenum target, GLenum mode);
typedef void (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
typedef void (APIENTRYP PFNGLPOINTSIZEPROC)(GLfloat size);
typedef void (APIENTRYP PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRYP PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
typedef void (APIENTRYP PFNGLCLEARDEPTHPROC)(GLdouble depth);
typedef void (APIENTRYP PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLFINISHPROC)(void);
typedef void (APIENTRYP PFNGLFLUSHPROC)(void);
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRYP PFNGLLOGICOPPROC)(GLenum opcode);
typedef void (APIENTRYP PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRYP PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (APIENTRYP PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (APIENTRYP PFNGLREADBUFFERPROC)(GLenum src);
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void (APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
typedef void (APIENTRYP PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble *data);
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
typedef void (APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
typedef void (APIENTRYP PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint *params);
typedef GLboolean (APIENTRYP PFNGLISENABLEDPROC)(GLenum cap);
typedef void (APIENTRYP PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef GLboolean (APIENTRYP PFNGLISTEXTUREPROC)(GLuint texture);

//------------------------------------------------------------------------------

#define PROC_LIST \
    PROC(PFNGLCULLFACEPROC,             glCullFace)                     \
    PROC(PFNGLFRONTFACEPROC,            glFrontFace)                    \
    PROC(PFNGLHINTPROC,                 glHint)                         \
    PROC(PFNGLLINEWIDTHPROC,            glLineWidth)                    \
    PROC(PFNGLPOINTSIZEPROC,            glPointSize)                    \
    PROC(PFNGLPOLYGONMODEPROC,          glPolygonMode)                  \
    PROC(PFNGLSCISSORPROC,              glScissor)                      \
    PROC(PFNGLTEXPARAMETERFPROC,        glTexParameterf)                \
    PROC(PFNGLTEXPARAMETERFVPROC,       glTexParameterfv)               \
    PROC(PFNGLTEXPARAMETERIPROC,        glTexParameteri)                \
    PROC(PFNGLTEXPARAMETERIVPROC,       glTexParameteriv)               \
    PROC(PFNGLTEXIMAGE1DPROC,           glTexImage1D)                   \
    PROC(PFNGLTEXIMAGE2DPROC,           glTexImage2D)                   \
    PROC(PFNGLDRAWBUFFERPROC,           glDrawBuffer)                   \
    PROC(PFNGLCLEARPROC,                glClear)                        \
    PROC(PFNGLCLEARCOLORPROC,           glClearColor)                   \
    PROC(PFNGLCLEARSTENCILPROC,         glClearStencil)                 \
    PROC(PFNGLCLEARDEPTHPROC,           glClearDepth)                   \
    PROC(PFNGLSTENCILMASKPROC,          glStencilMask)                  \
    PROC(PFNGLCOLORMASKPROC,            glColorMask)                    \
    PROC(PFNGLDEPTHMASKPROC,            glDepthMask)                    \
    PROC(PFNGLDISABLEPROC,              glDisable)                      \
    PROC(PFNGLENABLEPROC,               glEnable)                       \
    PROC(PFNGLFINISHPROC,               glFinish)                       \
    PROC(PFNGLFLUSHPROC,                glFlush)                        \
    PROC(PFNGLBLENDFUNCPROC,            glBlendFunc)                    \
    PROC(PFNGLLOGICOPPROC,              glLogicOp)                      \
    PROC(PFNGLSTENCILFUNCPROC,          glStencilFunc)                  \
    PROC(PFNGLSTENCILOPPROC,            glStencilOp)                    \
    PROC(PFNGLDEPTHFUNCPROC,            glDepthFunc)                    \
    PROC(PFNGLPIXELSTOREFPROC,          glPixelStoref)                  \
    PROC(PFNGLPIXELSTOREIPROC,          glPixelStorei)                  \
    PROC(PFNGLREADBUFFERPROC,           glReadBuffer)                   \
    PROC(PFNGLREADPIXELSPROC,           glReadPixels)                   \
    PROC(PFNGLGETBOOLEANVPROC,          glGetBooleanv)                  \
    PROC(PFNGLGETDOUBLEVPROC,           glGetDoublev)                   \
    PROC(PFNGLGETERRORPROC,             glGetError)                     \
    PROC(PFNGLGETFLOATVPROC,            glGetFloatv)                    \
    PROC(PFNGLGETINTEGERVPROC,          glGetIntegerv)                  \
    PROC(PFNGLGETSTRINGPROC,            glGetString)                    \
    PROC(PFNGLGETTEXIMAGEPROC,          glGetTexImage)                  \
    PROC(PFNGLGETTEXPARAMETERFVPROC,    glGetTexParameterfv)            \
    PROC(PFNGLGETTEXPARAMETERIVPROC,    glGetTexParameteriv)            \
    PROC(PFNGLGETTEXLEVELPARAMETERFVPROC,                               \
                                        glGetTexLevelParameterfv)       \
    PROC(PFNGLGETTEXLEVELPARAMETERIVPROC,                               \
                                        glGetTexLevelParameteriv)       \
    PROC(PFNGLISENABLEDPROC,            glIsEnabled)                    \
    PROC(PFNGLDEPTHRANGEPROC,           glDepthRange)                   \
    PROC(PFNGLVIEWPORTPROC,             glViewport)                     \
    PROC(PFNGLDRAWARRAYSPROC,           glDrawArrays)                   \
    PROC(PFNGLDRAWELEMENTSPROC,         glDrawElements)                 \
    PROC(PFNGLPOLYGONOFFSETPROC,        glPolygonOffset)                \
    PROC(PFNGLCOPYTEXIMAGE1DPROC,       glCopyTexImage1D)               \
    PROC(PFNGLCOPYTEXIMAGE2DPROC,       glCopyTexImage2D)               \
    PROC(PFNGLCOPYTEXSUBIMAGE1DPROC,    glCopyTexSubImage1D)            \
    PROC(PFNGLCOPYTEXSUBIMAGE2DPROC,    glCopyTexSubImage2D)            \
    PROC(PFNGLTEXSUBIMAGE1DPROC,        glTexSubImage1D)                \
    PROC(PFNGLTEXSUBIMAGE2DPROC,        glTexSubImage2D)                \
    PROC(PFNGLBINDTEXTUREPROC,          glBindTexture)                  \
    PROC(PFNGLDELETETEXTURESPROC,       glDeleteTextures)               \
    PROC(PFNGLGENTEXTURESPROC,          glGenTextures)                  \
    PROC(PFNGLISTEXTUREPROC,            glIsTexture)                    \
    PROC(PFNGLDRAWRANGEELEMENTSPROC,    glDrawRangeElements)            \
    PROC(PFNGLTEXIMAGE3DPROC,           glTexImage3D)                   \
    PROC(PFNGLTEXSUBIMAGE3DPROC,        glTexSubImage3D)                \
    PROC(PFNGLCOPYTEXSUBIMAGE3DPROC,    glCopyTexSubImage3D)            \
    PROC(PFNGLACTIVETEXTUREPROC,        glActiveTexture)                \
    PROC(PFNGLSAMPLECOVERAGEPROC,       glSampleCoverage)               \
    PROC(PFNGLCOMPRESSEDTEXIMAGE3DPROC, glCompressedTexImage3D)         \
    PROC(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D)         \
    PROC(PFNGLCOMPRESSEDTEXIMAGE1DPROC, glCompressedTexImage1D)         \
    PROC(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC,                              \
                                        glCompressedTexSubImage3D)      \
    PROC(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC,                              \
                                        glCompressedTexSubImage2D)      \
    PROC(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC,                              \
                                        glCompressedTexSubImage1D)      \
    PROC(PFNGLGETCOMPRESSEDTEXIMAGEPROC,                                \
                                        glGetCompressedTexImage)        \
    PROC(PFNGLBLENDFUNCSEPARATEPROC,    glBlendFuncSeparate)            \
    PROC(PFNGLMULTIDRAWARRAYSPROC,      glMultiDrawArrays)              \
    PROC(PFNGLMULTIDRAWELEMENTSPROC,    glMultiDrawElements)            \
    PROC(PFNGLPOINTPARAMETERFPROC,      glPointParameterf)              \
    PROC(PFNGLPOINTPARAMETERFVPROC,     glPointParameterfv)             \
    PROC(PFNGLPOINTPARAMETERIPROC,      glPointParameteri)              \
    PROC(PFNGLPOINTPARAMETERIVPROC,     glPointParameteriv)             \
    PROC(PFNGLBLENDCOLORPROC,           glBlendColor)                   \
    PROC(PFNGLBLENDEQUATIONPROC,        glBlendEquation)                \
    PROC(PFNGLGENQUERIESPROC,           glGenQueries)                   \
    PROC(PFNGLDELETEQUERIESPROC,        glDeleteQueries)                \
    PROC(PFNGLISQUERYPROC,              glIsQuery)                      \
    PROC(PFNGLBEGINQUERYPROC,           glBeginQuery)                   \
    PROC(PFNGLENDQUERYPROC,             glEndQuery)                     \
    PROC(PFNGLGETQUERYIVPROC,           glGetQueryiv)                   \
    PROC(PFNGLGETQUERYOBJECTIVPROC,     glGetQueryObjectiv)             \
    PROC(PFNGLGETQUERYOBJECTUIVPROC,    glGetQueryObjectuiv)            \
    PROC(PFNGLBINDBUFFERPROC,           glBindBuffer)                   \
    PROC(PFNGLDELETEBUFFERSPROC,        glDeleteBuffers)                \
    PROC(PFNGLGENBUFFERSPROC,           glGenBuffers)                   \
    PROC(PFNGLISBUFFERPROC,             glIsBuffer)                     \
    PROC(PFNGLBUFFERDATAPROC,           glBufferData)                   \
    PROC(PFNGLBUFFERSUBDATAPROC,        glBufferSubData)                \
    PROC(PFNGLGETBUFFERSUBDATAPROC,     glGetBufferSubData)             \
    PROC(PFNGLMAPBUFFERPROC,            glMapBuffer)                    \
    PROC(PFNGLUNMAPBUFFERPROC,          glUnmapBuffer)                  \
    PROC(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv)         \
    PROC(PFNGLGETBUFFERPOINTERVPROC,    glGetBufferPointerv)            \
    PROC(PFNGLBLENDEQUATIONSEPARATEPROC,                                \
                                        glBlendEquationSeparate)        \
    PROC(PFNGLDRAWBUFFERSPROC,          glDrawBuffers)                  \
    PROC(PFNGLSTENCILOPSEPARATEPROC,    glStencilOpSeparate)            \
    PROC(PFNGLSTENCILFUNCSEPARATEPROC,  glStencilFuncSeparate)          \
    PROC(PFNGLSTENCILMASKSEPARATEPROC,  glStencilMaskSeparate)          \
    PROC(PFNGLATTACHSHADERPROC,         glAttachShader)                 \
    PROC(PFNGLBINDATTRIBLOCATIONPROC,   glBindAttribLocation)           \
    PROC(PFNGLCOMPILESHADERPROC,        glCompileShader)                \
    PROC(PFNGLCREATEPROGRAMPROC,        glCreateProgram)                \
    PROC(PFNGLCREATESHADERPROC,         glCreateShader)                 \
    PROC(PFNGLDELETEPROGRAMPROC,        glDeleteProgram)                \
    PROC(PFNGLDELETESHADERPROC,         glDeleteShader)                 \
    PROC(PFNGLDETACHSHADERPROC,         glDetachShader)                 \
    PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC,                             \
                                        glDisableVertexAttribArray)     \
    PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC,                              \
                                        glEnableVertexAttribArray)      \
    PROC(PFNGLGETACTIVEATTRIBPROC,      glGetActiveAttrib)              \
    PROC(PFNGLGETACTIVEUNIFORMPROC,     glGetActiveUniform)             \
    PROC(PFNGLGETATTACHEDSHADERSPROC,   glGetAttachedShaders)           \
    PROC(PFNGLGETATTRIBLOCATIONPROC,    glGetAttribLocation)            \
    PROC(PFNGLGETPROGRAMIVPROC,         glGetProgramiv)                 \
    PROC(PFNGLGETPROGRAMINFOLOGPROC,    glGetProgramInfoLog)            \
    PROC(PFNGLGETSHADERIVPROC,          glGetShaderiv)                  \
    PROC(PFNGLGETSHADERINFOLOGPROC,     glGetShaderInfoLog)             \
    PROC(PFNGLGETSHADERSOURCEPROC,      glGetShaderSource)              \
    PROC(PFNGLGETUNIFORMLOCATIONPROC,   glGetUniformLocation)           \
    PROC(PFNGLGETUNIFORMFVPROC,         glGetUniformfv)                 \
    PROC(PFNGLGETUNIFORMIVPROC,         glGetUniformiv)                 \
    PROC(PFNGLGETVERTEXATTRIBDVPROC,    glGetVertexAttribdv)            \
    PROC(PFNGLGETVERTEXATTRIBFVPROC,    glGetVertexAttribfv)            \
    PROC(PFNGLGETVERTEXATTRIBIVPROC,    glGetVertexAttribiv)            \
    PROC(PFNGLGETVERTEXATTRIBPOINTERVPROC,                              \
                                        glGetVertexAttribPointerv)      \
    PROC(PFNGLISPROGRAMPROC,            glIsProgram)                    \
    PROC(PFNGLISSHADERPROC,             glIsShader)                     \
    PROC(PFNGLLINKPROGRAMPROC,          glLinkProgram)                  \
    PROC(PFNGLSHADERSOURCEPROC,         glShaderSource)                 \
    PROC(PFNGLUSEPROGRAMPROC,           glUseProgram)                   \
    PROC(PFNGLUNIFORM1FPROC,            glUniform1f)                    \
    PROC(PFNGLUNIFORM2FPROC,            glUniform2f)                    \
    PROC(PFNGLUNIFORM3FPROC,            glUniform3f)                    \
    PROC(PFNGLUNIFORM4FPROC,            glUniform4f)                    \
    PROC(PFNGLUNIFORM1IPROC,            glUniform1i)                    \
    PROC(PFNGLUNIFORM2IPROC,            glUniform2i)                    \
    PROC(PFNGLUNIFORM3IPROC,            glUniform3i)                    \
    PROC(PFNGLUNIFORM4IPROC,            glUniform4i)                    \
    PROC(PFNGLUNIFORM1FVPROC,           glUniform1fv)                   \
    PROC(PFNGLUNIFORM2FVPROC,           glUniform2fv)                   \
    PROC(PFNGLUNIFORM3FVPROC,           glUniform3fv)                   \
    PROC(PFNGLUNIFORM4FVPROC,           glUniform4fv)                   \
    PROC(PFNGLUNIFORM1IVPROC,           glUniform1iv)                   \
    PROC(PFNGLUNIFORM2IVPROC,           glUniform2iv)                   \
    PROC(PFNGLUNIFORM3IVPROC,           glUniform3iv)                   \
    PROC(PFNGLUNIFORM4IVPROC,           glUniform4iv)                   \
    PROC(PFNGLUNIFORMMATRIX2FVPROC,     glUniformMatrix2fv)             \
    PROC(PFNGLUNIFORMMATRIX3FVPROC,     glUniformMatrix3fv)             \
    PROC(PFNGLUNIFORMMATRIX4FVPROC,     glUniformMatrix4fv)             \
    PROC(PFNGLVALIDATEPROGRAMPROC,      glValidateProgram)              \
    PROC(PFNGLVERTEXATTRIB1DPROC,       glVertexAttrib1d)               \
    PROC(PFNGLVERTEXATTRIB1DVPROC,      glVertexAttrib1dv)              \
    PROC(PFNGLVERTEXATTRIB1FPROC,       glVertexAttrib1f)               \
    PROC(PFNGLVERTEXATTRIB1FVPROC,      glVertexAttrib1fv)              \
    PROC(PFNGLVERTEXATTRIB1SPROC,       glVertexAttrib1s)               \
    PROC(PFNGLVERTEXATTRIB1SVPROC,      glVertexAttrib1sv)              \
    PROC(PFNGLVERTEXATTRIB2DPROC,       glVertexAttrib2d)               \
    PROC(PFNGLVERTEXATTRIB2DVPROC,      glVertexAttrib2dv)              \
    PROC(PFNGLVERTEXATTRIB2FPROC,       glVertexAttrib2f)               \
    PROC(PFNGLVERTEXATTRIB2FVPROC,      glVertexAttrib2fv)              \
    PROC(PFNGLVERTEXATTRIB2SPROC,       glVertexAttrib2s)               \
    PROC(PFNGLVERTEXATTRIB2SVPROC,      glVertexAttrib2sv)              \
    PROC(PFNGLVERTEXATTRIB3DPROC,       glVertexAttrib3d)               \
    PROC(PFNGLVERTEXATTRIB3DVPROC,      glVertexAttrib3dv)              \
    PROC(PFNGLVERTEXATTRIB3FPROC,       glVertexAttrib3f)               \
    PROC(PFNGLVERTEXATTRIB3FVPROC,      glVertexAttrib3fv)              \
    PROC(PFNGLVERTEXATTRIB3SPROC,       glVertexAttrib3s)               \
    PROC(PFNGLVERTEXATTRIB3SVPROC,      glVertexAttrib3sv)              \
    PROC(PFNGLVERTEXATTRIB4NBVPROC,     glVertexAttrib4Nbv)             \
    PROC(PFNGLVERTEXATTRIB4NIVPROC,     glVertexAttrib4Niv)             \
    PROC(PFNGLVERTEXATTRIB4NSVPROC,     glVertexAttrib4Nsv)             \
    PROC(PFNGLVERTEXATTRIB4NUBPROC,     glVertexAttrib4Nub)             \
    PROC(PFNGLVERTEXATTRIB4NUBVPROC,    glVertexAttrib4Nubv)            \
    PROC(PFNGLVERTEXATTRIB4NUIVPROC,    glVertexAttrib4Nuiv)            \
    PROC(PFNGLVERTEXATTRIB4NUSVPROC,    glVertexAttrib4Nusv)            \
    PROC(PFNGLVERTEXATTRIB4BVPROC,      glVertexAttrib4bv)              \
    PROC(PFNGLVERTEXATTRIB4DPROC,       glVertexAttrib4d)               \
    PROC(PFNGLVERTEXATTRIB4DVPROC,      glVertexAttrib4dv)              \
    PROC(PFNGLVERTEXATTRIB4FPROC,       glVertexAttrib4f)               \
    PROC(PFNGLVERTEXATTRIB4FVPROC,      glVertexAttrib4fv)              \
    PROC(PFNGLVERTEXATTRIB4IVPROC,      glVertexAttrib4iv)              \
    PROC(PFNGLVERTEXATTRIB4SPROC,       glVertexAttrib4s)               \
    PROC(PFNGLVERTEXATTRIB4SVPROC,      glVertexAttrib4sv)              \
    PROC(PFNGLVERTEXATTRIB4UBVPROC,     glVertexAttrib4ubv)             \
    PROC(PFNGLVERTEXATTRIB4UIVPROC,     glVertexAttrib4uiv)             \
    PROC(PFNGLVERTEXATTRIB4USVPROC,     glVertexAttrib4usv)             \
    PROC(PFNGLVERTEXATTRIBPOINTERPROC,  glVertexAttribPointer)          \
    PROC(PFNGLUNIFORMMATRIX2X3FVPROC,   glUniformMatrix2x3fv)           \
    PROC(PFNGLUNIFORMMATRIX3X2FVPROC,   glUniformMatrix3x2fv)           \
    PROC(PFNGLUNIFORMMATRIX2X4FVPROC,   glUniformMatrix2x4fv)           \
    PROC(PFNGLUNIFORMMATRIX4X2FVPROC,   glUniformMatrix4x2fv)           \
    PROC(PFNGLUNIFORMMATRIX3X4FVPROC,   glUniformMatrix3x4fv)           \
    PROC(PFNGLUNIFORMMATRIX4X3FVPROC,   glUniformMatrix4x3fv)           \
    PROC(PFNGLCOLORMASKIPROC,           glColorMaski)                   \
    PROC(PFNGLGETBOOLEANI_VPROC,        glGetBooleani_v)                \
    PROC(PFNGLGETINTEGERI_VPROC,        glGetIntegeri_v)                \
    PROC(PFNGLENABLEIPROC,              glEnablei)                      \
    PROC(PFNGLDISABLEIPROC,             glDisablei)                     \
    PROC(PFNGLISENABLEDIPROC,           glIsEnabledi)                   \
    PROC(PFNGLBEGINTRANSFORMFEEDBACKPROC,                               \
                                        glBeginTransformFeedback)       \
    PROC(PFNGLENDTRANSFORMFEEDBACKPROC, glEndTransformFeedback)         \
    PROC(PFNGLBINDBUFFERRANGEPROC,      glBindBufferRange)              \
    PROC(PFNGLBINDBUFFERBASEPROC,       glBindBufferBase)               \
    PROC(PFNGLTRANSFORMFEEDBACKVARYINGSPROC,                            \
                                        glTransformFeedbackVaryings)    \
    PROC(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC,                          \
                                        glGetTransformFeedbackVarying)  \
    PROC(PFNGLCLAMPCOLORPROC,           glClampColor)                   \
    PROC(PFNGLBEGINCONDITIONALRENDERPROC,                               \
                                        glBeginConditionalRender)       \
    PROC(PFNGLENDCONDITIONALRENDERPROC, glEndConditionalRender)         \
    PROC(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer)         \
    PROC(PFNGLGETVERTEXATTRIBIIVPROC,   glGetVertexAttribIiv)           \
    PROC(PFNGLGETVERTEXATTRIBIUIVPROC,  glGetVertexAttribIuiv)          \
    PROC(PFNGLVERTEXATTRIBI1IPROC,      glVertexAttribI1i)              \
    PROC(PFNGLVERTEXATTRIBI2IPROC,      glVertexAttribI2i)              \
    PROC(PFNGLVERTEXATTRIBI3IPROC,      glVertexAttribI3i)              \
    PROC(PFNGLVERTEXATTRIBI4IPROC,      glVertexAttribI4i)              \
    PROC(PFNGLVERTEXATTRIBI1UIPROC,     glVertexAttribI1ui)             \
    PROC(PFNGLVERTEXATTRIBI2UIPROC,     glVertexAttribI2ui)             \
    PROC(PFNGLVERTEXATTRIBI3UIPROC,     glVertexAttribI3ui)             \
    PROC(PFNGLVERTEXATTRIBI4UIPROC,     glVertexAttribI4ui)             \
    PROC(PFNGLVERTEXATTRIBI1IVPROC,     glVertexAttribI1iv)             \
    PROC(PFNGLVERTEXATTRIBI2IVPROC,     glVertexAttribI2iv)             \
    PROC(PFNGLVERTEXATTRIBI3IVPROC,     glVertexAttribI3iv)             \
    PROC(PFNGLVERTEXATTRIBI4IVPROC,     glVertexAttribI4iv)             \
    PROC(PFNGLVERTEXATTRIBI1UIVPROC,    glVertexAttribI1uiv)            \
    PROC(PFNGLVERTEXATTRIBI2UIVPROC,    glVertexAttribI2uiv)            \
    PROC(PFNGLVERTEXATTRIBI3UIVPROC,    glVertexAttribI3uiv)            \
    PROC(PFNGLVERTEXATTRIBI4UIVPROC,    glVertexAttribI4uiv)            \
    PROC(PFNGLVERTEXATTRIBI4BVPROC,     glVertexAttribI4bv)             \
    PROC(PFNGLVERTEXATTRIBI4SVPROC,     glVertexAttribI4sv)             \
    PROC(PFNGLVERTEXATTRIBI4UBVPROC,    glVertexAttribI4ubv)            \
    PROC(PFNGLVERTEXATTRIBI4USVPROC,    glVertexAttribI4usv)            \
    PROC(PFNGLGETUNIFORMUIVPROC,        glGetUniformuiv)                \
    PROC(PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation)         \
    PROC(PFNGLGETFRAGDATALOCATIONPROC,  glGetFragDataLocation)          \
    PROC(PFNGLUNIFORM1UIPROC,           glUniform1ui)                   \
    PROC(PFNGLUNIFORM2UIPROC,           glUniform2ui)                   \
    PROC(PFNGLUNIFORM3UIPROC,           glUniform3ui)                   \
    PROC(PFNGLUNIFORM4UIPROC,           glUniform4ui)                   \
    PROC(PFNGLUNIFORM1UIVPROC,          glUniform1uiv)                  \
    PROC(PFNGLUNIFORM2UIVPROC,          glUniform2uiv)                  \
    PROC(PFNGLUNIFORM3UIVPROC,          glUniform3uiv)                  \
    PROC(PFNGLUNIFORM4UIVPROC,          glUniform4uiv)                  \
    PROC(PFNGLTEXPARAMETERIIVPROC,      glTexParameterIiv)              \
    PROC(PFNGLTEXPARAMETERIUIVPROC,     glTexParameterIuiv)             \
    PROC(PFNGLGETTEXPARAMETERIIVPROC,   glGetTexParameterIiv)           \
    PROC(PFNGLGETTEXPARAMETERIUIVPROC,  glGetTexParameterIuiv)          \
    PROC(PFNGLCLEARBUFFERIVPROC,        glClearBufferiv)                \
    PROC(PFNGLCLEARBUFFERUIVPROC,       glClearBufferuiv)               \
    PROC(PFNGLCLEARBUFFERFVPROC,        glClearBufferfv)                \
    PROC(PFNGLCLEARBUFFERFIPROC,        glClearBufferfi)                \
    PROC(PFNGLGETSTRINGIPROC,           glGetStringi)                   \
    PROC(PFNGLISRENDERBUFFERPROC,       glIsRenderbuffer)               \
    PROC(PFNGLBINDRENDERBUFFERPROC,     glBindRenderbuffer)             \
    PROC(PFNGLDELETERENDERBUFFERSPROC,  glDeleteRenderbuffers)          \
    PROC(PFNGLGENRENDERBUFFERSPROC,     glGenRenderbuffers)             \
    PROC(PFNGLRENDERBUFFERSTORAGEPROC,  glRenderbufferStorage)          \
    PROC(PFNGLGETRENDERBUFFERPARAMETERIVPROC,                           \
                                        glGetRenderbufferParameteriv)   \
    PROC(PFNGLISFRAMEBUFFERPROC,        glIsFramebuffer)                \
    PROC(PFNGLBINDFRAMEBUFFERPROC,      glBindFramebuffer)              \
    PROC(PFNGLDELETEFRAMEBUFFERSPROC,   glDeleteFramebuffers)           \
    PROC(PFNGLGENFRAMEBUFFERSPROC,      glGenFramebuffers)              \
    PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC,                               \
                                        glCheckFramebufferStatus)       \
    PROC(PFNGLFRAMEBUFFERTEXTURE1DPROC, glFramebufferTexture1D)         \
    PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)         \
    PROC(PFNGLFRAMEBUFFERTEXTURE3DPROC, glFramebufferTexture3D)         \
    PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC,                              \
                                        glFramebufferRenderbuffer)      \
    PROC(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC,                  \
                                        glGetFramebufferAttachmentParameteriv) \
    PROC(PFNGLGENERATEMIPMAPPROC,       glGenerateMipmap)               \
    PROC(PFNGLBLITFRAMEBUFFERPROC,      glBlitFramebuffer)              \
    PROC(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC,                       \
                                        glRenderbufferStorageMultisample) \
    PROC(PFNGLFRAMEBUFFERTEXTURELAYERPROC,                              \
                                        glFramebufferTextureLayer)      \
    PROC(PFNGLMAPBUFFERRANGEPROC,       glMapBufferRange)               \
    PROC(PFNGLFLUSHMAPPEDBUFFERRANGEPROC,                               \
                                        glFlushMappedBufferRange)       \
    PROC(PFNGLBINDVERTEXARRAYPROC,      glBindVertexArray)              \
    PROC(PFNGLDELETEVERTEXARRAYSPROC,   glDeleteVertexArrays)           \
    PROC(PFNGLGENVERTEXARRAYSPROC,      glGenVertexArrays)              \
    PROC(PFNGLISVERTEXARRAYPROC,        glIsVertexArray)                \
    PROC(PFNGLDRAWARRAYSINSTANCEDPROC,  glDrawArraysInstanced)          \
    PROC(PFNGLDRAWELEMENTSINSTANCEDPROC,                                \
                                        glDrawElementsInstanced)        \
    PROC(PFNGLTEXBUFFERPROC,            glTexBuffer)                    \
    PROC(PFNGLPRIMITIVERESTARTINDEXPROC,                                \
                                        glPrimitiveRestartIndex)        \
    PROC(PFNGLCOPYBUFFERSUBDATAPROC,    glCopyBufferSubData)            \
    PROC(PFNGLGETUNIFORMINDICESPROC,    glGetUniformIndices)            \
    PROC(PFNGLGETACTIVEUNIFORMSIVPROC,  glGetActiveUniformsiv)          \
    PROC(PFNGLGETACTIVEUNIFORMNAMEPROC, glGetActiveUniformName)         \
    PROC(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex)         \
    PROC(PFNGLGETACTIVEUNIFORMBLOCKIVPROC,                              \
                                        glGetActiveUniformBlockiv)      \
    PROC(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC,                            \
                                        glGetActiveUniformBlockName)    \
    PROC(PFNGLUNIFORMBLOCKBINDINGPROC,  glUniformBlockBinding)          \
    PROC(PFNGLDRAWELEMENTSBASEVERTEXPROC,                               \
                                        glDrawElementsBaseVertex)       \
    PROC(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC,                          \
                                        glDrawRangeElementsBaseVertex)  \
    PROC(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC,                      \
                                        glDrawElementsInstancedBaseVertex) \
    PROC(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC,                          \
                                        glMultiDrawElementsBaseVertex)  \
    PROC(PFNGLPROVOKINGVERTEXPROC,      glProvokingVertex)              \
    PROC(PFNGLFENCESYNCPROC,            glFenceSync)                    \
    PROC(PFNGLISSYNCPROC,               glIsSync)                       \
    PROC(PFNGLDELETESYNCPROC,           glDeleteSync)                   \
    PROC(PFNGLCLIENTWAITSYNCPROC,       glClientWaitSync)               \
    PROC(PFNGLWAITSYNCPROC,             glWaitSync)                     \
    PROC(PFNGLGETINTEGER64VPROC,        glGetInteger64v)                \
    PROC(PFNGLGETSYNCIVPROC,            glGetSynciv)                    \
    PROC(PFNGLGETINTEGER64I_VPROC,      glGetInteger64i_v)              \
    PROC(PFNGLGETBUFFERPARAMETERI64VPROC,                               \
                                        glGetBufferParameteri64v)       \
    PROC(PFNGLFRAMEBUFFERTEXTUREPROC,   glFramebufferTexture)           \
    PROC(PFNGLTEXIMAGE2DMULTISAMPLEPROC,                                \
                                        glTexImage2DMultisample)        \
    PROC(PFNGLTEXIMAGE3DMULTISAMPLEPROC,                                \
                                        glTexImage3DMultisample)        \
    PROC(PFNGLGETMULTISAMPLEFVPROC,     glGetMultisamplefv)             \
    PROC(PFNGLSAMPLEMASKIPROC,          glSampleMaski)                  \
    PROC(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC,                          \
                                        glBindFragDataLocationIndexed)  \
    PROC(PFNGLGETFRAGDATAINDEXPROC,     glGetFragDataIndex)             \
    PROC(PFNGLGENSAMPLERSPROC,          glGenSamplers)                  \
    PROC(PFNGLDELETESAMPLERSPROC,       glDeleteSamplers)               \
    PROC(PFNGLISSAMPLERPROC,            glIsSampler)                    \
    PROC(PFNGLBINDSAMPLERPROC,          glBindSampler)                  \
    PROC(PFNGLSAMPLERPARAMETERIPROC,    glSamplerParameteri)            \
    PROC(PFNGLSAMPLERPARAMETERIVPROC,   glSamplerParameteriv)           \
    PROC(PFNGLSAMPLERPARAMETERFPROC,    glSamplerParameterf)            \
    PROC(PFNGLSAMPLERPARAMETERFVPROC,   glSamplerParameterfv)           \
    PROC(PFNGLSAMPLERPARAMETERIIVPROC,  glSamplerParameterIiv)          \
    PROC(PFNGLSAMPLERPARAMETERIUIVPROC, glSamplerParameterIuiv)         \
    PROC(PFNGLGETSAMPLERPARAMETERIVPROC,                                \
                                        glGetSamplerParameteriv)        \
    PROC(PFNGLGETSAMPLERPARAMETERIIVPROC,                               \
                                        glGetSamplerParameterIiv)       \
    PROC(PFNGLGETSAMPLERPARAMETERFVPROC,                                \
                                        glGetSamplerParameterfv)        \
    PROC(PFNGLGETSAMPLERPARAMETERIUIVPROC,                              \
                                        glGetSamplerParameterIuiv)      \
    PROC(PFNGLQUERYCOUNTERPROC,         glQueryCounter)                 \
    PROC(PFNGLGETQUERYOBJECTI64VPROC,   glGetQueryObjecti64v)           \
    PROC(PFNGLGETQUERYOBJECTUI64VPROC,  glGetQueryObjectui64v)          \
    PROC(PFNGLVERTEXATTRIBDIVISORPROC,  glVertexAttribDivisor)          \
    PROC(PFNGLVERTEXATTRIBP1UIPROC,     glVertexAttribP1ui)             \
    PROC(PFNGLVERTEXATTRIBP1UIVPROC,    glVertexAttribP1uiv)            \
    PROC(PFNGLVERTEXATTRIBP2UIPROC,     glVertexAttribP2ui)             \
    PROC(PFNGLVERTEXATTRIBP2UIVPROC,    glVertexAttribP2uiv)            \
    PROC(PFNGLVERTEXATTRIBP3UIPROC,     glVertexAttribP3ui)             \
    PROC(PFNGLVERTEXATTRIBP3UIVPROC,    glVertexAttribP3uiv)            \
    PROC(PFNGLVERTEXATTRIBP4UIPROC,     glVertexAttribP4ui)             \
    PROC(PFNGLVERTEXATTRIBP4UIVPROC,    glVertexAttribP4uiv)            \
    PROC(PFNGLVERTEXP2UIPROC,           glVertexP2ui)                   \
    PROC(PFNGLVERTEXP2UIVPROC,          glVertexP2uiv)                  \
    PROC(PFNGLVERTEXP3UIPROC,           glVertexP3ui)                   \
    PROC(PFNGLVERTEXP3UIVPROC,          glVertexP3uiv)                  \
    PROC(PFNGLVERTEXP4UIPROC,           glVertexP4ui)                   \
    PROC(PFNGLVERTEXP4UIVPROC,          glVertexP4uiv)                  \
    PROC(PFNGLTEXCOORDP1UIPROC,         glTexCoordP1ui)                 \
    PROC(PFNGLTEXCOORDP1UIVPROC,        glTexCoordP1uiv)                \
    PROC(PFNGLTEXCOORDP2UIPROC,         glTexCoordP2ui)                 \
    PROC(PFNGLTEXCOORDP2UIVPROC,        glTexCoordP2uiv)                \
    PROC(PFNGLTEXCOORDP3UIPROC,         glTexCoordP3ui)                 \
    PROC(PFNGLTEXCOORDP3UIVPROC,        glTexCoordP3uiv)                \
    PROC(PFNGLTEXCOORDP4UIPROC,         glTexCoordP4ui)                 \
    PROC(PFNGLTEXCOORDP4UIVPROC,        glTexCoordP4uiv)                \
    PROC(PFNGLMULTITEXCOORDP1UIPROC,    glMultiTexCoordP1ui)            \
    PROC(PFNGLMULTITEXCOORDP1UIVPROC,   glMultiTexCoordP1uiv)           \
    PROC(PFNGLMULTITEXCOORDP2UIPROC,    glMultiTexCoordP2ui)            \
    PROC(PFNGLMULTITEXCOORDP2UIVPROC,   glMultiTexCoordP2uiv)           \
    PROC(PFNGLMULTITEXCOORDP3UIPROC,    glMultiTexCoordP3ui)            \
    PROC(PFNGLMULTITEXCOORDP3UIVPROC,   glMultiTexCoordP3uiv)           \
    PROC(PFNGLMULTITEXCOORDP4UIPROC,    glMultiTexCoordP4ui)            \
    PROC(PFNGLMULTITEXCOORDP4UIVPROC,   glMultiTexCoordP4uiv)           \
    PROC(PFNGLNORMALP3UIPROC,           glNormalP3ui)                   \
    PROC(PFNGLNORMALP3UIVPROC,          glNormalP3uiv)                  \
    PROC(PFNGLCOLORP3UIPROC,            glColorP3ui)                    \
    PROC(PFNGLCOLORP3UIVPROC,           glColorP3uiv)                   \
    PROC(PFNGLCOLORP4UIPROC,            glColorP4ui)                    \
    PROC(PFNGLCOLORP4UIVPROC,           glColorP4uiv)                   \
    PROC(PFNGLSECONDARYCOLORP3UIPROC,   glSecondaryColorP3ui)           \
    PROC(PFNGLSECONDARYCOLORP3UIVPROC,  glSecondaryColorP3uiv)

//------------------------------------------------------------------------------

#define PROC(type, name) \
    static type dyn_##name;

PROC_LIST

#undef PROC

//------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4706)
#endif

#define PROC(type, name) \
    if (!(dyn_##name = (type) libqu_gl_get_proc_address(#name))) { \
        LIBQU_LOGE("Failed to load function: %s.\n", #name); \
        return -1; \
    }

static int dyn_load_gl3(void)
{
    PROC_LIST

    return 0;
}

#undef PROC

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//------------------------------------------------------------------------------

/*
 * OpenGL 1.0
 */

#define glCullFace                      dyn_glCullFace
#define glFrontFace                     dyn_glFrontFace
#define glHint                          dyn_glHint
#define glLineWidth                     dyn_glLineWidth
#define glPointSize                     dyn_glPointSize
#define glPolygonMode                   dyn_glPolygonMode
#define glScissor                       dyn_glScissor
#define glTexParameterf                 dyn_glTexParameterf
#define glTexParameterfv                dyn_glTexParameterfv
#define glTexParameteri                 dyn_glTexParameteri
#define glTexParameteriv                dyn_glTexParameteriv
#define glTexImage1D                    dyn_glTexImage1D
#define glTexImage2D                    dyn_glTexImage2D
#define glDrawBuffer                    dyn_glDrawBuffer
#define glClear                         dyn_glClear
#define glClearColor                    dyn_glClearColor
#define glClearStencil                  dyn_glClearStencil
#define glClearDepth                    dyn_glClearDepth
#define glStencilMask                   dyn_glStencilMask
#define glColorMask                     dyn_glColorMask
#define glDepthMask                     dyn_glDepthMask
#define glDisable                       dyn_glDisable
#define glEnable                        dyn_glEnable
#define glFinish                        dyn_glFinish
#define glFlush                         dyn_glFlush
#define glBlendFunc                     dyn_glBlendFunc
#define glLogicOp                       dyn_glLogicOp
#define glStencilFunc                   dyn_glStencilFunc
#define glStencilOp                     dyn_glStencilOp
#define glDepthFunc                     dyn_glDepthFunc
#define glPixelStoref                   dyn_glPixelStoref
#define glPixelStorei                   dyn_glPixelStorei
#define glReadBuffer                    dyn_glReadBuffer
#define glReadPixels                    dyn_glReadPixels
#define glGetBooleanv                   dyn_glGetBooleanv
#define glGetDoublev                    dyn_glGetDoublev
#define glGetError                      dyn_glGetError
#define glGetFloatv                     dyn_glGetFloatv
#define glGetIntegerv                   dyn_glGetIntegerv
#define glGetString                     dyn_glGetString
#define glGetTexImage                   dyn_glGetTexImage
#define glGetTexParameterfv             dyn_glGetTexParameterfv
#define glGetTexParameteriv             dyn_glGetTexParameteriv
#define glGetTexLevelParameterfv        dyn_glGetTexLevelParameterfv
#define glGetTexLevelParameteriv        dyn_glGetTexLevelParameteriv
#define glIsEnabled                     dyn_glIsEnabled
#define glDepthRange                    dyn_glDepthRange
#define glViewport                      dyn_glViewport

/*
 * OpenGL 1.1
 */

#define glDrawArrays                    dyn_glDrawArrays
#define glDrawElements                  dyn_glDrawElements
#define glPolygonOffset                 dyn_glPolygonOffset
#define glCopyTexImage1D                dyn_glCopyTexImage1D
#define glCopyTexImage2D                dyn_glCopyTexImage2D
#define glCopyTexSubImage1D             dyn_glCopyTexSubImage1D
#define glCopyTexSubImage2D             dyn_glCopyTexSubImage2D
#define glTexSubImage1D                 dyn_glTexSubImage1D
#define glTexSubImage2D                 dyn_glTexSubImage2D
#define glBindTexture                   dyn_glBindTexture
#define glDeleteTextures                dyn_glDeleteTextures
#define glGenTextures                   dyn_glGenTextures
#define glIsTexture                     dyn_glIsTexture

/*
 * OpenGL 1.2
 */

#define glDrawRangeElements             dyn_glDrawRangeElements
#define glTexImage3D                    dyn_glTexImage3D
#define glTexSubImage3D                 dyn_glTexSubImage3D
#define glCopyTexSubImage3D             dyn_glCopyTexSubImage3D

/*
 * OpenGL 1.3
 */

#define glActiveTexture                 dyn_glActiveTexture
#define glSampleCoverage                dyn_glSampleCoverage
#define glCompressedTexImage3D          dyn_glCompressedTexImage3D
#define glCompressedTexImage2D          dyn_glCompressedTexImage2D
#define glCompressedTexImage1D          dyn_glCompressedTexImage1D
#define glCompressedTexSubImage3D       dyn_glCompressedTexSubImage3D
#define glCompressedTexSubImage2D       dyn_glCompressedTexSubImage2D
#define glCompressedTexSubImage1D       dyn_glCompressedTexSubImage1D
#define glGetCompressedTexImage         dyn_glGetCompressedTexImage

/*
 * OpenGL 1.4
 */

#define glBlendFuncSeparate             dyn_glBlendFuncSeparate
#define glMultiDrawArrays               dyn_glMultiDrawArrays
#define glMultiDrawElements             dyn_glMultiDrawElements
#define glPointParameterf               dyn_glPointParameterf
#define glPointParameterfv              dyn_glPointParameterfv
#define glPointParameteri               dyn_glPointParameteri
#define glPointParameteriv              dyn_glPointParameteriv
#define glBlendColor                    dyn_glBlendColor
#define glBlendEquation                 dyn_glBlendEquation

/*
 * OpenGL 1.5
 */

#define glGenQueries                    dyn_glGenQueries
#define glDeleteQueries                 dyn_glDeleteQueries
#define glIsQuery                       dyn_glIsQuery
#define glBeginQuery                    dyn_glBeginQuery
#define glEndQuery                      dyn_glEndQuery
#define glGetQueryiv                    dyn_glGetQueryiv
#define glGetQueryObjectiv              dyn_glGetQueryObjectiv
#define glGetQueryObjectuiv             dyn_glGetQueryObjectuiv
#define glBindBuffer                    dyn_glBindBuffer
#define glDeleteBuffers                 dyn_glDeleteBuffers
#define glGenBuffers                    dyn_glGenBuffers
#define glIsBuffer                      dyn_glIsBuffer
#define glBufferData                    dyn_glBufferData
#define glBufferSubData                 dyn_glBufferSubData
#define glGetBufferSubData              dyn_glGetBufferSubData
#define glMapBuffer                     dyn_glMapBuffer
#define glUnmapBuffer                   dyn_glUnmapBuffer
#define glGetBufferParameteriv          dyn_glGetBufferParameteriv
#define glGetBufferPointerv             dyn_glGetBufferPointerv

/*
 * OpenGL 2.0
 */

#define glBlendEquationSeparate         dyn_glBlendEquationSeparate
#define glDrawBuffers                   dyn_glDrawBuffers
#define glStencilOpSeparate             dyn_glStencilOpSeparate
#define glStencilFuncSeparate           dyn_glStencilFuncSeparate
#define glStencilMaskSeparate           dyn_glStencilMaskSeparate
#define glAttachShader                  dyn_glAttachShader
#define glBindAttribLocation            dyn_glBindAttribLocation
#define glCompileShader                 dyn_glCompileShader
#define glCreateProgram                 dyn_glCreateProgram
#define glCreateShader                  dyn_glCreateShader
#define glDeleteProgram                 dyn_glDeleteProgram
#define glDeleteShader                  dyn_glDeleteShader
#define glDetachShader                  dyn_glDetachShader
#define glDisableVertexAttribArray      dyn_glDisableVertexAttribArray
#define glEnableVertexAttribArray       dyn_glEnableVertexAttribArray
#define glGetActiveAttrib               dyn_glGetActiveAttrib
#define glGetActiveUniform              dyn_glGetActiveUniform
#define glGetAttachedShaders            dyn_glGetAttachedShaders
#define glGetAttribLocation             dyn_glGetAttribLocation
#define glGetProgramiv                  dyn_glGetProgramiv
#define glGetProgramInfoLog             dyn_glGetProgramInfoLog
#define glGetShaderiv                   dyn_glGetShaderiv
#define glGetShaderInfoLog              dyn_glGetShaderInfoLog
#define glGetShaderSource               dyn_glGetShaderSource
#define glGetUniformLocation            dyn_glGetUniformLocation
#define glGetUniformfv                  dyn_glGetUniformfv
#define glGetUniformiv                  dyn_glGetUniformiv
#define glGetVertexAttribdv             dyn_glGetVertexAttribdv
#define glGetVertexAttribfv             dyn_glGetVertexAttribfv
#define glGetVertexAttribiv             dyn_glGetVertexAttribiv
#define glGetVertexAttribPointerv       dyn_glGetVertexAttribPointerv
#define glIsProgram                     dyn_glIsProgram
#define glIsShader                      dyn_glIsShader
#define glLinkProgram                   dyn_glLinkProgram
#define glShaderSource                  dyn_glShaderSource
#define glUseProgram                    dyn_glUseProgram
#define glUniform1f                     dyn_glUniform1f
#define glUniform2f                     dyn_glUniform2f
#define glUniform3f                     dyn_glUniform3f
#define glUniform4f                     dyn_glUniform4f
#define glUniform1i                     dyn_glUniform1i
#define glUniform2i                     dyn_glUniform2i
#define glUniform3i                     dyn_glUniform3i
#define glUniform4i                     dyn_glUniform4i
#define glUniform1fv                    dyn_glUniform1fv
#define glUniform2fv                    dyn_glUniform2fv
#define glUniform3fv                    dyn_glUniform3fv
#define glUniform4fv                    dyn_glUniform4fv
#define glUniform1iv                    dyn_glUniform1iv
#define glUniform2iv                    dyn_glUniform2iv
#define glUniform3iv                    dyn_glUniform3iv
#define glUniform4iv                    dyn_glUniform4iv
#define glUniformMatrix2fv              dyn_glUniformMatrix2fv
#define glUniformMatrix3fv              dyn_glUniformMatrix3fv
#define glUniformMatrix4fv              dyn_glUniformMatrix4fv
#define glValidateProgram               dyn_glValidateProgram
#define glVertexAttrib1d                dyn_glVertexAttrib1d
#define glVertexAttrib1dv               dyn_glVertexAttrib1dv
#define glVertexAttrib1f                dyn_glVertexAttrib1f
#define glVertexAttrib1fv               dyn_glVertexAttrib1fv
#define glVertexAttrib1s                dyn_glVertexAttrib1s
#define glVertexAttrib1sv               dyn_glVertexAttrib1sv
#define glVertexAttrib2d                dyn_glVertexAttrib2d
#define glVertexAttrib2dv               dyn_glVertexAttrib2dv
#define glVertexAttrib2f                dyn_glVertexAttrib2f
#define glVertexAttrib2fv               dyn_glVertexAttrib2fv
#define glVertexAttrib2s                dyn_glVertexAttrib2s
#define glVertexAttrib2sv               dyn_glVertexAttrib2sv
#define glVertexAttrib3d                dyn_glVertexAttrib3d
#define glVertexAttrib3dv               dyn_glVertexAttrib3dv
#define glVertexAttrib3f                dyn_glVertexAttrib3f
#define glVertexAttrib3fv               dyn_glVertexAttrib3fv
#define glVertexAttrib3s                dyn_glVertexAttrib3s
#define glVertexAttrib3sv               dyn_glVertexAttrib3sv
#define glVertexAttrib4Nbv              dyn_glVertexAttrib4Nbv
#define glVertexAttrib4Niv              dyn_glVertexAttrib4Niv
#define glVertexAttrib4Nsv              dyn_glVertexAttrib4Nsv
#define glVertexAttrib4Nub              dyn_glVertexAttrib4Nub
#define glVertexAttrib4Nubv             dyn_glVertexAttrib4Nubv
#define glVertexAttrib4Nuiv             dyn_glVertexAttrib4Nuiv
#define glVertexAttrib4Nusv             dyn_glVertexAttrib4Nusv
#define glVertexAttrib4bv               dyn_glVertexAttrib4bv
#define glVertexAttrib4d                dyn_glVertexAttrib4d
#define glVertexAttrib4dv               dyn_glVertexAttrib4dv
#define glVertexAttrib4f                dyn_glVertexAttrib4f
#define glVertexAttrib4fv               dyn_glVertexAttrib4fv
#define glVertexAttrib4iv               dyn_glVertexAttrib4iv
#define glVertexAttrib4s                dyn_glVertexAttrib4s
#define glVertexAttrib4sv               dyn_glVertexAttrib4sv
#define glVertexAttrib4ubv              dyn_glVertexAttrib4ubv
#define glVertexAttrib4uiv              dyn_glVertexAttrib4uiv
#define glVertexAttrib4usv              dyn_glVertexAttrib4usv
#define glVertexAttribPointer           dyn_glVertexAttribPointer

/*
 * OpenGL 2.1
 */

#define glUniformMatrix2x3fv            dyn_glUniformMatrix2x3fv
#define glUniformMatrix3x2fv            dyn_glUniformMatrix3x2fv
#define glUniformMatrix2x4fv            dyn_glUniformMatrix2x4fv
#define glUniformMatrix4x2fv            dyn_glUniformMatrix4x2fv
#define glUniformMatrix3x4fv            dyn_glUniformMatrix3x4fv
#define glUniformMatrix4x3fv            dyn_glUniformMatrix4x3fv

/*
 * OpenGL 3.0
 */

#define glColorMaski                    dyn_glColorMaski
#define glGetBooleani_v                 dyn_glGetBooleani_v
#define glGetIntegeri_v                 dyn_glGetIntegeri_v
#define glEnablei                       dyn_glEnablei
#define glDisablei                      dyn_glDisablei
#define glIsEnabledi                    dyn_glIsEnabledi
#define glBeginTransformFeedback        dyn_glBeginTransformFeedback
#define glEndTransformFeedback          dyn_glEndTransformFeedback
#define glBindBufferRange               dyn_glBindBufferRange
#define glBindBufferBase                dyn_glBindBufferBase
#define glTransformFeedbackVaryings     dyn_glTransformFeedbackVaryings
#define glGetTransformFeedbackVarying   dyn_glGetTransformFeedbackVarying
#define glClampColor                    dyn_glClampColor
#define glBeginConditionalRender        dyn_glBeginConditionalRender
#define glEndConditionalRender          dyn_glEndConditionalRender
#define glVertexAttribIPointer          dyn_glVertexAttribIPointer
#define glGetVertexAttribIiv            dyn_glGetVertexAttribIiv
#define glGetVertexAttribIuiv           dyn_glGetVertexAttribIuiv
#define glVertexAttribI1i               dyn_glVertexAttribI1i
#define glVertexAttribI2i               dyn_glVertexAttribI2i
#define glVertexAttribI3i               dyn_glVertexAttribI3i
#define glVertexAttribI4i               dyn_glVertexAttribI4i
#define glVertexAttribI1ui              dyn_glVertexAttribI1ui
#define glVertexAttribI2ui              dyn_glVertexAttribI2ui
#define glVertexAttribI3ui              dyn_glVertexAttribI3ui
#define glVertexAttribI4ui              dyn_glVertexAttribI4ui
#define glVertexAttribI1iv              dyn_glVertexAttribI1iv
#define glVertexAttribI2iv              dyn_glVertexAttribI2iv
#define glVertexAttribI3iv              dyn_glVertexAttribI3iv
#define glVertexAttribI4iv              dyn_glVertexAttribI4iv
#define glVertexAttribI1uiv             dyn_glVertexAttribI1uiv
#define glVertexAttribI2uiv             dyn_glVertexAttribI2uiv
#define glVertexAttribI3uiv             dyn_glVertexAttribI3uiv
#define glVertexAttribI4uiv             dyn_glVertexAttribI4uiv
#define glVertexAttribI4bv              dyn_glVertexAttribI4bv
#define glVertexAttribI4sv              dyn_glVertexAttribI4sv
#define glVertexAttribI4ubv             dyn_glVertexAttribI4ubv
#define glVertexAttribI4usv             dyn_glVertexAttribI4usv
#define glGetUniformuiv                 dyn_glGetUniformuiv
#define glBindFragDataLocation          dyn_glBindFragDataLocation
#define glGetFragDataLocation           dyn_glGetFragDataLocation
#define glUniform1ui                    dyn_glUniform1ui
#define glUniform2ui                    dyn_glUniform2ui
#define glUniform3ui                    dyn_glUniform3ui
#define glUniform4ui                    dyn_glUniform4ui
#define glUniform1uiv                   dyn_glUniform1uiv
#define glUniform2uiv                   dyn_glUniform2uiv
#define glUniform3uiv                   dyn_glUniform3uiv
#define glUniform4uiv                   dyn_glUniform4uiv
#define glTexParameterIiv               dyn_glTexParameterIiv
#define glTexParameterIuiv              dyn_glTexParameterIuiv
#define glGetTexParameterIiv            dyn_glGetTexParameterIiv
#define glGetTexParameterIuiv           dyn_glGetTexParameterIuiv
#define glClearBufferiv                 dyn_glClearBufferiv
#define glClearBufferuiv                dyn_glClearBufferuiv
#define glClearBufferfv                 dyn_glClearBufferfv
#define glClearBufferfi                 dyn_glClearBufferfi
#define glGetStringi                    dyn_glGetStringi
#define glIsRenderbuffer                dyn_glIsRenderbuffer
#define glBindRenderbuffer              dyn_glBindRenderbuffer
#define glDeleteRenderbuffers           dyn_glDeleteRenderbuffers
#define glGenRenderbuffers              dyn_glGenRenderbuffers
#define glRenderbufferStorage           dyn_glRenderbufferStorage
#define glGetRenderbufferParameteriv    dyn_glGetRenderbufferParameteriv
#define glIsFramebuffer                 dyn_glIsFramebuffer
#define glBindFramebuffer               dyn_glBindFramebuffer
#define glDeleteFramebuffers            dyn_glDeleteFramebuffers
#define glGenFramebuffers               dyn_glGenFramebuffers
#define glCheckFramebufferStatus        dyn_glCheckFramebufferStatus
#define glFramebufferTexture1D          dyn_glFramebufferTexture1D
#define glFramebufferTexture2D          dyn_glFramebufferTexture2D
#define glFramebufferTexture3D          dyn_glFramebufferTexture3D
#define glFramebufferRenderbuffer       dyn_glFramebufferRenderbuffer
#define glGetFramebufferAttachmentParameteriv \
                                        dyn_glGetFramebufferAttachmentParameteriv
#define glGenerateMipmap                dyn_glGenerateMipmap
#define glBlitFramebuffer               dyn_glBlitFramebuffer
#define glRenderbufferStorageMultisample \
                                        dyn_glRenderbufferStorageMultisample
#define glFramebufferTextureLayer       dyn_glFramebufferTextureLayer
#define glMapBufferRange                dyn_glMapBufferRange
#define glFlushMappedBufferRange        dyn_glFlushMappedBufferRange
#define glBindVertexArray               dyn_glBindVertexArray
#define glDeleteVertexArrays            dyn_glDeleteVertexArrays
#define glGenVertexArrays               dyn_glGenVertexArrays
#define glIsVertexArray                 dyn_glIsVertexArray

/*
 * OpenGL 3.1
 */

#define glDrawArraysInstanced           dyn_glDrawArraysInstanced
#define glDrawElementsInstanced         dyn_glDrawElementsInstanced
#define glTexBuffer                     dyn_glTexBuffer
#define glPrimitiveRestartIndex         dyn_glPrimitiveRestartIndex
#define glCopyBufferSubData             dyn_glCopyBufferSubData
#define glGetUniformIndices             dyn_glGetUniformIndices
#define glGetActiveUniformsiv           dyn_glGetActiveUniformsiv
#define glGetActiveUniformName          dyn_glGetActiveUniformName
#define glGetUniformBlockIndex          dyn_glGetUniformBlockIndex
#define glGetActiveUniformBlockiv       dyn_glGetActiveUniformBlockiv
#define glGetActiveUniformBlockName     dyn_glGetActiveUniformBlockName
#define glUniformBlockBinding           dyn_glUniformBlockBinding

/*
 * OpenGL 3.2
 */

#define glDrawElementsBaseVertex        dyn_glDrawElementsBaseVertex
#define glDrawRangeElementsBaseVertex   dyn_glDrawRangeElementsBaseVertex
#define glDrawElementsInstancedBaseVertex \
                                        dyn_glDrawElementsInstancedBaseVertex
#define glMultiDrawElementsBaseVertex   dyn_glMultiDrawElementsBaseVertex
#define glProvokingVertex               dyn_glProvokingVertex
#define glFenceSync                     dyn_glFenceSync
#define glIsSync                        dyn_glIsSync
#define glDeleteSync                    dyn_glDeleteSync
#define glClientWaitSync                dyn_glClientWaitSync
#define glWaitSync                      dyn_glWaitSync
#define glGetInteger64v                 dyn_glGetInteger64v
#define glGetSynciv                     dyn_glGetSynciv
#define glGetInteger64i_v               dyn_glGetInteger64i_v
#define glGetBufferParameteri64v        dyn_glGetBufferParameteri64v
#define glFramebufferTexture            dyn_glFramebufferTexture
#define glTexImage2DMultisample         dyn_glTexImage2DMultisample
#define glTexImage3DMultisample         dyn_glTexImage3DMultisample
#define glGetMultisamplefv              dyn_glGetMultisamplefv
#define glSampleMaski                   dyn_glSampleMaski

/*
 * OpenGL 3.3
 */

#define glBindFragDataLocationIndexed   dyn_glBindFragDataLocationIndexed
#define glGetFragDataIndex              dyn_glGetFragDataIndex
#define glGenSamplers                   dyn_glGenSamplers
#define glDeleteSamplers                dyn_glDeleteSamplers
#define glIsSampler                     dyn_glIsSampler
#define glBindSampler                   dyn_glBindSampler
#define glSamplerParameteri             dyn_glSamplerParameteri
#define glSamplerParameteriv            dyn_glSamplerParameteriv
#define glSamplerParameterf             dyn_glSamplerParameterf
#define glSamplerParameterfv            dyn_glSamplerParameterfv
#define glSamplerParameterIiv           dyn_glSamplerParameterIiv
#define glSamplerParameterIuiv          dyn_glSamplerParameterIuiv
#define glGetSamplerParameteriv         dyn_glGetSamplerParameteriv
#define glGetSamplerParameterIiv        dyn_glGetSamplerParameterIiv
#define glGetSamplerParameterfv         dyn_glGetSamplerParameterfv
#define glGetSamplerParameterIuiv       dyn_glGetSamplerParameterIuiv
#define glQueryCounter                  dyn_glQueryCounter
#define glGetQueryObjecti64v            dyn_glGetQueryObjecti64v
#define glGetQueryObjectui64v           dyn_glGetQueryObjectui64v
#define glVertexAttribDivisor           dyn_glVertexAttribDivisor
#define glVertexAttribP1ui              dyn_glVertexAttribP1ui
#define glVertexAttribP1uiv             dyn_glVertexAttribP1uiv
#define glVertexAttribP2ui              dyn_glVertexAttribP2ui
#define glVertexAttribP2uiv             dyn_glVertexAttribP2uiv
#define glVertexAttribP3ui              dyn_glVertexAttribP3ui
#define glVertexAttribP3uiv             dyn_glVertexAttribP3uiv
#define glVertexAttribP4ui              dyn_glVertexAttribP4ui
#define glVertexAttribP4uiv             dyn_glVertexAttribP4uiv
#define glVertexP2ui                    dyn_glVertexP2ui
#define glVertexP2uiv                   dyn_glVertexP2uiv
#define glVertexP3ui                    dyn_glVertexP3ui
#define glVertexP3uiv                   dyn_glVertexP3uiv
#define glVertexP4ui                    dyn_glVertexP4ui
#define glVertexP4uiv                   dyn_glVertexP4uiv
#define glTexCoordP1ui                  dyn_glTexCoordP1ui
#define glTexCoordP1uiv                 dyn_glTexCoordP1uiv
#define glTexCoordP2ui                  dyn_glTexCoordP2ui
#define glTexCoordP2uiv                 dyn_glTexCoordP2uiv
#define glTexCoordP3ui                  dyn_glTexCoordP3ui
#define glTexCoordP3uiv                 dyn_glTexCoordP3uiv
#define glTexCoordP4ui                  dyn_glTexCoordP4ui
#define glTexCoordP4uiv                 dyn_glTexCoordP4uiv
#define glMultiTexCoordP1ui             dyn_glMultiTexCoordP1ui
#define glMultiTexCoordP1uiv            dyn_glMultiTexCoordP1uiv
#define glMultiTexCoordP2ui             dyn_glMultiTexCoordP2ui
#define glMultiTexCoordP2uiv            dyn_glMultiTexCoordP2uiv
#define glMultiTexCoordP3ui             dyn_glMultiTexCoordP3ui
#define glMultiTexCoordP3uiv            dyn_glMultiTexCoordP3uiv
#define glMultiTexCoordP4ui             dyn_glMultiTexCoordP4ui
#define glMultiTexCoordP4uiv            dyn_glMultiTexCoordP4uiv
#define glNormalP3ui                    dyn_glNormalP3ui
#define glNormalP3uiv                   dyn_glNormalP3uiv
#define glColorP3ui                     dyn_glColorP3ui
#define glColorP3uiv                    dyn_glColorP3uiv
#define glColorP4ui                     dyn_glColorP4ui
#define glColorP4uiv                    dyn_glColorP4uiv
#define glSecondaryColorP3ui            dyn_glSecondaryColorP3ui
#define glSecondaryColorP3uiv           dyn_glSecondaryColorP3uiv

//------------------------------------------------------------------------------

#endif // LIBQU_DYN_GL3_H_INC
