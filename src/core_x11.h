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

#ifndef LIBQU_GRAPHICS_X11_H_INC
#define LIBQU_GRAPHICS_X11_H_INC

//------------------------------------------------------------------------------

#include <GL/glx.h>
#include <GL/glxext.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include "core.h"

//------------------------------------------------------------------------------

// glXCreateContext() from GLX 1.0
typedef GLXContext(*PFNGLXCREATECONTEXTPROC)(Display *, XVisualInfo *,
    GLXContext, Bool);

// glXDestroyContext() from GLX 1.0
typedef void (*PFNGLXDESTROYCONTEXTPROC)(Display *, GLXContext);

// glXQueryVersion() from GLX 1.0
typedef Bool (*PFNGLXQUERYVERSION)(Display *, int *, int *);

// glXSwapBuffers() from GLX 1.0
typedef void (*PFNGLXSWAPBUFFERSPROC)(Display *, GLXDrawable);

// glXQueryExtensionsString() from GLX 1.1
typedef char const *(*PFNGLXQUERYEXTENSIONSSTRINGPROC)(Display *, int);

//------------------------------------------------------------------------------

struct glx_lib
{
    void *so;

    // GLX 1.0+

    PFNGLXCREATECONTEXTPROC             _glXCreateContext;
    PFNGLXDESTROYCONTEXTPROC            _glXDestroyContext;
    PFNGLXQUERYVERSION                  _glXQueryVersion;
    PFNGLXSWAPBUFFERSPROC               _glXSwapBuffers;

    // GLX 1.1+

    PFNGLXQUERYEXTENSIONSSTRINGPROC     _glXQueryExtensionsString;

    // GLX 1.3+

    PFNGLXCHOOSEFBCONFIGPROC            _glXChooseFBConfig;
    PFNGLXCREATEWINDOWPROC              _glXCreateWindow;
    PFNGLXDESTROYWINDOWPROC             _glXDestroyWindow;
    PFNGLXGETFBCONFIGATTRIBPROC         _glXGetFBConfigAttrib;
    PFNGLXGETVISUALFROMFBCONFIGPROC     _glXGetVisualFromFBConfig;
    PFNGLXMAKECONTEXTCURRENTPROC        _glXMakeContextCurrent;

    // GLX 1.4+

    PFNGLXGETPROCADDRESSPROC            _glXGetProcAddress;

    // Extensions

    PFNGLXCREATECONTEXTATTRIBSARBPROC   _glXCreateContextAttribsARB;
    PFNGLXGETPROCADDRESSPROC            _glXGetProcAddressARB;
    PFNGLXSWAPINTERVALEXTPROC           _glXSwapIntervalEXT;

    // Flags

    bool ARB_create_context;
    bool ARB_create_context_profile;
    bool EXT_swap_control;
    bool EXT_create_context_es2_profile;
};

//------------------------------------------------------------------------------

#define glXCreateContext                priv.glx._glXCreateContext
#define glXDestroyContext               priv.glx._glXDestroyContext
#define glXQueryVersion                 priv.glx._glXQueryVersion
#define glXSwapBuffers                  priv.glx._glXSwapBuffers
#define glXQueryExtensionsString        priv.glx._glXQueryExtensionsString
#define glXChooseFBConfig               priv.glx._glXChooseFBConfig
#define glXCreateWindow                 priv.glx._glXCreateWindow
#define glXDestroyWindow                priv.glx._glXDestroyWindow
#define glXGetFBConfigAttrib            priv.glx._glXGetFBConfigAttrib
#define glXGetVisualFromFBConfig        priv.glx._glXGetVisualFromFBConfig
#define glXMakeContextCurrent           priv.glx._glXMakeContextCurrent
#define glXGetProcAddress               priv.glx._glXGetProcAddress
#define glXCreateContextAttribsARB      priv.glx._glXCreateContextAttribsARB
#define glXGetProcAddressARB            priv.glx._glXGetProcAddressARB
#define glXSwapIntervalEXT              priv.glx._glXSwapIntervalEXT

//------------------------------------------------------------------------------

#endif // LIBQU_GRAPHICS_X11_H_INC
