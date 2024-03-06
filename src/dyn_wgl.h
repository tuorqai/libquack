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

#ifndef LIBQU_DYN_WGL_H_INC
#define LIBQU_DYN_WGL_H_INC

//------------------------------------------------------------------------------

#ifdef _WIN32

//------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "log.h"
#include "platform.h"

//------------------------------------------------------------------------------

typedef BOOL (WINAPI *PFNWGLCOPYCONTEXTPROC)(HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask);
typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTPROC)(HDC hDc);
typedef HGLRC (WINAPI *PFNWGLCREATELAYERCONTEXTPROC)(HDC hDc, int level);
typedef BOOL (WINAPI *PFNWGLDELETECONTEXTPROC)(HGLRC oldContext);
typedef BOOL (WINAPI *PFNWGLDESCRIBELAYERPLANEPROC)(HDC hDc, int pixelFormat, int layerPlane, UINT nBytes, LAYERPLANEDESCRIPTOR *plpd);
typedef HGLRC (WINAPI *PFNWGLGETCURRENTCONTEXTPROC)(void);
typedef HDC (WINAPI *PFNWGLGETCURRENTDCPROC)(void);
typedef int (WINAPI *PFNWGLGETLAYERPALETTEENTRIESPROC)(HDC hdc, int iLayerPlane, int iStart, int cEntries, COLORREF *pcr);
typedef PROC (WINAPI *PFNWGLGETPROCADDRESSPROC)(LPCSTR lpszProc);
typedef BOOL (WINAPI *PFNWGLMAKECURRENTPROC)(HDC hDc, HGLRC newContext);
typedef BOOL (WINAPI *PFNWGLREALIZELAYERPALETTEPROC)(HDC hdc, int iLayerPlane, BOOL bRealize);
typedef int (WINAPI *PFNWGLSETLAYERPALETTEENTRIESPROC)(HDC hdc, int iLayerPlane, int iStart, int cEntries, const COLORREF *pcr);
typedef BOOL (WINAPI *PFNWGLSHARELISTSPROC)(HGLRC hrcSrvShare, HGLRC hrcSrvSource);
typedef BOOL (WINAPI *PFNWGLSWAPLAYERBUFFERSPROC)(HDC hdc, UINT fuFlags);
typedef BOOL (WINAPI *PFNWGLUSEFONTBITMAPSAPROC)(HDC hDC, DWORD first, DWORD count, DWORD listBase);
typedef BOOL (WINAPI *PFNWGLUSEFONTBITMAPSWPROC)(HDC hDC, DWORD first, DWORD count, DWORD listBase);
typedef BOOL (WINAPI *PFNWGLUSEFONTOUTLINESAPROC)(HDC hDC, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);
typedef BOOL (WINAPI *PFNWGLUSEFONTOUTLINESWPROC)(HDC hDC, DWORD first, DWORD count, DWORD listBase, FLOAT deviation, FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf);

//------------------------------------------------------------------------------

#define PROC_LIST \
    PROC(PFNWGLCOPYCONTEXTPROC,             wglCopyContext)                 \
    PROC(PFNWGLCREATECONTEXTPROC,           wglCreateContext)               \
    PROC(PFNWGLCREATELAYERCONTEXTPROC,      wglCreateLayerContext)          \
    PROC(PFNWGLDELETECONTEXTPROC,           wglDeleteContext)               \
    PROC(PFNWGLDESCRIBELAYERPLANEPROC,      wglDescribeLayerPlane)          \
    PROC(PFNWGLGETCURRENTCONTEXTPROC,       wglGetCurrentContext)           \
    PROC(PFNWGLGETCURRENTDCPROC,            wglGetCurrentDC)                \
    PROC(PFNWGLGETLAYERPALETTEENTRIESPROC,  wglGetLayerPaletteEntries)      \
    PROC(PFNWGLGETPROCADDRESSPROC,          wglGetProcAddress)              \
    PROC(PFNWGLMAKECURRENTPROC,             wglMakeCurrent)                 \
    PROC(PFNWGLREALIZELAYERPALETTEPROC,     wglRealizeLayerPalette)         \
    PROC(PFNWGLSETLAYERPALETTEENTRIESPROC,  wglSetLayerPaletteEntries)      \
    PROC(PFNWGLSHARELISTSPROC,              wglShareLists)                  \
    PROC(PFNWGLSWAPLAYERBUFFERSPROC,        wglSwapLayerBuffers)            \
    PROC(PFNWGLUSEFONTBITMAPSAPROC,         wglUseFontBitmapsA)             \
    PROC(PFNWGLUSEFONTBITMAPSWPROC,         wglUseFontBitmapsW)             \
    PROC(PFNWGLUSEFONTOUTLINESAPROC,        wglUseFontOutlinesA)            \
    PROC(PFNWGLUSEFONTOUTLINESWPROC,        wglUseFontOutlinesW)

//------------------------------------------------------------------------------

#define PROC(type, name) \
    static type dyn_##name;

PROC_LIST

#undef PROC

//------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4706)
#endif

#define PROC(type, name) \
    if (!(dyn_##name = pl_get_dll_proc(dll, #name))) { \
        LIBQU_LOGE("Failed to load function: %s.\n", #name); \
        pl_close_dll(dll); \
        return NULL; \
    }

static void *dyn_load_wgl(void)
{
    char const *names[] = {
        "opengl32.dll",
    };

    void *dll = NULL;

    for (size_t i = 0; i < sizeof(names) / sizeof(*names); i++) {
        dll = pl_open_dll(names[i]);

        if (dll) {
            LIBQU_LOGI("OpenGL is opened from %s.\n", names[i]);
            break;
        }
    }

    if (!dll) {
        LIBQU_LOGE("Failed to open OpenGL DLL.\n");
        return NULL;
    }

    PROC_LIST

    return dll;
}

#undef PROC

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//------------------------------------------------------------------------------

#define wglCopyContext                  dyn_wglCopyContext
#define wglCreateContext                dyn_wglCreateContext
#define wglCreateLayerContext           dyn_wglCreateLayerContext
#define wglDeleteContext                dyn_wglDeleteContext
#define wglDescribeLayerPlane           dyn_wglDescribeLayerPlane
#define wglGetCurrentContext            dyn_wglGetCurrentContext
#define wglGetCurrentDC                 dyn_wglGetCurrentDC
#define wglGetLayerPaletteEntries       dyn_wglGetLayerPaletteEntries
#define wglGetProcAddress               dyn_wglGetProcAddress
#define wglMakeCurrent                  dyn_wglMakeCurrent
#define wglRealizeLayerPalette          dyn_wglRealizeLayerPalette
#define wglSetLayerPaletteEntries       dyn_wglSetLayerPaletteEntries
#define wglShareLists                   dyn_wglShareLists
#define wglSwapLayerBuffers             dyn_wglSwapLayerBuffers
#define wglUseFontBitmapsA              dyn_wglUseFontBitmapsA
#define wglUseFontBitmapsW              dyn_wglUseFontBitmapsW
#define wglUseFontOutlinesA             dyn_wglUseFontOutlinesA
#define wglUseFontOutlinesW             dyn_wglUseFontOutlinesW

//------------------------------------------------------------------------------

#endif // _WIN32

//------------------------------------------------------------------------------

#endif // LIBQU_DYN_WGL_H_INC
