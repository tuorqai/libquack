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

#include "core.h"
#include "log.h"

//------------------------------------------------------------------------------

static struct libqu_core_impl const *impl_list[] = {
#ifdef QU_USE_X11
    &libqu_core_x11_impl,
#endif

    &libqu_core_null_impl,
};

//------------------------------------------------------------------------------

static struct
{
    struct libqu_core_impl const *impl;

    char const *window_title;
    qu_vec2i window_size;
} priv;

//------------------------------------------------------------------------------

static struct libqu_core_impl const *choose_impl(void)
{
    int count = sizeof(impl_list) / sizeof(impl_list[0]);

    for (int i = 0; i < count; i++) {
        if (impl_list[i]->check_if_available()) {
            return impl_list[i];
        }
    }

    abort();
}

//------------------------------------------------------------------------------

void libqu_core_initialize(struct libqu_core_params const *params)
{
    priv.impl = choose_impl();

    if (!priv.impl->initialize(params)) {
        LIBQU_LOGE("Failed to initialize libqu::core implementation.\n");
        abort();
    }

    LIBQU_LOGI("Initialized.\n");
}

void libqu_core_terminate(void)
{
    priv.impl->terminate();

    LIBQU_LOGI("Terminated.\n");
}

bool libqu_core_process(void)
{
    return priv.impl->process();
}

void libqu_core_swap(void)
{
    priv.impl->swap();
}

char const *libqu_core_get_window_title(void)
{
    return priv.window_title;
}

qu_vec2i libqu_core_get_window_size(void)
{
    return priv.window_size;
}

void libqu_core_set_window_title(char const *title)
{
    if (priv.impl->set_window_title(title)) {
        priv.window_title = title;
    }
}

void libqu_core_set_window_size(qu_vec2i size)
{
    if (priv.impl->set_window_size(size)) {
        priv.window_size = size;
    }
}

int libqu_gl_get_version(void)
{
    return priv.impl->gl_get_version();
}

void *libqu_gl_get_proc_address(char const *name)
{
    return priv.impl->gl_get_proc_address(name);
}

