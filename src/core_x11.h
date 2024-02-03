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

typedef XClassHint *(*PFNXALLOCCLASSHINTPROC)(void);
typedef int (*PFNXCHANGEPROPERTYPROC)(Display *, Window, Atom, Atom, int, int, _Xconst unsigned char *, int);
typedef int (*PFNXCHECKTYPEDWINDOWEVENTPROC)(Display *, Window, int, XEvent *);
typedef int (*PFNXCHECKWINDOWEVENTPROC)(Display *, Window, long, XEvent *);
typedef int (*PFNXCLOSEDISPLAYPROC)(Display *);
typedef Colormap (*PFNXCREATECOLORMAPPROC)(Display *, Window, Visual *, int);
typedef Window (*PFNXCREATEWINDOWPROC)(Display *, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual *, unsigned long, XSetWindowAttributes *);
typedef int (*PFNXDESTROYWINDOWPROC)(Display *, Window);
typedef int (*PFNXFREEPROC)(void *);
typedef int (*PFNXFREECOLORMAPPROC)(Display *, Colormap);
typedef int (*PFNXGETWINDOWPROPERTYPROC)(Display *, Window, Atom, long,long, Bool, Atom, Atom *, int *, unsigned long *, unsigned long *, unsigned char **);
typedef Status (*PFNXINTERNATOMSPROC)(Display *, char **, int, Bool, Atom *);
typedef KeySym (*PFNXLOOKUPKEYSYMPROC)(XKeyEvent *, int);
typedef int (*PFNXMAPWINDOWPROC)(Display *, Window);
typedef int (*PFNXMOVERESIZEWINDOWPROC)(Display *, Window, int, int, unsigned int, unsigned int);
typedef Display *(*PFNXOPENDISPLAYPROC)(_Xconst char *);
typedef int (*PFNXSETCLASSHINTPROC)(Display *, Window, XClassHint *);
typedef void (*PFNXSETWMNORMALHINTSPROC)(Display *, Window, XSizeHints *);
typedef Status (*PFNXSETWMPROTOCOLSPROC)(Display *, Window, Atom *, int);
typedef int (*PFNXSTORENAMEPROC)(Display *, Window, _Xconst char *);
typedef Bool (*PFNXKBSETDETECTABLEAUTOREPEATPROC)(Display *, Bool, Bool *);

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

struct xlib
{
    void *so;

    PFNXALLOCCLASSHINTPROC              _XAllocClassHint;
    PFNXCHANGEPROPERTYPROC              _XChangeProperty;
    PFNXCHECKTYPEDWINDOWEVENTPROC       _XCheckTypedWindowEvent;
    PFNXCHECKWINDOWEVENTPROC            _XCheckWindowEvent;
    PFNXCLOSEDISPLAYPROC                _XCloseDisplay;
    PFNXCREATECOLORMAPPROC              _XCreateColormap;
    PFNXCREATEWINDOWPROC                _XCreateWindow;
    PFNXDESTROYWINDOWPROC               _XDestroyWindow;
    PFNXFREEPROC                        _XFree;
    PFNXFREECOLORMAPPROC                _XFreeColormap;
    PFNXGETWINDOWPROPERTYPROC           _XGetWindowProperty;
    PFNXINTERNATOMSPROC                 _XInternAtoms;
    PFNXLOOKUPKEYSYMPROC                _XLookupKeysym;
    PFNXMAPWINDOWPROC                   _XMapWindow;
    PFNXMOVERESIZEWINDOWPROC            _XMoveResizeWindow;
    PFNXOPENDISPLAYPROC                 _XOpenDisplay;
    PFNXSETCLASSHINTPROC                _XSetClassHint;
    PFNXSETWMNORMALHINTSPROC            _XSetWMNormalHints;
    PFNXSETWMPROTOCOLSPROC              _XSetWMProtocols;
    PFNXSTORENAMEPROC                   _XStoreName;
    PFNXKBSETDETECTABLEAUTOREPEATPROC   _XkbSetDetectableAutoRepeat;
};

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

#define XAllocClassHint                 priv.xlib._XAllocClassHint
#define XChangeProperty                 priv.xlib._XChangeProperty
#define XCheckTypedWindowEvent          priv.xlib._XCheckTypedWindowEvent
#define XCheckWindowEvent               priv.xlib._XCheckWindowEvent
#define XCloseDisplay                   priv.xlib._XCloseDisplay
#define XCreateColormap                 priv.xlib._XCreateColormap
#define XCreateWindow                   priv.xlib._XCreateWindow
#define XDestroyWindow                  priv.xlib._XDestroyWindow
#define XFree                           priv.xlib._XFree
#define XFreeColormap                   priv.xlib._XFreeColormap
#define XGetWindowProperty              priv.xlib._XGetWindowProperty
#define XInternAtoms                    priv.xlib._XInternAtoms
#define XLookupKeysym                   priv.xlib._XLookupKeysym
#define XMapWindow                      priv.xlib._XMapWindow
#define XMoveResizeWindow               priv.xlib._XMoveResizeWindow
#define XOpenDisplay                    priv.xlib._XOpenDisplay
#define XSetClassHint                   priv.xlib._XSetClassHint
#define XSetWMNormalHints               priv.xlib._XSetWMNormalHints
#define XSetWMProtocols                 priv.xlib._XSetWMProtocols
#define XStoreName                      priv.xlib._XStoreName
#define XkbSetDetectableAutoRepeat      priv.xlib._XkbSetDetectableAutoRepeat

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
