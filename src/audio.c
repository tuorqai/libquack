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

#include <string.h>
#include <vorbis/vorbisfile.h>
#include "audio.h"
#include "log.h"
#include "platform.h"

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

void libqu_audio_set_master_volume(float volume)
{
    priv.impl->set_master_volume(volume);
}

struct libqu_sound *libqu_audio_load_sound(struct libqu_wave *wave)
{
    struct libqu_sound *sound = pl_calloc(1, sizeof(*sound));

    sound->wave = wave;

    if (priv.impl->load_sound(sound) == 0) {
        return sound;
    }

    libqu_wave_destroy(sound->wave);
    pl_free(sound);

    return NULL;
}

void libqu_audio_delete_sound(struct libqu_sound *sound)
{
    priv.impl->delete_sound(sound);

    libqu_wave_destroy(sound->wave);
    pl_free(sound);
}

qu_handle libqu_audio_play_sound(struct libqu_sound *sound, int loop)
{
    return priv.impl->play_sound(sound, loop);
}

void libqu_audio_pause_voice(qu_handle voice_id)
{
    priv.impl->pause_voice(voice_id);
}

void libqu_audio_unpause_voice(qu_handle voice_id)
{
    priv.impl->unpause_voice(voice_id);
}

void libqu_audio_stop_voice(qu_handle voice_id)
{
    priv.impl->stop_voice(voice_id);
}

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

//------------------------------------------------------------------------------

struct wav_chunk
{
    char id[4];
    uint32_t size;
    char format[4];
};

struct wav_subchunk
{
    char id[4];
    uint32_t size;
};

struct wav_info
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

static void *_wav_test(struct libqu_file *file)
{
    struct wav_chunk chunk;

    if (libqu_fread(&chunk, sizeof(chunk), file) < (int) sizeof(chunk)) {
        return NULL;
    }

    if (memcmp("RIFF", chunk.id, 4)) {
        return NULL;
    }

    if (memcmp("WAVE", chunk.format, 4)) {
        return NULL;
    }

    return pl_calloc(1, sizeof(struct wav_info));
}

static int _wav_open(struct libqu_sndfile *sndfile)
{
    struct wav_info *info = sndfile->context;
    int state = 0;

    while (true) {
        struct wav_subchunk sub;

        if (libqu_fread(&sub, sizeof(sub), sndfile->file) < (int) sizeof(sub)) {
            LIBQU_LOGE(
                "Failed to open WAV file \"%s\": can't read subchunk header.\n",
                sndfile->file->name
            );
            return -1;
        }

        int64_t start = libqu_ftell(sndfile->file);

        if (memcmp("fmt ", sub.id, 4) == 0) {
            if (libqu_fread(info, 16, sndfile->file) < 16) {
                LIBQU_LOGE(
                    "Failed to open WAV file \"%s\": fmt subchunk unreadable.\n",
                    sndfile->file->name
                );
                return -1;
            }

            sndfile->channel_count = info->num_channels;
            sndfile->sample_rate = info->sample_rate;

            state |= 0x01;
        } else if (memcmp("data", sub.id, 4) == 0) {
            sndfile->sample_count = sub.size / (info->bits_per_sample / 8);
            
            info->data_start = libqu_ftell(sndfile->file);
            info->data_end = info->data_start + sub.size;

            state |= 0x02;
        }

        if (libqu_fseek(sndfile->file, start + sub.size, SEEK_SET) == -1) {
            LIBQU_LOGE(
                "Failed to open WAV file \"%s\": unexpected end of file.\n",
                sndfile->file->name
            );
            return -1;
        }
    }

    if (!(state & 0x01)) {
        LIBQU_LOGE("wav: fmt subchunk not found\n");
        return -1;
    }

    if (!(state & 0x02)) {
        LIBQU_LOGE("wav: data subchunk not found\n");
        return -1;
    }

    libqu_fseek(sndfile->file, info->data_start, SEEK_SET);

    return 0;
}

static void _wav_close(struct libqu_sndfile *sndfile)
{
    pl_free(sndfile->context);
}

static int64_t _wav_read(struct libqu_sndfile *sndfile, int16_t *samples, int64_t max_samples)
{
    struct wav_info *info = sndfile->context;

    int16_t bytes_per_sample = info->bits_per_sample / 8;
    int64_t samples_read = 0;

    while (samples_read < max_samples) {
        int64_t position = libqu_ftell(sndfile->file);

        if (position >= info->data_end) {
            break;
        }

        unsigned char bytes[4];

        if (libqu_fread(bytes, bytes_per_sample, sndfile->file) != bytes_per_sample) {
            break;
        }

        switch (bytes_per_sample) {
        case 1:
            /* Unsigned 8-bit PCM */
            *samples++ = (((short) bytes[0]) - 128) << 8;
            break;
        case 2:
            /* Signed 16-bit PCM */
            *samples++ = (bytes[1] << 8) | bytes[0];
            break;
        case 3:
            /* Signed 24-bit PCM */
            *samples++ = (bytes[2] << 8) | bytes[1];
            break;
        case 4:
            /* Signed 32-bit PCM */
            *samples++ = (bytes[3] << 8) | bytes[2];
            break;
        }

        samples_read++;
    }

    return samples_read;
}

static int64_t _wav_seek(struct libqu_sndfile *sndfile, int64_t sample_offset)
{
    struct wav_info *info = sndfile->context;

    return libqu_fseek(sndfile->file, info->data_start +
        (sample_offset * (info->bits_per_sample / 8)), SEEK_SET);
}

//------------------------------------------------------------------------------

static char const *_vorbis_err_str(int status)
{
    if (status >= 0) {
        return "(no error)";
    }

    switch (status) {
    case OV_HOLE:           return "OV_HOLE";
    case OV_EREAD:          return "OV_EREAD";
    case OV_EFAULT:         return "OV_EFAULT";
    case OV_EINVAL:         return "OV_EINVAL";
    case OV_ENOTVORBIS:     return "OV_ENOTVORBIS";
    case OV_EBADHEADER:     return "OV_EBADHEADER";
    case OV_EVERSION:       return "OV_EVERSION";
    case OV_EBADLINK:       return "OV_EBADLINK";
    }

    return "(unknown error)";
}

static size_t _vorbis_io_read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    return libqu_fread(ptr, size * nmemb, datasource);
}

static int _vorbis_io_seek(void *datasource, ogg_int64_t offset, int whence)
{
    return (int) libqu_fseek(datasource, offset, whence);
}

static long _vorbis_io_tell(void *datasource)
{
    return (long) libqu_ftell(datasource);
}

static ov_callbacks const _vorbis_callbacks = {
    .read_func = _vorbis_io_read,
    .seek_func = _vorbis_io_seek,
    .close_func = NULL,
    .tell_func = _vorbis_io_tell,
};

static void *_vorbis_test(struct libqu_file *file)
{
    OggVorbis_File *vf = pl_calloc(1, sizeof(*vf));

    if (!vf) {
        return NULL;
    }

    int status = ov_test_callbacks(file, vf, NULL, 0, _vorbis_callbacks);

    if (status != 0) {
        pl_free(vf);
        return NULL;
    }

    return vf;
}

static int _vorbis_open(struct libqu_sndfile *sndfile)
{
    OggVorbis_File *vf = sndfile->context;

    int status = ov_test_open(vf);

    if (status != 0) {
        LIBQU_LOGE("Failed to open Vorbis audio from \"%s\": %s\n",
            sndfile->file->name, _vorbis_err_str(status));
        return -1;
    }

    vorbis_info *info = ov_info(vf, -1);
    ogg_int64_t samples_per_channel = ov_pcm_total(vf, -1);

    sndfile->channel_count = info->channels;
    sndfile->sample_count = samples_per_channel * info->channels;
    sndfile->sample_rate = info->rate;

    return 0;
}

static void _vorbis_close(struct libqu_sndfile *sndfile)
{
    ov_clear(sndfile->context);
    pl_free(sndfile->context);
}

static int64_t _vorbis_read(struct libqu_sndfile *sndfile, int16_t *samples, int64_t max_samples)
{
    OggVorbis_File *vf = sndfile->context;
    long samples_read = 0;

    while (samples_read < max_samples) {
        int bytes_left = (max_samples - samples_read) / sizeof(int16_t);
        long bytes_read = ov_read(vf, (char *) samples, bytes_left, 0, 2, 1, NULL);

        // End of file.
        if (bytes_read == 0) {
            break;
        }

        // Some error occured.
        // ...but it still works as intended, right?
        // Couldn't care less if it still reports some errors.
        // I'll just ignore them.
        if (bytes_read < 0) {
#if 0
            LIBQU_LOGE("Failed to read Ogg Vorbis from file %s. Reason: %s\n",
                sndfile->file->name, _vorbis_err_str(bytes_read));
#endif
            break;
        }

        samples_read += bytes_read / sizeof(int16_t);
        samples += bytes_read / sizeof(int16_t);
    }

    return samples_read;
}

static int64_t _vorbis_seek(struct libqu_sndfile *sndfile, int64_t sample_offset)
{
    OggVorbis_File *vf = sndfile->context;

    return ov_pcm_seek(vf, sample_offset / sndfile->channel_count);
}

//------------------------------------------------------------------------------

enum
{
    SNDFILE_FORMAT_WAV,
    SNDFILE_FORMAT_VORBIS,
    TOTAL_SNDFILE_FORMATS,
};

struct sndfile_callbacks
{
    void *(*test)(struct libqu_file *file);
    int (*open)(struct libqu_sndfile *sndfile);
    void (*close)(struct libqu_sndfile *sndfile);
    int64_t (*read)(struct libqu_sndfile *sndfile, int16_t *dst, int64_t max_samples);
    int64_t (*seek)(struct libqu_sndfile *sndfile, int64_t sample_offset);
};

static struct sndfile_callbacks const sndfile_callbacks[] = {
    {
        _wav_test,
        _wav_open,
        _wav_close,
        _wav_read,
        _wav_seek,
    },
    {
        _vorbis_test,
        _vorbis_open,
        _vorbis_close,
        _vorbis_read,
        _vorbis_seek,
    },
};

static char const * const sndfile_format_names[] = {
    "Wave",
    "Vorbis",
};

struct libqu_sndfile *libqu_sndfile_open(struct libqu_file *file)
{
    int format = -1;
    void *context = NULL;

    for (format = 0; format < TOTAL_SNDFILE_FORMATS; format++) {
        libqu_fseek(file, 0, SEEK_SET);
        context = sndfile_callbacks[format].test(file);

        if (context) {
            break;
        }
    }

    if (!context) {
        LIBQU_LOGE("Can't open \"%s\", format not recognized.\n", file->name);
        return NULL;
    }

    struct libqu_sndfile *sndfile = pl_calloc(1, sizeof(*sndfile));

    if (!sndfile) {
        return NULL;
    }

    sndfile->format = format;
    sndfile->file = file;
    sndfile->context = context;

    int status = sndfile_callbacks[format].open(sndfile);

    if (status == -1) {
        sndfile_callbacks[format].close(sndfile);
        pl_free(sndfile);

        return NULL;
    }

    LIBQU_LOGI("File \"%s\" is recognized as %s.\n",
        file->name, sndfile_format_names[format]);

    return sndfile;
}

void libqu_sndfile_close(struct libqu_sndfile *sndfile)
{
    sndfile_callbacks[sndfile->format].close(sndfile);
    pl_free(sndfile);
}

int64_t libqu_sndfile_read(struct libqu_sndfile *sndfile, int16_t *samples, int64_t max_samples)
{
    return sndfile_callbacks[sndfile->format].read(sndfile, samples, max_samples);
}

int64_t libqu_sndfile_seek(struct libqu_sndfile *sndfile, int64_t sample_offset)
{
    return sndfile_callbacks[sndfile->format].seek(sndfile, sample_offset);
}
