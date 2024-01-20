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

#ifndef LIBQU_PLATFORM_H_INC
#define LIBQU_PLATFORM_H_INC

//------------------------------------------------------------------------------

#include "libqu/libqu.h"

//------------------------------------------------------------------------------

typedef struct pl_thread pl_thread;
typedef struct pl_mutex pl_mutex;

typedef struct pl_date_time
{
    int year;
    int month;
    int day;
    int weekday;
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
} pl_date_time;

//------------------------------------------------------------------------------

void *pl_malloc(size_t size);
void *pl_calloc(size_t count, size_t size);
void *pl_realloc(void *data, size_t size);
void pl_free(void *data);

uint32_t pl_get_ticks_mediump(void);
uint64_t pl_get_ticks_highp(void);

pl_thread *pl_create_thread(char const *name, void *(*func)(void *), void *arg);
void pl_detach_thread(pl_thread *thread);
void *pl_wait_thread(pl_thread *thread);

pl_mutex *pl_create_mutex(void);
void pl_destroy_mutex(pl_mutex *mutex);
void pl_lock_mutex(pl_mutex *mutex);
void pl_unlock_mutex(pl_mutex *mutex);

void pl_sleep(uint32_t milliseconds);

void *pl_open_dll(char const *path);
void pl_close_dll(void *dll);
void *pl_get_dll_proc(void *dll, char const *name);
void pl_get_date_time(pl_date_time *date_time);

//------------------------------------------------------------------------------

#endif // LIBQU_PLATFORM_H_INC

