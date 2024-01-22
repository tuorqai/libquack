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
    _UTF8_STRING,
    __NET_SUPPORTED,
    __NET_WM_NAME,
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
    unsigned int extras;
    Atom atoms[TOTAL_ATOMS];
    long event_mask;
    Colormap colormap;
    Window window;

    char class_str[256];
    char title_str[256];

    struct {
        unsigned int extensions;
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
        PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;

        int version;
        GLXContext context;
        GLXDrawable surface;
    } glx;
} priv;

//------------------------------------------------------------------------------

static void check_extras(void)
{
    Atom type;
    int format;
    unsigned long items;
    unsigned long bytes_after;
    Atom *atoms;

    XGetWindowProperty(priv.dpy, priv.root,
        priv.atoms[__NET_SUPPORTED],
        0, 0xFFFFFFFF, False, AnyPropertyType,
        &type, &format, &items, &bytes_after,
        (unsigned char **) &atoms);

    priv.extras = 0;

    for (unsigned long i = 0; i < items; i++) {
        if (atoms[i] == priv.atoms[__NET_WM_NAME]) {
            priv.extras |= (1 << __NET_WM_NAME);
        }
    }
}

static void store_atoms(void)
{
    char *names[TOTAL_ATOMS] = {
        "WM_PROTOCOLS",
        "WM_DELETE_WINDOW",
        "UTF8_STRING",
        "_NET_SUPPORTED",
        "_NET_WM_NAME",
    };

    XInternAtoms(priv.dpy, names, TOTAL_ATOMS, False, priv.atoms);
    check_extras();
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

static qu_vec2i calc_center_of_window(qu_vec2i size)
{
    qu_vec2i pos = {
        (DisplayWidth(priv.dpy, priv.screen) / 2) - (size.x / 2),
        (DisplayHeight(priv.dpy, priv.screen) / 2) - (size.y / 2),
    };

    return pos;
}

static void store_class(char const *str)
{
    strncpy(priv.class_str, str, sizeof(priv.class_str) - 1);

    XClassHint *hint = XAllocClassHint();

    if (!hint) {
        return;
    }

    hint->res_class = priv.class_str;
    XSetClassHint(priv.dpy, priv.window, hint);

    XFree(hint);
}

static void store_title(char const *str)
{
    strncpy(priv.title_str, str, sizeof(priv.title_str) - 1);

    XStoreName(priv.dpy, priv.window, priv.title_str);

    if (priv.extras & (1 << __NET_WM_NAME)) {
        XChangeProperty(priv.dpy, priv.window,
            priv.atoms[__NET_WM_NAME],
            priv.atoms[_UTF8_STRING],
            8, PropModeReplace,
            (unsigned char *) priv.title_str,
            strlen(priv.title_str));
    }
}

static bool create_window(struct libqu_core_params const *params, XVisualInfo *vi)
{
    priv.event_mask = ExposureMask;
    priv.colormap = XCreateColormap(priv.dpy, priv.root, vi->visual, AllocNone);

    unsigned long attrmask = CWEventMask | CWColormap;

    XSetWindowAttributes attr = {
        .event_mask = priv.event_mask,
        .colormap = priv.colormap,
    };

    qu_vec2i pos = calc_center_of_window(params->window_size);
    qu_vec2i size = params->window_size;

    priv.window = XCreateWindow(
        priv.dpy,       // display
        priv.root,      // parent
        pos.x, pos.y,   // position
        size.x, size.y, // size
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

    XSizeHints hints = {
        .flags = PPosition | PSize | PMinSize | PMaxSize,
        .x = pos.x,
        .y = pos.y,
        .width = size.x,
        .height = size.y,
        .min_width = size.x,
        .max_width = size.x,
        .min_height = size.y,
        .max_height = size.y,
    };

    XSetWMNormalHints(priv.dpy, priv.window, &hints);
    XMapWindow(priv.dpy, priv.window);

    store_class("libquack"); // Latin-1
    store_title(params->window_title);

    return true;
}

static void create_modern_glx_context(GLXFBConfig fbc, int version)
{
    int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB,  (version / 100),
        GLX_CONTEXT_MINOR_VERSION_ARB,  (version % 100) / 10,
        None,                           None,
        None,
    };

    if (priv.glx.extensions & _ARB_create_context_profile) {
        if (version >= 300) {
            attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
            attribs[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
        }
    }

    priv.glx.context = priv.glx.glXCreateContextAttribsARB(
        priv.dpy, fbc, NULL, True, attribs
    );
}

static bool create_glx_context(GLXFBConfig fbc, XVisualInfo *vi)
{
    if (priv.glx.extensions & _ARB_create_context) {
        int versions[] = {
            460, 450, 440, 430, 420, 410, 400,
            330, 320, 310, 300,
            210,
        };

        for (size_t i = 0; i < sizeof(versions) / sizeof(versions[0]); i++) {
            create_modern_glx_context(fbc, versions[i]);

            if (priv.glx.context) {
                priv.glx.version = versions[i];
                break;
            }
        }
    }

    XFree(vi);

    if (!priv.glx.context) {
        priv.glx.context = glXCreateContext(priv.dpy, vi, NULL, True);

        if (!priv.glx.context) {
            return false;
        }

        priv.glx.version = 120;
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

    if (!create_window(params, vi)) {
        return false;
    }

    if (!create_glx_context(fbc, vi)) {
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

static bool core_x11_set_window_title(char const *title)
{
    store_title(title);

    return true;
}

static bool core_x11_set_window_size(qu_vec2i size)
{
    XSizeHints hints = {
        .flags = PMinSize | PMaxSize,
        .min_width = size.x,
        .max_width = size.x,
        .min_height = size.y,
        .max_height = size.y,
    };

    XSetWMNormalHints(priv.dpy, priv.window, &hints);

    qu_vec2i pos = calc_center_of_window(size);
    XMoveResizeWindow(priv.dpy, priv.window, pos.x, pos.y, size.x, size.y);

    return true;
}

static int core_glx_get_version(void)
{
    return priv.glx.version;
}

static void *core_glx_get_proc_address(char const *name)
{
    return glXGetProcAddress((GLubyte const *) name);
}

//------------------------------------------------------------------------------

struct libqu_core_impl const libqu_core_x11_impl = {
    core_x11_check_if_available,
    core_x11_initialize,
    core_x11_terminate,
    core_x11_process,
    core_x11_swap,
    core_x11_set_window_title,
    core_x11_set_window_size,
    core_glx_get_version,
    core_glx_get_proc_address,
};

//------------------------------------------------------------------------------

#endif // QU_USE_X11

