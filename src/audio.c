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

#include "audio.h"
#include "log.h"

//------------------------------------------------------------------------------

static struct libqu_audio_impl const *impl_list[] = {
    &libqu_audio_null_impl,
};

//------------------------------------------------------------------------------

static struct
{
    struct libqu_audio_impl const *impl;
} priv;

//------------------------------------------------------------------------------

static struct libqu_audio_impl const *choose_impl(void)
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

void libqu_audio_initialize(struct libqu_audio_params const *params)
{
    priv.impl = choose_impl();

    if (!priv.impl->initialize(params)) {
        LIBQU_LOGE("Failed to initialize libqu::audio implementation.\n");
        abort();
    }

    LIBQU_LOGI("Initialized.\n");
}

void libqu_audio_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}
