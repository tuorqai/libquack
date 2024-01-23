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

#include "libqu/libqu.h"

//------------------------------------------------------------------------------

#ifndef LIBQU_FS_H_INC
#define LIBQU_FS_H_INC

//------------------------------------------------------------------------------

#define LIBQU_FILE_NAME_LENGTH      (256)

//------------------------------------------------------------------------------

enum libqu_file_source
{
    LIBQU_FILE_SOURCE_STD,
    LIBQU_FILE_SOURCE_MEMBUF,
    LIBQU_TOTAL_FILE_SOURCES,
};

struct libqu_file
{
    enum libqu_file_source source;

    char name[QU_FILE_NAME_LENGTH];
    size_t size;

    void *context;
};

//------------------------------------------------------------------------------

struct libqu_file *libqu_fopen(char const *path);
struct libqu_file *libqu_fopen_buffer(void const *data, size_t size);
void libqu_fclose(struct libqu_file *file);

int64_t libqu_fread(void *buffer, size_t size, struct libqu_file *file);
int64_t libqu_ftell(struct libqu_file *file);
int64_t libqu_fseek(struct libqu_file *file, int64_t offset, int origin);

//------------------------------------------------------------------------------

#endif // LIBQU_FS_H_INC

