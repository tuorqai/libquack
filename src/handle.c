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

#include <stb_ds.h>
#include "handle.h"

//------------------------------------------------------------------------------

struct item
{
    qu_handle key;
    void *value;
};

struct space
{
    struct item *map;
    intptr_t counter;
};

static struct
{
    struct space spaces[LIBQU_TOTAL_HANDLE_TYPES];
} priv;

//------------------------------------------------------------------------------

qu_handle libqu_handle_create(enum libqu_handle_type type, void *data)
{
    struct item item = {
        priv.spaces[type].counter++,
        data,
    };

    hmputs(priv.spaces[type].map, item);
    return item.key;
}

void libqu_handle_destroy(enum libqu_handle_type type, qu_handle id)
{
    hmdel(priv.spaces[type].map, id);
}

void *libqu_handle_get(enum libqu_handle_type type, qu_handle id)
{
    struct item *item_p = hmgetp_null(priv.spaces[type].map, id);

    if (item_p) {
        return item_p->value;
    }

    return NULL;
}
