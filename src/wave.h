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

#ifndef LIBQU_WAVE_H_INC
#define LIBQU_WAVE_H_INC

//------------------------------------------------------------------------------

#include "fs.h"

//------------------------------------------------------------------------------

struct libqu_wave
{
    int16_t channel_count;
    int64_t sample_count;
    int64_t sample_rate;
    int16_t *samples;
};

//------------------------------------------------------------------------------

struct libqu_wave *libqu_wave_create(int16_t channels, int64_t samples, int64_t sample_rate);
struct libqu_wave *libqu_wave_load(struct libqu_file *file);
void libqu_wave_destroy(struct libqu_wave *wave);

//------------------------------------------------------------------------------

#endif // LIBQU_WAVE_H_INC
