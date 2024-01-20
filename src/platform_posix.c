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

#ifdef __unix__

//------------------------------------------------------------------------------

#ifdef __linux__
    #define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

#include "platform.h"

//------------------------------------------------------------------------------

#define THREAD_NAME_LENGTH          256

//------------------------------------------------------------------------------

struct pl_thread
{
    pthread_t id;
    char name[THREAD_NAME_LENGTH];
    void *(*func)(void *);
    void *arg;
};

struct pl_mutex
{
    pthread_mutex_t id;
};

//------------------------------------------------------------------------------
// Memory

void *pl_malloc(size_t size)
{
    return malloc(size);
}

void *pl_calloc(size_t count, size_t size)
{
    return calloc(count, size);
}

void *pl_realloc(void *data, size_t size)
{
    return realloc(data, size);
}

void pl_free(void *data)
{
    free(data);
}

//------------------------------------------------------------------------------
// Clock

uint32_t pl_get_ticks_mediump(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

uint64_t pl_get_ticks_highp(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000000000) + ts.tv_nsec;
}

//------------------------------------------------------------------------------
// Threads

static void *thread_main(void *thread_ptr)
{
    pl_thread *thread = thread_ptr;
    void *retval = thread->func(thread->arg);
    pl_free(thread);

    return retval;
}

pl_thread *pl_create_thread(char const *name, void *(*func)(void *), void *arg)
{
    pl_thread *thread = pl_calloc(1, sizeof(*thread));

    if (!thread) {
        return NULL;
    }

    if (!name || name[0] == '\0') {
        strncpy(thread->name, "(unnamed)", THREAD_NAME_LENGTH - 1);
    } else {
        strncpy(thread->name, name, THREAD_NAME_LENGTH - 1);
    }

    thread->func = func;
    thread->arg = arg;

    int error = pthread_create(&thread->id, NULL, thread_main, thread);

    if (error) {
        pl_free(thread);
        return NULL;
    }

#ifdef __linux__
    pthread_setname_np(thread->id, thread->name);
#endif

    return thread;
}

void pl_detach_thread(pl_thread *thread)
{
    pthread_detach(thread->id);
}

void *pl_wait_thread(pl_thread *thread)
{
    void *retval;
    pthread_join(thread->id, &retval);

    return retval;
}

pl_mutex *pl_create_mutex(void)
{
    pl_mutex *mutex = pl_calloc(1, sizeof(pl_mutex));

    if (!mutex) {
        return NULL;
    }

    int error = pthread_mutex_init(&mutex->id, NULL);

    if (error) {
        pl_free(mutex);
        return NULL;
    }

    return mutex;
}

void pl_destroy_mutex(pl_mutex *mutex)
{
    if (!mutex) {
        return;
    }

    pthread_mutex_destroy(&mutex->id);
    pl_free(mutex);
}

void pl_lock_mutex(pl_mutex *mutex)
{
    pthread_mutex_lock(&mutex->id);
}

void pl_unlock_mutex(pl_mutex *mutex)
{
    pthread_mutex_unlock(&mutex->id);
}

void pl_sleep(uint32_t milliseconds)
{
    struct timespec ts = {
        .tv_sec = milliseconds / 1000,
        .tv_nsec = (milliseconds % 1000) * 1000000,
    };

    while ((nanosleep(&ts, &ts) == -1) && (errno == EINTR)) {
        // Wait, do nothing.
    }
}

//------------------------------------------------------------------------------
// Dynamic loading libraries

void *pl_open_dll(char const *path)
{
    return dlopen(path, RTLD_LAZY);
}

void pl_close_dll(void *library)
{
    if (library) {
        dlclose(library);
    }
}

void *pl_get_dll_proc(void *library, char const *name)
{
    if (library) {
        return dlsym(library, name);
    }

    return NULL;
}

//------------------------------------------------------------------------------
// Date & Time

void pl_get_date_time(pl_date_time *date_time)
{
    struct timeval tv;
    struct tm tm;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    if (!localtime_r(&tv.tv_sec, &tm)) {
        return;
    }

    date_time->year = 1900 + tm.tm_year;
    date_time->month = 1 + tm.tm_mon;
    date_time->day = tm.tm_mday;
    date_time->weekday = (tm.tm_wday == 0) ? 7 : tm.tm_wday;

    date_time->hours = tm.tm_hour;
    date_time->minutes = tm.tm_min;
    date_time->seconds = tm.tm_sec;
    date_time->milliseconds = tv.tv_usec / 1000;
}

//------------------------------------------------------------------------------

#endif // __unix__

