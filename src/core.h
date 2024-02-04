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

#ifndef LIBQU_CORE_H_INC
#define LIBQU_CORE_H_INC

//------------------------------------------------------------------------------

#include "libquack.h"

//------------------------------------------------------------------------------

#define LIBQU_WINDOW_TITLE_LENGTH 256

//------------------------------------------------------------------------------

enum libqu_event_type
{
    LIBQU_EVENT_ACTIVATE,
    LIBQU_EVENT_DEACTIVATE,
    LIBQU_EVENT_KEY_PRESSED,
    LIBQU_EVENT_KEY_RELEASED,
};

struct libqu_core_params
{
    char window_title[LIBQU_WINDOW_TITLE_LENGTH];
    qu_vec2i window_size;
};

struct libqu_core_impl
{
    bool (*check_if_available)(void);
    bool (*initialize)(struct libqu_core_params const *params);
    void (*terminate)(void);
    bool (*process)(void);
    void (*swap)(void);
    bool (*set_window_title)(char const *title);
    bool (*set_window_size)(qu_vec2i size);
    int (*gl_get_version)(void);
    void *(*gl_get_proc_address)(char const *name);
};

struct libqu_event
{
    enum libqu_event_type type;

    union {
        qu_key key;
    } data;
};

//------------------------------------------------------------------------------

extern struct libqu_core_impl const libqu_core_null_impl;

#ifdef QU_USE_X11
extern struct libqu_core_impl const libqu_core_x11_impl;
#endif

//------------------------------------------------------------------------------

void libqu_core_initialize(struct libqu_core_params const *params);
void libqu_core_terminate(void);
bool libqu_core_process(void);
void libqu_core_swap(void);

bool libqu_core_is_window_active(void);

char const *libqu_core_get_window_title(void);
qu_vec2i libqu_core_get_window_size(void);

void libqu_core_set_window_title(char const *title);
void libqu_core_set_window_size(qu_vec2i size);

qu_key_state const *libqu_core_get_keyboard_state(void);

void libqu_core_enqueue_event(struct libqu_event *event);

int libqu_gl_get_version(void);
void *libqu_gl_get_proc_address(char const *name);

//------------------------------------------------------------------------------

#endif // LIBQU_CORE_H_INC

