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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

//------------------------------------------------------------------------------

void libqu_log_puts(enum libqu_log_level level, char const *tag, char const *str)
{
    char *labels[] = {
        [LIBQU_LOG_LEVEL_DEBUG]   = "DBG ",
        [LIBQU_LOG_LEVEL_INFO]    = "INFO",
        [LIBQU_LOG_LEVEL_WARNING] = "WARN",
        [LIBQU_LOG_LEVEL_ERROR]   = "ERR ",
    };

    FILE *outputs[] = {
        [LIBQU_LOG_LEVEL_DEBUG]   = stdout,
        [LIBQU_LOG_LEVEL_INFO]    = stdout,
        [LIBQU_LOG_LEVEL_WARNING] = stderr,
        [LIBQU_LOG_LEVEL_ERROR]   = stderr,
    };

    // Source file path: leave only last segment
    if (tag[0] == '?') {
        for (int i = strlen(tag) - 1; i >= 0; i--) {
            if (tag[i] == '/' || tag[i] == '\\') {
                tag = tag + i + 1;
                break;
            }
        }
    }

    fprintf(outputs[level], "[%s] %s: %s", labels[level], tag, str);
}

void libqu_log_printf(enum libqu_log_level level, char const *tag, char const *fmt, ...)
{
    va_list ap;
    char buffer[256];
    char *heap = NULL;

    va_start(ap, fmt);
    int count = vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    if ((size_t) count >= sizeof(buffer)) {
        heap = malloc(count + 1);

        if (heap) {
            va_start(ap, fmt);
            vsnprintf(heap, count + 1, fmt, ap);
            va_end(ap);
        }
    }

    libqu_log_puts(level, tag, heap ? heap : buffer);
    free(heap);
}

