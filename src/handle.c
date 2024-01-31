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
#include "audio.h"
#include "graphics.h"
#include "handle.h"

//------------------------------------------------------------------------------

struct item
{
    qu_handle key;
    void *value;
};

static struct
{
    struct item *hashmaps[LIBQU_TOTAL_HANDLE_TYPES];
    intptr_t counter[LIBQU_TOTAL_HANDLE_TYPES];
} priv;

//------------------------------------------------------------------------------

static void dtor(enum libqu_handle_type type, void *data)
{
    switch (type) {
    case LIBQU_HANDLE_IMAGE:
        libqu_image_destroy(data);
        break;
    case LIBQU_HANDLE_TEXTURE:
        libqu_graphics_destroy_texture(data);
        break;
    case LIBQU_HANDLE_WAVE:
        libqu_wave_destroy(data);
        break;
    case LIBQU_HANDLE_SOUND:
        libqu_audio_delete_sound(data);
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------

void libqu_release_handles(void)
{
    for (int i = 0; i < LIBQU_TOTAL_HANDLE_TYPES; i++) {
        for (int j = 0; j < hmlen(priv.hashmaps[i]); j++) {
            dtor(i, priv.hashmaps[i][j].value);
        }

        hmfree(priv.hashmaps[i]);
    }
}

qu_handle libqu_handle_create(enum libqu_handle_type type, void *data)
{
    struct item item = {
        .key = priv.counter[type]++,
        .value = data,
    };

    hmputs(priv.hashmaps[type], item);
    return item.key;
}

void libqu_handle_destroy(enum libqu_handle_type type, qu_handle id)
{
    struct item *item_p = hmgetp_null(priv.hashmaps[type], id);

    if (item_p) {
        dtor(type, item_p->value);
        (void) hmdel(priv.hashmaps[type], id);
    }
}

void *libqu_handle_get(enum libqu_handle_type type, qu_handle id)
{
    struct item *item_p = hmgetp_null(priv.hashmaps[type], id);

    if (item_p) {
        return item_p->value;
    }

    return NULL;
}
