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

#ifndef LIBQU_GRAPHICS_H_INC
#define LIBQU_GRAPHICS_H_INC

//------------------------------------------------------------------------------

#include "libqu/libqu.h"

//------------------------------------------------------------------------------

struct libqu_graphics_params
{
    int unused;
};

//------------------------------------------------------------------------------

void libqu_graphics_initialize(struct libqu_graphics_params const *params);
void libqu_graphics_terminate(void);

//------------------------------------------------------------------------------

#endif // LIBQU_GRAPHICS_H_INC

