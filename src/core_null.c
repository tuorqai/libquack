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

static bool core_null_check_if_available(void)
{
    return true;
}

static bool core_null_initialize(struct libqu_core_params const *params)
{
    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void core_null_terminate(void)
{
    LIBQU_LOGI("Terminated.\n");
}

static bool core_null_process(void)
{
    return false;
}

static void core_null_swap(void)
{
}

static int core_null_gl_get_version(void)
{
    return -1;
}

static void *core_null_gl_get_proc_address(char const *name)
{
    return NULL;
}

//------------------------------------------------------------------------------

struct libqu_core_impl const libqu_core_null_impl = {
    core_null_check_if_available,
    core_null_initialize,
    core_null_terminate,
    core_null_process,
    core_null_swap,
    core_null_gl_get_version,
    core_null_gl_get_proc_address,
};

