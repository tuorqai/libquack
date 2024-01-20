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

#ifdef QU_USE_X11

//------------------------------------------------------------------------------

#include <string.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <X11/Xlib.h>
#include "core.h"
#include "log.h"
#include "platform.h"

//------------------------------------------------------------------------------

enum
{
    _WM_PROTOCOLS,
    _WM_DELETE_WINDOW,
    TOTAL_ATOMS,
};

enum
{
    _ARB_create_context = (1 << 0),
    _ARB_create_context_profile = (1 << 1),
    _EXT_swap_control = (1 << 2),
    _EXT_create_context_es2_profile = (1 << 3),
};

static struct
{
    Display *dpy;
    int screen;
    Window root;
    long event_mask;
    Colormap colormap;
    Window window;
    Atom atoms[TOTAL_ATOMS];

    struct {
        unsigned int extensions;
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
        PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;

        GLXContext context;
        GLXDrawable surface;
    } glx;
} priv;

//------------------------------------------------------------------------------

static void store_atoms(void)
{
    char *names[TOTAL_ATOMS] = {
        "WM_PROTOCOLS",
        "WM_DELETE_WINDOW",
    };

    XInternAtoms(priv.dpy, names, TOTAL_ATOMS, False, priv.atoms);
}

static void parse_single_glx_extension(char const *ext)
{
    if (strcmp(ext, "GLX_ARB_create_context") == 0) {
        priv.glx.extensions |= _ARB_create_context;
        priv.glx.glXCreateContextAttribsARB = (void *) glXGetProcAddressARB(
            (GLubyte const *) "glXCreateContextAttribsARB"
        );
        return;
    }

    if (strcmp(ext, "GLX_ARB_create_context_profile") == 0) {
        priv.glx.extensions |= _ARB_create_context_profile;
        return;
    }

    if (strcmp(ext, "GLX_EXT_swap_control") == 0) {
        priv.glx.extensions |= _EXT_swap_control;
        priv.glx.glXSwapIntervalEXT = (void *) glXGetProcAddressARB(
            (GLubyte const *) "glXSwapIntervalEXT"
        );
        return;
    }

    if (strcmp(ext, "GLX_EXT_create_context_es2_profile") == 0) {
        priv.glx.extensions |= _EXT_create_context_es2_profile;
        return;
    }
}

static void parse_glx_extensions(void)
{
    char const *ro = glXQueryExtensionsString(priv.dpy, priv.screen);
    char *rw = pl_calloc(strlen(ro) + 1, sizeof(char));
    strncpy(rw, ro, strlen(ro));

    char *token = strtok(rw, " ");

    while (token) {
        parse_single_glx_extension(token);
        token = strtok(NULL, " ");
    }

    pl_free(rw);
}

static GLXFBConfig choose_fbc(void)
{
    int attribs[] = {
        GLX_RED_SIZE,       (8),
        GLX_GREEN_SIZE,     (8),
        GLX_BLUE_SIZE,      (8),
        GLX_ALPHA_SIZE,     (8),
        GLX_RENDER_TYPE,    (GLX_RGBA_BIT),
        GLX_DRAWABLE_TYPE,  (GLX_WINDOW_BIT),
        GLX_X_RENDERABLE,   (True),
        GLX_DOUBLEBUFFER,   (True),
        None,
    };

    int total = 0;
    GLXFBConfig *list = glXChooseFBConfig(priv.dpy, priv.screen, attribs, &total);

    if (!list || !total) {
        LIBQU_LOGE("No suitable framebuffer configuration found.\n");
        return NULL;
    }

    int best = -1;
    int best_samples = 0;

    for (int i = 0; i < total; i++) {
        int sample_buffers = 0;
        glXGetFBConfigAttrib(priv.dpy, list[i], GLX_SAMPLE_BUFFERS, &sample_buffers);

        if (!sample_buffers) {
            continue;
        }

        int samples = 0;
        glXGetFBConfigAttrib(priv.dpy, list[i], GLX_SAMPLES, &samples);

        if (samples > best_samples) {
            best = i;
            best_samples = samples;
        }
    }

    GLXFBConfig fbc = list[(best == -1) ? 0 : best];
    XFree(list);

    return fbc;
}

static bool create_window(XVisualInfo *vi)
{
    priv.event_mask = ExposureMask;
    priv.colormap = XCreateColormap(priv.dpy, priv.root, vi->visual, AllocNone);

    unsigned long attrmask = CWEventMask | CWColormap;

    XSetWindowAttributes attr = {
        .event_mask = priv.event_mask,
        .colormap = priv.colormap,
    };

    priv.window = XCreateWindow(
        priv.dpy,       // display
        priv.root,      // parent
        0, 0,           // position
        640, 480,       // size
        0,              // border width
        CopyFromParent, // depth
        InputOutput,    // class
        vi->visual,     // visual
        attrmask, &attr
    );

    if (!priv.window) {
        return false;
    }

    Atom protocols[] = { priv.atoms[_WM_DELETE_WINDOW] };
    XSetWMProtocols(priv.dpy, priv.window, protocols, 1);

    XMapWindow(priv.dpy, priv.window);
}

static void create_modern_context(GLXFBConfig fbc)
{
    int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB,  3,
        GLX_CONTEXT_MINOR_VERSION_ARB,  3,
        None,                           None,
        None,
    };

    if (priv.glx.extensions & _ARB_create_context_profile) {
        attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
        attribs[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
    }

    priv.glx.context = priv.glx.glXCreateContextAttribsARB(
        priv.dpy, fbc, NULL, True, attribs
    );
}

static void create_legacy_context(XVisualInfo *vi)
{
    priv.glx.context = glXCreateContext(priv.dpy, vi, NULL, True);
}

static bool create_context(GLXFBConfig fbc, XVisualInfo *vi)
{
    if (priv.glx.extensions & _ARB_create_context) {
        create_modern_context(fbc);
    } else {
        create_legacy_context(vi);
    }

    XFree(vi);

    if (!priv.glx.context) {
        return false;
    }

    priv.glx.surface = glXCreateWindow(priv.dpy, fbc, priv.window, NULL);
    glXMakeContextCurrent(priv.dpy, priv.glx.surface, priv.glx.surface, priv.glx.context);

    return true;
}

static bool handle_event(XEvent *ev)
{
    switch (ev->type) {
    default:
        return false;
    case Expose:
        break;
    }

    return true;
}

//------------------------------------------------------------------------------

static bool core_x11_check_if_available(void)
{
    priv.dpy = XOpenDisplay(NULL);

    return priv.dpy != NULL;
}

static bool core_x11_initialize(struct libqu_core_params const *params)
{
    if (!priv.dpy) {
        return false;
    }

    priv.screen = DefaultScreen(priv.dpy);
    priv.root = DefaultRootWindow(priv.dpy);

    store_atoms();
    parse_glx_extensions();

    GLXFBConfig fbc = choose_fbc();

    if (!fbc) {
        return false;
    }

    XVisualInfo *vi = glXGetVisualFromFBConfig(priv.dpy, fbc);

    if (!create_window(vi)) {
        return false;
    }

    if (!create_context(fbc, vi)) {
        return false;
    }

    return true;
}

static void core_x11_terminate(void)
{
    if (priv.dpy) {
        glXDestroyWindow(priv.dpy, priv.glx.surface);
        glXDestroyContext(priv.dpy, priv.glx.context);

        XDestroyWindow(priv.dpy, priv.window);
        XCloseDisplay(priv.dpy);
    }

    memset(&priv, 0, sizeof(priv));
}

static bool core_x11_process(void)
{
    XEvent event;

    while (XCheckTypedWindowEvent(priv.dpy, priv.window, ClientMessage, &event)) {
        if (event.xclient.data.l[0] == (long) priv.atoms[_WM_DELETE_WINDOW]) {
            return false;
        }
    }

    while (XCheckWindowEvent(priv.dpy, priv.window, priv.event_mask, &event)) {
        handle_event(&event);
    }

    return true;
}

static void core_x11_swap(void)
{
    glXSwapBuffers(priv.dpy, priv.glx.surface);
}

//------------------------------------------------------------------------------

struct libqu_core_impl const libqu_core_x11_impl = {
    core_x11_check_if_available,
    core_x11_initialize,
    core_x11_terminate,
    core_x11_process,
    core_x11_swap,
};

//------------------------------------------------------------------------------

#endif // QU_USE_X11

