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

#ifndef LIBQU_LOG_H_INC
#define LIBQU_LOG_H_INC

//------------------------------------------------------------------------------

enum libqu_log_level
{
    LIBQU_LOG_LEVEL_DEBUG,
    LIBQU_LOG_LEVEL_INFO,
    LIBQU_LOG_LEVEL_WARNING,
    LIBQU_LOG_LEVEL_ERROR,
};

//------------------------------------------------------------------------------

void libqu_log_puts(enum libqu_log_level level, char const *tag, char const *str);
void libqu_log_printf(enum libqu_log_level level, char const *tag, char const *fmt, ...);

//------------------------------------------------------------------------------

#ifndef LIBQU_TAG
#define LIBQU_TAG "?"__FILE__
#endif

#ifdef NDEBUG
#define LIBQU_LOGD(...)
#else
#define LIBQU_LOGD(...) \
    libqu_log_printf(LIBQU_LOG_LEVEL_DEBUG, LIBQU_TAG, __VA_ARGS__)
#endif

#define LIBQU_LOGI(...) \
    libqu_log_printf(LIBQU_LOG_LEVEL_INFO, LIBQU_TAG, __VA_ARGS__)

#define LIBQU_LOGW(...) \
    libqu_log_printf(LIBQU_LOG_LEVEL_WARNING, LIBQU_TAG, __VA_ARGS__)

#define LIBQU_LOGE(...) \
    libqu_log_printf(LIBQU_LOG_LEVEL_ERROR, LIBQU_TAG, __VA_ARGS__)

//------------------------------------------------------------------------------

#endif // LIBQU_LOG_H_INC

