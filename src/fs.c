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

#include <assert.h>
#include <stdio.h>
#include "fs.h"
#include "log.h"

//------------------------------------------------------------------------------

static void *fs_std_open(char const *path)
{
    return fopen(path, "rb");
}

static void fs_std_close(void *context)
{
    fclose((FILE *) context);
}

static int64_t fs_std_read(void *dst, size_t size, void *context)
{
    FILE *stream = (FILE *) context;
    
    return fread(dst, 1, size, stream);
}

static int64_t fs_std_tell(void *context)
{
    FILE *stream = (FILE *) context;
    
    return ftell(stream);
}

static int64_t fs_std_seek(void *context, int64_t offset, int origin)
{
    FILE *stream = (FILE *) context;
    
    return fseek(stream, offset, origin);
}

//------------------------------------------------------------------------------

struct fs_membuf
{
    void const *data;
    size_t size;
    int64_t offset;
};

static void fs_membuf_close(void *context)
{
    // Actual data remains intact; this only frees fs_membuf struct.
    pl_free(context);
}

static int64_t fs_membuf_read(void *dst, size_t size, void *context)
{
    struct fs_membuf *buffer = context;
    
    // Count of bytes that should be read (copied).
    int64_t bytes_to_read = size;

    // Don't overrun the buffer.
    if ((buffer->offset + size) > buffer->size) {
        bytes_to_read = buffer->size - buffer->offset;
    }

    // Cast data pointer to byte pointer for pointer arithmetic.
    unsigned char const *data_bytes = buffer->data;

    // Copy to the destination buffer.
    memcpy(dst, data_bytes + buffer->offset, bytes_to_read);
    
    // Advance offset.
    buffer->offset += bytes_to_read;

    return bytes_to_read;
}

static int64_t fs_membuf_tell(void *context)
{
    return ((struct fs_membuf *) context)->offset;
}

static int64_t fs_membuf_seek(void *context, int64_t offset, int origin)
{
    struct fs_membuf *buffer = context;
    
    // Absolute offset.
    int64_t abs_offset = offset;
    
    if (origin == SEEK_CUR) {
        abs_offset = buffer->offset + offset;
    } else if (origin == SEEK_END) {
        abs_offset = buffer->size + offset;
    }
    
    // Return -1 if offset is outside of bounds.
    if (abs_offset < 0 || abs_offset > (int64_t) buffer->size) {
        return -1;
    }
    
    buffer->offset = abs_offset;

    return 0;
}

//------------------------------------------------------------------------------

struct fs_callbacks
{
    void *(*open)(char const *path);
    void (*close)(void *handle);
    int64_t (*read)(void *dst, size_t size, void *handle);
    int64_t (*tell)(void *handle);
    int64_t (*seek)(void *handle, int64_t offset, int whence);
};

static struct fs_callbacks const fs_callbacks[LIBQU_TOTAL_FILE_SOURCES] = {
    [LIBQU_FILE_SOURCE_STD] = {
        .open = fs_std_open,
        .close = fs_std_close,
        .read = fs_std_read,
        .tell = fs_std_tell,
        .seek = fs_std_seek,
    },
    [LIBQU_FILE_SOURCE_MEMBUF] = {
        .open = NULL,
        .close = fs_membuf_close,
        .read = fs_membuf_read,
        .tell = fs_membuf_tell,
        .seek = fs_membuf_seek,
    },
};

//------------------------------------------------------------------------------

struct libqu_file *libqu_fopen(char const *path)
{
    int source = -1;
    void *context = NULL;

    for (int i = 0; i < LIBQU_TOTAL_FILE_SOURCES; i++) {
        if (!fs_callbacks[i].open) {
            continue;
        }

        context = fs_callbacks[i].open(path);

        if (context) {
            source = i;
            break;
        }
    }

    if (source == -1 || !context) {
        return NULL;
    }

    struct libqu_file *file = pl_malloc(sizeof(*file));

    file->source = source;
    file->context = context;

    strncpy(file->name, path, sizeof(file->name) - 1);

    fs_callbacks[file->source].seek(file->context, 0, SEEK_END);
    file->size = (size_t) fs_callbacks[file->source].tell(file->context);

    fs_callbacks[file->source].seek(file->context, 0, SEEK_SET);

    LIBQU_LOGD("Opened file %s.\n", file->name);

    return file;
}

struct libqu_file *libqu_fopen_buffer(void const *data, size_t size)
{
    struct fs_membuf *buffer = pl_malloc(sizeof(*buffer));

    buffer->data = data;
    buffer->size = size;
    buffer->offset = 0;

    struct libqu_file *file = pl_malloc(sizeof(*file));
    
    file->source = LIBQU_FILE_SOURCE_MEMBUF;
    file->context = buffer;
    sprintf(file->name, "%p", data);
    file->size = size;

    if (size < 1024) {
        LIBQU_LOGD("Opened memory buffer %p (%d bytes).\n", data, size);
    } else {
        LIBQU_LOGD("Opened memory buffer %p (%dK).\n", data, size / 1024);
    }
    
    return file;
}

void libqu_fclose(struct libqu_file *file)
{
    fs_callbacks[file->source].close(file->context);
    LIBQU_LOGD("Closed file %s.\n", file->name);

    pl_free(file);
}

int64_t libqu_fread(void *buffer, size_t size, struct libqu_file *file)
{
    assert(file->source >= 0 && file->source < LIBQU_TOTAL_FILE_SOURCES);
    return fs_callbacks[file->source].read(buffer, size, file->context);
}

int64_t libqu_ftell(struct libqu_file *file)
{
    assert(file->source >= 0 && file->source < LIBQU_TOTAL_FILE_SOURCES);
    return fs_callbacks[file->source].tell(file->context);
}

int64_t libqu_fseek(struct libqu_file *file, int64_t offset, int origin)
{
    assert(file->source >= 0 && file->source < LIBQU_TOTAL_FILE_SOURCES);
    return fs_callbacks[file->source].seek(file->context, offset, origin);
}
