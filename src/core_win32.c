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

#include <shlwapi.h>

#include <GL/gl.h>
#include <GL/wglext.h>

#include "core.h"
#include "dyn_wgl.h"

//------------------------------------------------------------------------------

enum
{
    _ARB_PIXEL_FORMAT                   = (1L << 0),
    _ARB_CREATE_CONTEXT                 = (1L << 1),
    _ARB_CREATE_CONTEXT_PROFILE         = (1L << 2),
    _EXT_CREATE_CONTEXT_ES2_PROFILE     = (1L << 3),
    _EXT_SWAP_CONTROL                   = (1L << 4),
};

//------------------------------------------------------------------------------

// SetProcessDpiAwareness() from shcore.dll
typedef HRESULT (APIENTRY *SETPROCESSDPIAWARENESSPROC)(int);

//------------------------------------------------------------------------------

static struct
{
    HINSTANCE       hInstance;
    HCURSOR         hCursor;
    WCHAR           szTitle[LIBQU_WINDOW_TITLE_LENGTH];
    DWORD           dwStyle;
    HWND            hWnd;
    DWORD           dwWGLExtensions;
    HDC             hDC;
    HGLRC           hGLRC;
    int             nSamples;
    int             nGLVersion;
    void            *pGL;

    PFNWGLGETPIXELFORMATATTRIBIVARBPROC     wglGetPixelFormatAttribivARB;
    PFNWGLCHOOSEPIXELFORMATARBPROC          wglChoosePixelFormatARB;
    PFNWGLCREATECONTEXTATTRIBSARBPROC       wglCreateContextAttribsARB;
    PFNWGLSWAPINTERVALEXTPROC               wglSwapIntervalEXT;
} priv;

//------------------------------------------------------------------------------

static void LogWin32Error(void)
{
    DWORD dwError = GetLastError();
    LPVOID lpBuffer;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        &lpBuffer,
        0, NULL);

    LIBQU_LOGE("GetLastError() -> 0x%04x: %s\n", dwError, lpBuffer);

    LocalFree(lpBuffer);
}

static void ParseSingleWGLExtension(char const *pszExtension)
{
    if (strcmp(pszExtension, "WGL_ARB_pixel_format") == 0) {
        priv.dwWGLExtensions |= _ARB_PIXEL_FORMAT;

        priv.wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
            wglGetProcAddress("wglGetPixelFormatAttribivARB");

        priv.wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)
            wglGetProcAddress("wglChoosePixelFormatARB");

        return;
    }

    if (strcmp(pszExtension, "WGL_ARB_create_context") == 0) {
        priv.dwWGLExtensions |= _ARB_CREATE_CONTEXT;
    
        priv.wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
            wglGetProcAddress("wglCreateContextAttribsARB");

        return;
    }

    if (strcmp(pszExtension, "WGL_ARB_create_context_profile") == 0) {
        priv.dwWGLExtensions |= _ARB_CREATE_CONTEXT_PROFILE;
        return;
    }

    if (strcmp(pszExtension, "WGL_EXT_create_context_es2_profile") == 0) {
        priv.dwWGLExtensions |= _EXT_CREATE_CONTEXT_ES2_PROFILE;
        return;
    }

    if (strcmp(pszExtension, "WGL_EXT_swap_control") == 0) {
        priv.dwWGLExtensions |= _EXT_SWAP_CONTROL;

        priv.wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)
            wglGetProcAddress("wglSwapIntervalEXT");

        return;
    }
}

static void ParseWGLExtensions(HDC hDC)
{
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
        (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress("wglGetExtensionsStringARB");
    
    if (!wglGetExtensionsStringARB) {
        return;
    }

    LIBQU_LOGI("WGL extensions: %s\n", wglGetExtensionsStringARB(hDC));

    char *pszExtensions = StrDupA(wglGetExtensionsStringARB(hDC));

    if (!pszExtensions) {
        return;
    }

    char *pszToken = strtok(pszExtensions, " ");

    while (pszToken) {
        ParseSingleWGLExtension(pszToken);
        pszToken = strtok(NULL, " ");
    }

    LocalFree(pszExtensions);
}

static int BootstrapWGL(void)
{
    WNDCLASSEXW wcex = {
        .cbSize         = sizeof(WNDCLASSEXW),
        .style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc    = DefWindowProcW,
        .hInstance      = priv.hInstance,
        .lpszClassName  = L"Trampoline",
    };

    if (!RegisterClassExW(&wcex)) {
        return -1;
    }

    HWND hWnd = CreateWindowExW(0, wcex.lpszClassName, wcex.lpszClassName, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, wcex.hInstance, NULL);

    if (!hWnd) {
        return -1;
    }

    int nStatus = -1;
    HDC hDC = GetDC(hWnd);

    if (hDC) {
        PIXELFORMATDESCRIPTOR pfd = {
            .nSize          = sizeof(PIXELFORMATDESCRIPTOR),
            .nVersion       = 1,
            .dwFlags        = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
            .iPixelType     = PFD_TYPE_RGBA,
            .cColorBits     = 32,
            .cAlphaBits     = 8,
            .iLayerType     = PFD_MAIN_PLANE,
        };

        int nFormat = ChoosePixelFormat(hDC, &pfd);

        if (nFormat && SetPixelFormat(hDC, nFormat, &pfd)) {
            HGLRC hGLRC = wglCreateContext(hDC);

            if (hGLRC) {
                if (wglMakeCurrent(hDC, hGLRC)) {
                    nStatus = 0;
                    ParseWGLExtensions(hDC);
                    wglMakeCurrent(hDC, NULL);
                }

                wglDeleteContext(hGLRC);
            }
        }

        ReleaseDC(hWnd, hDC);
    }

    DestroyWindow(hWnd);

    return nStatus;
}

static int InitPixelFormatEx(HDC hDC)
{
    int formatAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     (TRUE),
        WGL_ACCELERATION_ARB,       (WGL_FULL_ACCELERATION_ARB),
        WGL_SUPPORT_OPENGL_ARB,     (TRUE),
        WGL_DOUBLE_BUFFER_ARB,      (TRUE),
        WGL_PIXEL_TYPE_ARB,         (WGL_TYPE_RGBA_ARB),
        WGL_COLOR_BITS_ARB,         (32),
        WGL_DEPTH_BITS_ARB,         (24),
        WGL_STENCIL_BITS_ARB,       (8),
        0,
    };

    int formats[256];
    UINT nFormats = 0;

    priv.wglChoosePixelFormatARB(hDC, formatAttribs, NULL, 256, formats, &nFormats);

    if (!nFormats) {
        LIBQU_LOGE("InitPixelFormatEx: wglChoosePixelFormatARB() failed.\n");
        return -1;
    }

    int iBestFormat = -1;
    int nBestSamples = 0;

    for (unsigned int i = 0; i < nFormats; i++) {
        int attribs[] = {
            WGL_SAMPLE_BUFFERS_ARB,
            WGL_SAMPLES_ARB,
        };

        int values[2];
        priv.wglGetPixelFormatAttribivARB(hDC, formats[i], 0, 2, attribs, values);

        int nSampleBuffers = values[0];
        int nSamples = values[1];

        if (!nSampleBuffers) {
            continue;
        }

        if (nSamples > nBestSamples) {
            iBestFormat = i;
            nBestSamples = nSamples;
        }
    }

    int nFormat;
    PIXELFORMATDESCRIPTOR pfd;

    if (iBestFormat == -1) {
        priv.nSamples = 1;
        nFormat = formats[0];
    } else {
        priv.nSamples = nBestSamples;
        nFormat = formats[iBestFormat];
    }

    LIBQU_LOGI("InitPixelFormatEx: chose format with %d samples.\n", priv.nSamples);

    if (!DescribePixelFormat(hDC, nFormat, sizeof(pfd), &pfd)) {
        LIBQU_LOGE("InitPixelFormatEx: DescribePixelFormat() failed.\n");
        return -1;
    }

    if (!SetPixelFormat(hDC, nFormat, &pfd)) {
        LIBQU_LOGE("InitPixelFormatEx: SetPixelFormat() failed.\n");
        LogWin32Error();
        return -1;
    }

    return 0;
}

static int InitPixelFormat(HDC hDC)
{
    if (priv.dwWGLExtensions & _ARB_PIXEL_FORMAT) {
        return InitPixelFormatEx(hDC);
    }

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize          = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion       = 1,
        .dwFlags        = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType     = PFD_TYPE_RGBA,
        .cColorBits     = 32,
        .cStencilBits   = 8,
        .iLayerType     = PFD_MAIN_PLANE,
    };

    int nFormat = ChoosePixelFormat(hDC, &pfd);

    if (!nFormat) {
        LIBQU_LOGE("InitPixelFormat: ChoosePixelFormat() failed.\n");
        return -1;
    }

    if (!SetPixelFormat(hDC, nFormat, &pfd)) {
        LIBQU_LOGE("InitPixelFormat: SetPixelFormat() failed.\n");
        return -1;
    }

    return 0;
}

static HGLRC InitGLRCEx(HDC hDC)
{
    int nProfileAttrib = 0;
    int nProfileMask = 0;

    if (priv.dwWGLExtensions & _ARB_CREATE_CONTEXT_PROFILE) {
        nProfileAttrib = WGL_CONTEXT_PROFILE_MASK_ARB;
        nProfileMask = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
    }

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,  3,
        WGL_CONTEXT_MINOR_VERSION_ARB,  3,
        nProfileAttrib,                 nProfileMask,
        0,
    };

    HGLRC hGLRC = priv.wglCreateContextAttribsARB(hDC, NULL, attribs);

    if (!hGLRC) {
        LIBQU_LOGE("InitGLRCEx: wglCreateContextAttribsARB() failed.\n");
        return NULL;
    }

    if (!wglMakeCurrent(hDC, hGLRC)) {
        LIBQU_LOGE("InitGLRCEx: wglMakeCurrent() failed.\n");
        wglDeleteContext(hGLRC);
        return NULL;
    }

    priv.nGLVersion = 330;

    return hGLRC;
}

static HGLRC InitGLRC(HDC hDC)
{
    if (priv.dwWGLExtensions & _ARB_CREATE_CONTEXT) {
        return InitGLRCEx(hDC);
    }

    HGLRC hGLRC = wglCreateContext(hDC);

    if (!hGLRC) {
        LIBQU_LOGE("InitGLRC: wglCreateContext() failed.\n");
        return NULL;
    }

    if (!wglMakeCurrent(hDC, hGLRC)) {
        LIBQU_LOGE("InitGLRC: wglMakeCurrent() failed.\n");
        wglDeleteContext(hGLRC);
        return NULL;
    }

    priv.nGLVersion = 110;

    return hGLRC;
}

static void InitSwapInterval(void)
{
    if (priv.dwWGLExtensions & _EXT_SWAP_CONTROL) {
        priv.wglSwapIntervalEXT(1);
    }
}

static int InitWGL(HWND hWnd)
{
    priv.pGL = dyn_load_wgl();

    if (!priv.pGL) {
        LIBQU_LOGE("InitWGL: failed to load DLL.\n");
        return -1;
    }

    if (BootstrapWGL() == -1) {
        LIBQU_LOGE("InitWGL: failed to obtain WGL extension list.\n");
        return -1;
    }

    HDC hDC = GetDC(hWnd);

    if (hDC) {
        if (InitPixelFormat(hDC) == 0) {
            HGLRC hGLRC = InitGLRC(hDC);

            if (hGLRC) {
                InitSwapInterval();

                priv.hDC = hDC;
                priv.hGLRC = hGLRC;

                return 0;
            }
        }

        ReleaseDC(hWnd, hDC);
    }

    LIBQU_LOGE("InitWGL: context is not created.\n");

    return -1;
}

static qu_key ConvertKey(WPARAM wParam, LPARAM lParam)
{
    UINT nVirtualKey = (UINT) wParam;
    UINT nScancode = (lParam & (0xff << 16)) >> 16;
    BOOL fExtended = (lParam & (0x01 << 24));

    if (nVirtualKey >= 'A' && nVirtualKey <= 'Z') {
        return QU_KEY_A + (nVirtualKey - 'A');
    }

    if (nVirtualKey >= '0' && nVirtualKey <= '9') {
        return QU_KEY_0 + (nVirtualKey - '0');
    }

    if (nVirtualKey >= VK_F1 && nVirtualKey <= VK_F12) {
        return QU_KEY_F1 + (nVirtualKey - VK_F1);
    }

    if (nVirtualKey >= VK_NUMPAD0 && nVirtualKey <= VK_NUMPAD9) {
        return QU_KEY_KP_0 + (nVirtualKey - VK_NUMPAD0);
    }

    if (nVirtualKey == VK_RETURN) {
        if (fExtended) {
            return QU_KEY_KP_ENTER;
        }

        return QU_KEY_ENTER;
    }

    if (nVirtualKey == VK_SHIFT) {
        UINT nExtendedKey = MapVirtualKey(nScancode, MAPVK_VSC_TO_VK_EX);

        if (nExtendedKey == VK_LSHIFT) {
            return QU_KEY_LSHIFT;
        } else if (nExtendedKey == VK_RSHIFT) {
            return QU_KEY_RSHIFT;
        }
    }

    if (nVirtualKey == VK_CONTROL) {
        if (fExtended) {
            return QU_KEY_RCTRL;
        }

        return QU_KEY_LCTRL;
    }

    if (nVirtualKey == VK_MENU) {
        if (fExtended) {
            return QU_KEY_RALT;
        }

        return QU_KEY_LALT;
    }

    switch (nVirtualKey) {
    case VK_BACK:               return QU_KEY_BACKSPACE;
    case VK_TAB:                return QU_KEY_TAB;
    case VK_PAUSE:              return QU_KEY_PAUSE;
    case VK_CAPITAL:            return QU_KEY_CAPSLOCK;
    case VK_ESCAPE:             return QU_KEY_ESCAPE;
    case VK_SPACE:              return QU_KEY_SPACE;
    case VK_PRIOR:              return QU_KEY_PGUP;
    case VK_NEXT:               return QU_KEY_PGDN;
    case VK_END:                return QU_KEY_END;
    case VK_HOME:               return QU_KEY_HOME;
    case VK_LEFT:               return QU_KEY_LEFT;
    case VK_UP:                 return QU_KEY_UP;
    case VK_RIGHT:              return QU_KEY_RIGHT;
    case VK_DOWN:               return QU_KEY_DOWN;
    case VK_PRINT:              return QU_KEY_PRINTSCREEN;
    case VK_INSERT:             return QU_KEY_INSERT;
    case VK_DELETE:             return QU_KEY_DELETE;
    case VK_LWIN:               return QU_KEY_LSUPER;
    case VK_RWIN:               return QU_KEY_RSUPER;
    case VK_MULTIPLY:           return QU_KEY_KP_MUL;
    case VK_ADD:                return QU_KEY_KP_ADD;
    case VK_SUBTRACT:           return QU_KEY_KP_SUB;
    case VK_DECIMAL:            return QU_KEY_KP_POINT;
    case VK_DIVIDE:             return QU_KEY_KP_DIV;
    case VK_NUMLOCK:            return QU_KEY_NUMLOCK;
    case VK_SCROLL:             return QU_KEY_SCROLLLOCK;
    case VK_LMENU:              return QU_KEY_MENU;
    case VK_RMENU:              return QU_KEY_MENU;
    case VK_OEM_1:              return QU_KEY_SEMICOLON;
    case VK_OEM_PLUS:           return QU_KEY_EQUAL;
    case VK_OEM_COMMA:          return QU_KEY_COMMA;
    case VK_OEM_MINUS:          return QU_KEY_MINUS;
    case VK_OEM_PERIOD:         return QU_KEY_PERIOD;
    case VK_OEM_2:              return QU_KEY_SLASH;
    case VK_OEM_3:              return QU_KEY_GRAVE;
    case VK_OEM_4:              return QU_KEY_LBRACKET;
    case VK_OEM_6:              return QU_KEY_RBRACKET;
    case VK_OEM_5:              return QU_KEY_BACKSLASH;
    case VK_OEM_7:              return QU_KEY_APOSTROPHE;
    }

    return QU_KEY_INVALID;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        return InitWGL(hWnd);
    case WM_DESTROY:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_ACTIVATE:
        {
            struct libqu_event event = {
                .type = (LOWORD(wParam) == WA_INACTIVE)
                    ? LIBQU_EVENT_DEACTIVATE
                    : LIBQU_EVENT_ACTIVATE,
            };

            libqu_core_enqueue_event(&event);
        }
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        {
            struct libqu_event event = {
                .type = (msg == WM_KEYUP || msg == WM_SYSKEYUP)
                    ? LIBQU_EVENT_KEY_RELEASED
                    : LIBQU_EVENT_KEY_PRESSED,
                .data = {
                    .key = ConvertKey(wParam, lParam),
                },
            };

            libqu_core_enqueue_event(&event);
        }
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

static void InitDpiAwareness(void)
{
    void *pShCore = pl_open_dll("shcore.dll");

    if (!pShCore) {
        return;
    }

    SETPROCESSDPIAWARENESSPROC pfnSetProcessDpiAwareness =
        pl_get_dll_proc(pShCore, "SetProcessDpiAwareness");

    if (pfnSetProcessDpiAwareness) {
        // 2: PROCESS_PER_MONITOR_DPI_AWARE
        pfnSetProcessDpiAwareness(2);
    }

    pl_close_dll(pShCore);
}

static int InitCursor(void)
{
    priv.hCursor = LoadCursorW(NULL, IDC_CROSS);

    return (priv.hCursor) ? 0 : -1;
}

static int InitTitle(char const *title)
{
    int nSuccess = MultiByteToWideChar(CP_UTF8, 0, title, -1,
        priv.szTitle, LIBQU_WINDOW_TITLE_LENGTH);

    return (nSuccess > 0) ? 0 : -1;
}

static int InitGeometry(int width, int height)
{
    HMONITOR hMonitor = MonitorFromWindow(priv.hWnd, MONITOR_DEFAULTTONEAREST);

    MONITORINFO mi = {
        .cbSize = sizeof(MONITORINFO),
    };

    GetMonitorInfoW(hMonitor, &mi);

    int dx = (mi.rcMonitor.right - mi.rcMonitor.left - width) / 2;
    int dy = (mi.rcMonitor.bottom - mi.rcMonitor.top - height) / 2;

    RECT rect = {
        .left   = dx,
        .top    = dy,
        .right  = dx + width,
        .bottom = dy + height,
    };
    
    if (!AdjustWindowRect(&rect, priv.dwStyle, FALSE)) {
        return -1;
    }

    int x = rect.left;
    int y = rect.top;
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;

    if (!SetWindowPos(priv.hWnd, NULL, x, y, w, h, SWP_SHOWWINDOW)) {
        return -1;
    }

    UpdateWindow(priv.hWnd);

    return 0;
}

static int InitWindow(int width, int height)
{
    WNDCLASSEXW wcex = {
        .cbSize         = sizeof(WNDCLASSEXW),
        .style          = CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
        .lpfnWndProc    = WndProc,
        .cbClsExtra     = 0,
        .cbWndExtra     = 0,
        .hInstance      = priv.hInstance,
        .hIcon          = LoadIconW(NULL, IDI_WINLOGO),
        .hCursor        = priv.hCursor,
        .hbrBackground  = NULL,
        .lpszMenuName   = NULL,
        .lpszClassName  = priv.szTitle,
        .hIconSm        = NULL,
    };

    if (!RegisterClassExW(&wcex)) {
        LIBQU_LOGE("InitWindow: RegisterClassExW() failed.\n");
        return -1;
    }

    BOOL fResizable = FALSE; // TODO: later

    priv.dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    if (fResizable) {
        priv.dwStyle |= WS_MAXIMIZEBOX | WS_SIZEBOX;
    }

    priv.hWnd = CreateWindowW(
        priv.szTitle,       // class name
        priv.szTitle,       // window name
        priv.dwStyle,       // style
        CW_USEDEFAULT,      // x
        CW_USEDEFAULT,      // y
        width,              // width
        height,             // height
        NULL,               // parent
        NULL,               // menu
        priv.hInstance,     // instance
        NULL                // param
    );

    if (!priv.hWnd) {
        LIBQU_LOGE("InitWindow: CreateWindowW() failed.\n");
        return -1;
    }

    return InitGeometry(width, height);
}

//------------------------------------------------------------------------------

static bool core_win32_check_if_available(void)
{
    priv.hInstance = GetModuleHandleW(NULL);

    if (!priv.hInstance) {
        // Current instance is unknown for some reason,
        // nothing to do.
        return false;
    }

    return true;
}

static bool core_win32_initialize(struct libqu_core_params const *params)
{
    InitDpiAwareness();

    if (InitTitle(params->window_title) == -1) {
        LIBQU_LOGI("InitTitle() failed.\n");
        return false;
    }

    if (InitCursor() == -1) {
        LIBQU_LOGI("InitCursor() failed.\n");
        return false;
    }

    if (InitWindow(params->window_size.x, params->window_size.y) == -1) {
        LIBQU_LOGI("InitWindow() failed.\n");
        return false;
    }

    LIBQU_LOGI("Initialized.\n");

    return true;
}

static void core_win32_terminate(void)
{
    if (priv.hWnd) {
        if (priv.hDC) {
            if (priv.hGLRC) {
                wglMakeCurrent(priv.hDC, NULL);
                wglDeleteContext(priv.hGLRC);
            }

            ReleaseDC(priv.hWnd, priv.hDC);
        }

        DestroyWindow(priv.hWnd);
    }

    memset(&priv, 0, sizeof(priv));

    LIBQU_LOGI("Terminated.\n");
}

static bool core_win32_process(void)
{
    MSG msg;

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return true;
}

static void core_win32_swap(void)
{
    SwapBuffers(priv.hDC);
}

static bool core_win32_set_window_title(char const *title)
{
    if (!InitTitle(title)) {
        return false;
    }

    return SetWindowTextW(priv.hWnd, priv.szTitle);
}

static bool core_win32_set_window_size(qu_vec2i size)
{
    if (InitGeometry(size.x, size.y) == -1) {
        return false;
    }

    return true;
}

static int core_wgl_get_version(void)
{
    return priv.nGLVersion;
}

static void *core_wgl_get_proc_address(char const *name)
{
    void *proc;

    // This returns NULL for legacy (1.0 and 1.1) OpenGL functions.
    if ((proc = (void *) wglGetProcAddress(name))) {
        return proc;
    }

    // If legacy function is requested, get its address directly from DLL.
    if ((proc = pl_get_dll_proc(priv.pGL, name))) {
        return proc;
    }

    return NULL;
}

//------------------------------------------------------------------------------

struct libqu_core_impl const libqu_core_win32_impl = {
    core_win32_check_if_available,
    core_win32_initialize,
    core_win32_terminate,
    core_win32_process,
    core_win32_swap,
    core_win32_set_window_title,
    core_win32_set_window_size,
    core_wgl_get_version,
    core_wgl_get_proc_address,
};

//------------------------------------------------------------------------------

#endif // _WIN32
