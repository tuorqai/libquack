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

#ifndef LIBQU_H_INC
#define LIBQU_H_INC

//------------------------------------------------------------------------------

#include <stdbool.h>

//------------------------------------------------------------------------------

#ifdef QU_SHARED
    #ifdef _WIN32
        #ifdef QU_BUILD
            #define QU_API __declspec(dllexport)
        #else
            #define QU_API __declspec(dllimport)
        #endif
        #define QU_CALL __cdecl
    #else
        #ifdef __GNUC__
            #define QU_API __attribute__((visibility("default")))
        #endif
    #endif
#endif

#ifndef QU_API
    #define QU_API
#endif

#ifndef QU_CALL
    #define QU_CALL
#endif

//------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {
#endif

//------------------------------------------------------------------------------

QU_API void QU_CALL qu_initialize(void);
QU_API void QU_CALL qu_terminate(void);
QU_API bool QU_CALL qu_process(void);
QU_API void QU_CALL qu_present(void);

//------------------------------------------------------------------------------

#if defined(__cplusplus)
} // extern "C"
#endif

//------------------------------------------------------------------------------

#endif // LIBQU_H_INC

