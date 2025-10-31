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

#ifndef LIBQU_HANDLE_H_INC
#define LIBQU_HANDLE_H_INC

//------------------------------------------------------------------------------

#include "libquack.h"

//------------------------------------------------------------------------------

enum libqu_handle_type
{
    LIBQU_HANDLE_IMAGE,
    LIBQU_HANDLE_TEXTURE,
    LIBQU_HANDLE_WAVE,
    LIBQU_HANDLE_SOUND,
    LIBQU_HANDLE_MUSIC,
    LIBQU_TOTAL_HANDLE_TYPES,
};

//------------------------------------------------------------------------------

void libqu_release_handles(void);

qu_handle libqu_handle_create(enum libqu_handle_type type, void *data);
void libqu_handle_destroy(enum libqu_handle_type type, qu_handle id);
void *libqu_handle_get(enum libqu_handle_type type, qu_handle id);

//------------------------------------------------------------------------------

#endif // LIBQU_HANDLE_H_INC
