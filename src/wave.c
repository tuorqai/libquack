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

#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "wave.h"

//------------------------------------------------------------------------------

#if 0

struct riff
{
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    int64_t data_start;
    int64_t data_end;
};

static int wave_loader_open(struct libqu_file *file)
{
    char chunk_id[4];
    uint32_t chunk_size;

    if ((libqu_fread(chunk_id, 4, file) < 4) || memcmp("RIFF", chunk_id, 4)) {
        return -1;
    }

    if (libqu_fread(&chunk_size, 4, file) < 4) {
        return -1;
    }

    char format[4];

    if ((libqu_fread(format, 4, file) < 4) || memcmp("WAVE", format, 4)) {
        return -1;
    }

    struct riff *riff = pl_calloc(1, sizeof(*riff));

    while (true) {
        char subchunk_id[4];
        uint32_t subchunk_size;

        if (libqu_fread(subchunk_id, 4, file) < 4) {
            return false;
        }

        if (libqu_fread(&subchunk_size, 4, file) < 4) {
            return false;
        }

        int64_t subchunk_start = libqu_ftell(file);

        if (memcmp("fmt ", subchunk_id, 4) == 0) {
            if (libqu_fread(riff, 16, file) < 16) {
                return -1;
            }

            loader->num_channels = (int16_t) riff->num_channels;
            loader->sample_rate = (int64_t) riff->sample_rate;
        } else if (memcmp("data", subchunk_id, 4) == 0) {
            loader->num_samples = subchunk_size / (riff->bits_per_sample / 8);
            riff->data_start = libqu_ftell(file);
            riff->data_end = riff->data_start + subchunk_size;
            break;
        }

        if (libqu_fseek(file, subchunk_start + subchunk_size, SEEK_SET) == -1) {
            return false;
        }
    }

    loader->context = riff;

    libqu_fseek(file, riff->data_start, SEEK_SET);

    return 0;
}

#endif

//------------------------------------------------------------------------------

struct libqu_wave *libqu_wave_create(int16_t channels, int64_t samples, int64_t sample_rate)
{
    struct libqu_wave *wave = pl_calloc(1, sizeof(*wave));

    wave->samples = pl_malloc(sizeof(*wave->samples) * channels * samples);
    wave->channel_count = channels;
    wave->sample_count = samples;
    wave->sample_rate = sample_rate;

    return wave;
}

struct libqu_wave *libqu_wave_load(struct libqu_file *file)
{
    return NULL;
}

void libqu_wave_destroy(struct libqu_wave *wave)
{
    pl_free(wave->samples);
    pl_free(wave);
}
