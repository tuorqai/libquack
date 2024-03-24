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

#ifndef LIBQU_AUDIO_H_INC
#define LIBQU_AUDIO_H_INC

//------------------------------------------------------------------------------

#include "fs.h"
#include "platform.h"

//------------------------------------------------------------------------------

#define LIBQU_MUSIC_BUFFERS             8
#define LIBQU_MUSIC_BUFFER_LENGTH       4096

//------------------------------------------------------------------------------

//
// Audio format
//
struct libqu_audio_format
{
    int channels;                       // number of channels (1 or 2)
    int rate;                           // sample rate (e.g. 44100)
};

//
// Wave object, stores static sample data
//
struct libqu_wave
{
    int refcount;                       // reference count
    struct libqu_audio_format format;   // audio format
    int16_t *buffer;                    // sample data
    size_t size;                        // total number of samples
};

//
// Sound file object, used to dynamically read from audio file
//
struct libqu_sndfile
{
    int type;                           // type index
    struct libqu_file *file;            // file handle
    struct libqu_audio_format format;   // audio format
    unsigned long samples_per_channel;  // number of samples per channel
    void *context;                      // arbitrary context information
};

//
// Sound object
//
struct libqu_sound
{
    struct libqu_wave *wave;            // wave handle
    intptr_t priv[4];                   // arbitrary private information
};

//
// Music object
//
struct libqu_music
{
    struct libqu_file *file;            // file handle
    struct libqu_sndfile *sndfile;      // sound file reader
    pl_thread *thread;                  // music thread
    pl_mutex *mutex;                    // critical section
    int16_t *buffers[LIBQU_MUSIC_BUFFERS]; // sample buffers
    int current_buffer;                 // buffer index to be processed
    int loop;                           // loop value
    int released;                       // thread is stopped
    int seek;                           // need to rewind
    intptr_t priv[4];                   // arbitrary private information
};

struct libqu_audio_params
{
    int unused;
};

struct libqu_audio_impl
{
    bool (*check_if_available)(void);
    bool (*initialize)(struct libqu_audio_params const *params);
    void (*terminate)(void);
    void (*set_master_volume)(float volume);

    int (*create_sound)(struct libqu_sound *sound);
    void (*destroy_sound)(struct libqu_sound *sound);
    qu_playback_state (*get_sound_state)(struct libqu_sound *sound);
    void (*set_sound_loop)(struct libqu_sound *sound, int loop);
    void (*set_sound_state)(struct libqu_sound *sound, qu_playback_state state);

    int (*create_music)(struct libqu_music *music);
    void (*destroy_music)(struct libqu_music *music);
    int (*enqueue_music_buffer)(struct libqu_music *music, int16_t const *buffer, size_t samples);
    int (*dequeue_played_music_buffers)(struct libqu_music *music);
    void (*dequeue_all_music_buffers)(struct libqu_music *music);
    qu_playback_state (*get_music_state)(struct libqu_music *music);
    void (*set_music_state)(struct libqu_music *music, qu_playback_state state);
};

//------------------------------------------------------------------------------

extern struct libqu_audio_impl const libqu_audio_null_impl;

#ifdef QU_USE_OPENAL
extern struct libqu_audio_impl const libqu_audio_openal_impl;
#endif

//------------------------------------------------------------------------------

void libqu_audio_initialize(struct libqu_audio_params const *params);
void libqu_audio_terminate(void);

void libqu_audio_set_master_volume(float volume);

struct libqu_sound *libqu_audio_load_sound(struct libqu_wave *wave);
void libqu_audio_destroy_sound(struct libqu_sound *sound);
qu_playback_state libqu_audio_get_sound_state(struct libqu_sound *sound);
void libqu_audio_set_sound_loop(struct libqu_sound *sound, int loop);
void libqu_audio_set_sound_state(struct libqu_sound *sound, qu_playback_state state);

struct libqu_music *libqu_audio_open_music(struct libqu_file *file);
void libqu_audio_close_music(struct libqu_music *music);
qu_playback_state libqu_audio_get_music_state(struct libqu_music *music);
void libqu_audio_set_music_loop(struct libqu_music *music, int loop);
void libqu_audio_set_music_state(struct libqu_music *music, qu_playback_state state);
double libqu_audio_get_music_duration(struct libqu_music *music);
double libqu_audio_get_music_position(struct libqu_music *music);
void libqu_audio_seek_music(struct libqu_music *music, double sec);

struct libqu_wave *libqu_wave_create(int16_t channels, int64_t samples, int64_t sample_rate);
struct libqu_wave *libqu_wave_load(struct libqu_file *file);
void libqu_wave_destroy(struct libqu_wave *wave);

struct libqu_sndfile *libqu_sndfile_open(struct libqu_file *file);
void libqu_sndfile_close(struct libqu_sndfile *sndfile);
int64_t libqu_sndfile_read(struct libqu_sndfile *sndfile, int16_t *samples, int64_t max_samples);
int64_t libqu_sndfile_seek(struct libqu_sndfile *sndfile, int64_t sample_offset);
int64_t libqu_sndfile_tell(struct libqu_sndfile *sndfile);

//------------------------------------------------------------------------------

#endif // LIBQU_AUDIO_H_INC
