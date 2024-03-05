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

#ifdef _WIN32

//------------------------------------------------------------------------------

#if !defined(_UNICODE)
#define _UNICODE
#endif

#if !defined(UNICODE)
#define UNICODE
#endif

//------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "platform.h"

//------------------------------------------------------------------------------

#define THREAD_FLAG_WAIT            0x01
#define THREAD_FLAG_DETACHED        0x02

//------------------------------------------------------------------------------

struct pl_thread
{
    DWORD id;
    HANDLE handle;
    CRITICAL_SECTION cs;
    UINT flags;
    char const *name;
    void *(*func)(void *);
    void *arg;
    void *retval;
};

struct pl_mutex
{
    CRITICAL_SECTION cs;
};

//------------------------------------------------------------------------------
// Memory

void *pl_malloc(size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

void *pl_calloc(size_t count, size_t size)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void *pl_realloc(void *data, size_t size)
{
    return HeapReAlloc(GetProcessHeap(), 0, data, size);
}

void pl_free(void *data)
{
    HeapFree(GetProcessHeap(), 0, data);
}

//------------------------------------------------------------------------------
// Clock

uint32_t pl_get_ticks_mediump(void)
{
    return GetTickCount();
}

uint64_t pl_get_ticks_highp(void)
{
    static LARGE_INTEGER frequency = { 0 };

    if (frequency.QuadPart == 0) {
        if (!QueryPerformanceFrequency(&frequency)) {
            frequency.QuadPart = -1;
        }
    }

    if (frequency.QuadPart == -1) {
        return GetTickCount() * 1000000;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000000000) / frequency.QuadPart;
}

//------------------------------------------------------------------------------
// Threads

static void thread_end(pl_thread *thread, void *retval)
{
    // If the thread is detached, then only its info struct should be freed.
    if (!(thread->flags & THREAD_FLAG_DETACHED)) {
        EnterCriticalSection(&thread->cs);

        // If the thread is being waited in wait_thread(), do not release
        // its resources.
        // Also don't forget to set retval.
        if (thread->flags & THREAD_FLAG_WAIT) {
            thread->retval = retval;
            LeaveCriticalSection(&thread->cs);
            return;
        }

        LeaveCriticalSection(&thread->cs);

        CloseHandle(thread->handle);
        DeleteCriticalSection(&thread->cs);
    }

    pl_free(thread);
}

static DWORD WINAPI thread_main(LPVOID param)
{
    pl_thread *thread = (pl_thread *) param;
    void *retval = thread->func(thread->arg);

    thread_end(thread, retval);

    return 0;
}

pl_thread *pl_create_thread(char const *name, void *(*func)(void *), void *arg)
{
    pl_thread *thread = pl_calloc(1, sizeof(*thread));

    if (!thread) {
        return NULL;
    }

    thread->name = name;
    thread->func = func;
    thread->arg = arg;

    InitializeCriticalSection(&thread->cs);
    thread->flags = 0;

    thread->handle = CreateThread(NULL, 0, thread_main, thread, 0, &thread->id);

    if (!thread->handle) {
        pl_free(thread);
        return NULL;
    }

    return thread;
}

void pl_detach_thread(pl_thread *thread)
{
    EnterCriticalSection(&thread->cs);
    thread->flags |= THREAD_FLAG_DETACHED;
    LeaveCriticalSection(&thread->cs);

    CloseHandle(thread->handle);
    DeleteCriticalSection(&thread->cs);
}

void *pl_wait_thread(pl_thread *thread)
{
    EnterCriticalSection(&thread->cs);

    if (thread->flags & THREAD_FLAG_DETACHED) {
        LeaveCriticalSection(&thread->cs);
        return NULL;
    }

    thread->flags |= THREAD_FLAG_WAIT;
    LeaveCriticalSection(&thread->cs);

    WaitForSingleObject(thread->handle, INFINITE);

    // Save retval before the thread struct is freed.
    void *retval = thread->retval;

    thread->flags &= ~THREAD_FLAG_WAIT;
    thread_end(thread, NULL);

    return retval;
}

pl_mutex *pl_create_mutex(void)
{
    pl_mutex *mutex = pl_calloc(1, sizeof(*mutex));

    if (!mutex) {
        return NULL;
    }

    InitializeCriticalSection(&mutex->cs);

    return mutex;
}

void pl_destroy_mutex(pl_mutex *mutex)
{
    DeleteCriticalSection(&mutex->cs);
    pl_free(mutex);
}

void pl_lock_mutex(pl_mutex *mutex)
{
    EnterCriticalSection(&mutex->cs);
}

void pl_unlock_mutex(pl_mutex *mutex)
{
    LeaveCriticalSection(&mutex->cs);
}

void pl_sleep(uint32_t milliseconds)
{
    Sleep(milliseconds);
}

//------------------------------------------------------------------------------
// Dynamic loading libraries

static wchar_t *conv_str(char const *str)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    wchar_t *str_w = pl_calloc(size, sizeof(*str_w));

    if (!str_w) {
        return NULL;
    }

    int result = MultiByteToWideChar(CP_UTF8, 0, str, -1, str_w, size);

    if (result == 0) {
        pl_free(str_w);
        return NULL;
    }

    return str_w;
}

void *pl_open_dll(char const *path)
{
    wchar_t *wpath = conv_str(path);

    if (!wpath) {
        return NULL;
    }

    HMODULE library = LoadLibraryExW(wpath, NULL, 0);
    pl_free(wpath);

    if (!library) {
        return NULL;
    }

    return library;
}

void pl_close_dll(void *library)
{
    if (library) {
        FreeLibrary(library);
    }
}

void *pl_get_dll_proc(void *library, char const *name)
{
    if (library) {
        return GetProcAddress(library, name);
    }

    return NULL;
}

//------------------------------------------------------------------------------
// Date & Time

void pl_get_date_time(pl_date_time *date_time)
{
    SYSTEMTIME localTime = { 0 };

    GetLocalTime(&localTime);

    date_time->year = localTime.wYear;
    date_time->month = localTime.wMonth;
    date_time->day = localTime.wDay;
    date_time->weekday = (localTime.wDayOfWeek == 0) ? 7 : localTime.wDayOfWeek;

    date_time->hours = localTime.wHour;
    date_time->minutes = localTime.wMinute;
    date_time->seconds = localTime.wSecond;
    date_time->milliseconds = localTime.wMilliseconds;
}

//------------------------------------------------------------------------------

#endif // _WIN32
