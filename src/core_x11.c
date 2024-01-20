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

static struct
{
    Display *dpy;
} priv;

//------------------------------------------------------------------------------

static bool core_x11_check_if_available(void)
{
    priv.dpy = XOpenDisplay(NULL);

    return priv.dpy != NULL;
}

static bool core_x11_initialize(struct libqu_core_params const *params)
{
    return true;
}

static void core_x11_terminate(void)
{
    if (priv.dpy) {
        XCloseDisplay(priv.dpy);
    }

    memset(&priv, 0, sizeof(priv));
}

//------------------------------------------------------------------------------

struct libqu_core_impl const libqu_core_x11_impl = {
    core_x11_check_if_available,
    core_x11_initialize,
    core_x11_terminate,
};

//------------------------------------------------------------------------------

#endif // QU_USE_X11

