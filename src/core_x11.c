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
#include <X11/Xlib.h>
#include "core.h"

//------------------------------------------------------------------------------

enum
{
    _WM_PROTOCOLS,
    _WM_DELETE_WINDOW,
    TOTAL_ATOMS,
};

static struct
{
    Display *dpy;
    int screen;
    Window root;
    long event_mask;
    Window window;
    Atom atoms[TOTAL_ATOMS];
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

static bool create_window(void)
{
    priv.event_mask = ExposureMask;

    unsigned long attrmask = CWEventMask;

    XSetWindowAttributes attr = {
        .event_mask = priv.event_mask,
    };

    priv.window = XCreateWindow(
        priv.dpy,       // display
        priv.root,      // parent
        0, 0,           // position
        640, 480,       // size
        0,              // border width
        CopyFromParent, // depth
        InputOutput,    // class
        CopyFromParent, // visual
        attrmask, &attr
    );

    if (!priv.window) {
        return false;
    }

    Atom protocols[] = { priv.atoms[_WM_DELETE_WINDOW] };
    XSetWMProtocols(priv.dpy, priv.window, protocols, 1);

    XMapWindow(priv.dpy, priv.window);
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

    if (!create_window()) {
        return false;
    }

    return true;
}

static void core_x11_terminate(void)
{
    if (priv.dpy) {
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

