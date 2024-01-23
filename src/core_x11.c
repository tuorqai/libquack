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
#include <X11/XKBlib.h>
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

static qu_key key_conv(KeySym sym)
{
    switch (sym)
    {
    case XK_0:              return QU_KEY_0;
    case XK_1:              return QU_KEY_1;
    case XK_2:              return QU_KEY_2;
    case XK_3:              return QU_KEY_3;
    case XK_4:              return QU_KEY_4;
    case XK_5:              return QU_KEY_5;
    case XK_6:              return QU_KEY_6;
    case XK_7:              return QU_KEY_7;
    case XK_8:              return QU_KEY_8;
    case XK_9:              return QU_KEY_9;
    case XK_a:              return QU_KEY_A;
    case XK_b:              return QU_KEY_B;
    case XK_c:              return QU_KEY_C;
    case XK_d:              return QU_KEY_D;
    case XK_e:              return QU_KEY_E;
    case XK_f:              return QU_KEY_F;
    case XK_g:              return QU_KEY_G;
    case XK_h:              return QU_KEY_H;
    case XK_i:              return QU_KEY_I;
    case XK_j:              return QU_KEY_J;
    case XK_k:              return QU_KEY_K;
    case XK_l:              return QU_KEY_L;
    case XK_m:              return QU_KEY_M;
    case XK_n:              return QU_KEY_N;
    case XK_o:              return QU_KEY_O;
    case XK_p:              return QU_KEY_P;
    case XK_q:              return QU_KEY_Q;
    case XK_r:              return QU_KEY_R;
    case XK_s:              return QU_KEY_S;
    case XK_t:              return QU_KEY_T;
    case XK_u:              return QU_KEY_U;
    case XK_v:              return QU_KEY_V;
    case XK_w:              return QU_KEY_W;
    case XK_x:              return QU_KEY_X;
    case XK_y:              return QU_KEY_Y;
    case XK_z:              return QU_KEY_Z;
    case XK_grave:          return QU_KEY_GRAVE;
    case XK_apostrophe:     return QU_KEY_APOSTROPHE;
    case XK_minus:          return QU_KEY_MINUS;
    case XK_equal:          return QU_KEY_EQUAL;
    case XK_bracketleft:    return QU_KEY_LBRACKET;
    case XK_bracketright:   return QU_KEY_RBRACKET;
    case XK_comma:          return QU_KEY_COMMA;
    case XK_period:         return QU_KEY_PERIOD;
    case XK_semicolon:      return QU_KEY_SEMICOLON;
    case XK_slash:          return QU_KEY_SLASH;
    case XK_backslash:      return QU_KEY_BACKSLASH;
    case XK_space:          return QU_KEY_SPACE;
    case XK_Escape:         return QU_KEY_ESCAPE;
    case XK_BackSpace:      return QU_KEY_BACKSPACE;
    case XK_Tab:            return QU_KEY_TAB;
    case XK_Return:         return QU_KEY_ENTER;
    case XK_F1:             return QU_KEY_F1;
    case XK_F2:             return QU_KEY_F2;
    case XK_F3:             return QU_KEY_F3;
    case XK_F4:             return QU_KEY_F4;
    case XK_F5:             return QU_KEY_F5;
    case XK_F6:             return QU_KEY_F6;
    case XK_F7:             return QU_KEY_F7;
    case XK_F8:             return QU_KEY_F8;
    case XK_F9:             return QU_KEY_F9;
    case XK_F10:            return QU_KEY_F10;
    case XK_F11:            return QU_KEY_F11;
    case XK_F12:            return QU_KEY_F12;
    case XK_Up:             return QU_KEY_UP;
    case XK_Down:           return QU_KEY_DOWN;
    case XK_Left:           return QU_KEY_LEFT;
    case XK_Right:          return QU_KEY_RIGHT;
    case XK_Shift_L:        return QU_KEY_LSHIFT;
    case XK_Shift_R:        return QU_KEY_RSHIFT;
    case XK_Control_L:      return QU_KEY_LCTRL;
    case XK_Control_R:      return QU_KEY_RCTRL;
    case XK_Alt_L:          return QU_KEY_LALT;
    case XK_Alt_R:          return QU_KEY_RALT;
    case XK_Super_L:        return QU_KEY_LSUPER;
    case XK_Super_R:        return QU_KEY_RSUPER;
    case XK_Menu:           return QU_KEY_MENU;
    case XK_Page_Up:        return QU_KEY_PGUP;
    case XK_Page_Down:      return QU_KEY_PGDN;
    case XK_Home:           return QU_KEY_HOME;
    case XK_End:            return QU_KEY_END;
    case XK_Insert:         return QU_KEY_INSERT;
    case XK_Delete:         return QU_KEY_DELETE;
    case XK_Print:          return QU_KEY_PRINTSCREEN;
    case XK_Pause:          return QU_KEY_PAUSE;
    case XK_Caps_Lock:      return QU_KEY_CAPSLOCK;
    case XK_Scroll_Lock:    return QU_KEY_SCROLLLOCK;
    case XK_Num_Lock:       return QU_KEY_NUMLOCK;
    case XK_KP_0:           return QU_KEY_KP_0;
    case XK_KP_1:           return QU_KEY_KP_1;
    case XK_KP_2:           return QU_KEY_KP_2;
    case XK_KP_3:           return QU_KEY_KP_3;
    case XK_KP_4:           return QU_KEY_KP_4;
    case XK_KP_5:           return QU_KEY_KP_5;
    case XK_KP_6:           return QU_KEY_KP_6;
    case XK_KP_7:           return QU_KEY_KP_7;
    case XK_KP_8:           return QU_KEY_KP_8;
    case XK_KP_9:           return QU_KEY_KP_9;
    case XK_KP_Multiply:    return QU_KEY_KP_MUL;
    case XK_KP_Add:         return QU_KEY_KP_ADD;
    case XK_KP_Subtract:    return QU_KEY_KP_SUB;
    case XK_KP_Decimal:     return QU_KEY_KP_POINT;
    case XK_KP_Divide:      return QU_KEY_KP_DIV;
    case XK_KP_Enter:       return QU_KEY_KP_ENTER;
    default:
        break;
    }

    return QU_KEY_INVALID;
}

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

    XFree(atoms);
}

static void setup_atoms_and_protocols(void)
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
    priv.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask
        | EnterWindowMask | LeaveWindowMask
        | FocusChangeMask;

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

static bool create_modern_glx_context(GLXFBConfig fbc, int version)
{
    int major = version / 100;
    int minor = (version % 100) / 10;

    int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB,  major,
        GLX_CONTEXT_MINOR_VERSION_ARB,  minor,
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

    if (!priv.glx.context) {
        return false;
    }

    priv.glx.version = version;
    LIBQU_LOGI("Created OpenGL context %d.%d.\n", major, minor);

    return true;
}

static bool create_legacy_glx_context(XVisualInfo *vi)
{
    priv.glx.context = glXCreateContext(priv.dpy, vi, NULL, True);

    if (!priv.glx.context) {
        return false;
    }

    priv.glx.version = 120;
    LIBQU_LOGI("Created legacy OpenGL context.\n");

    return true;
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
            if (create_modern_glx_context(fbc, versions[i])) {
                break;
            }
        }
    } else {
        create_legacy_glx_context(vi);
    }

    XFree(vi);

    if (!priv.glx.context) {
        LIBQU_LOGE("Failed to create OpenGL context.\n");
        return false;
    }

    priv.glx.surface = glXCreateWindow(priv.dpy, fbc, priv.window, NULL);

    return glXMakeContextCurrent(priv.dpy, priv.glx.surface, priv.glx.surface,
        priv.glx.context);
}

static void handle_key_event(XEvent *ev)
{
    enum libqu_event_type type;

    if (ev->type == KeyPress) {
        type = LIBQU_EVENT_KEY_PRESSED;
    } else {
        type = LIBQU_EVENT_KEY_RELEASED;
    }

    KeySym sym = XLookupKeysym(&ev->xkey, ShiftMapIndex);
    qu_key key = key_conv(sym);

    struct libqu_event conv = {
        .type = type,
        .data = {
            .key = key,
        },
    };

    libqu_core_enqueue_event(&conv);
}

static void handle_focus_event(XEvent *ev)
{
    struct libqu_event conv = {
        .type = (ev->type == FocusIn)
            ? LIBQU_EVENT_ACTIVATE
            : LIBQU_EVENT_DEACTIVATE,
    };

    libqu_core_enqueue_event(&conv);
}

static bool handle_event(XEvent *ev)
{
    switch (ev->type) {
    default:
        return false;
    case Expose:
        break;
    case KeyPress:
    case KeyRelease:
        handle_key_event(ev);
        break;
    case EnterNotify:
    case LeaveNotify:
        break;
    case FocusIn:
    case FocusOut:
        handle_focus_event(ev);
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

    if (XkbSetDetectableAutoRepeat(priv.dpy, True, NULL)) {
        LIBQU_LOGI("XkbSetDetectableAutoRepeat is available.\n");
    }

    setup_atoms_and_protocols();
    parse_glx_extensions();

    GLXFBConfig fbc = choose_fbc();

    if (!fbc) {
        LIBQU_LOGE("Failed to choose suitable GLXFBConfig.\n");
        return false;
    }

    XVisualInfo *vi = glXGetVisualFromFBConfig(priv.dpy, fbc);

    if (!create_window(params, vi)) {
        LIBQU_LOGE("Failed to create X11 window.\n");
        return false;
    }

    if (!create_glx_context(fbc, vi)) {
        LIBQU_LOGE("Failed to create or activate GLX context.\n");
        return false;
    }

    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void core_x11_terminate(void)
{
    if (priv.dpy) {
        glXMakeContextCurrent(priv.dpy, None, None, None);
        
        glXDestroyWindow(priv.dpy, priv.glx.surface);
        glXDestroyContext(priv.dpy, priv.glx.context);

        LIBQU_LOGI("OpenGL context is destroyed.\n");

        XDestroyWindow(priv.dpy, priv.window);
        XFreeColormap(priv.dpy, priv.colormap);
        XCloseDisplay(priv.dpy);

        LIBQU_LOGI("Window is destroyed, display is closed.\n");
    }

    memset(&priv, 0, sizeof(priv));

    LIBQU_LOGI("Terminated.\n");
}

static bool core_x11_process(void)
{
    XEvent event;

    while (XCheckTypedWindowEvent(priv.dpy, priv.window, ClientMessage, &event)) {
        if (event.xclient.data.l[0] == (long) priv.atoms[_WM_DELETE_WINDOW]) {
            LIBQU_LOGI("Caught WM_DELETE_WINDOW.\n");
            return false;
        }
    }

    while (XCheckWindowEvent(priv.dpy, priv.window, priv.event_mask, &event)) {
        if (!handle_event(&event)) {
            LIBQU_LOGW("Unhandled event: 0x%04x.\n", event.type);
        }
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
