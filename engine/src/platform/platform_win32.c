//
// Created by jwraynor on 8/21/2023.
//
#include "platform/platform.h"
#include "core/logger.h"
#include <stdio.h>

#if VPLATFORM_WINDOWS

#include "core/input.h"
#include <windows.h>
#include <windowsx.h>
#include <vulkan/vulkan.h>
#include "containers/darray.h"
#include "defines.h"
#include "renderer/vulkan/vulkan_types.inl"
#include <vulkan/vulkan_win32.h>
#include "core/event.h"
static f64 clock_frequency;
static LARGE_INTEGER start_time;

typedef struct internal_state {
  HINSTANCE hInstance;
  HWND hWnd;
  VkSurfaceKHR surface;
} internal_state;

LRESULT CALLBACK win32_process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

b8 platform_startup(platform_state *platform, conststr str, i32 x, i32 y, i32 width, i32 height) {
    //Allocate the internal state
    platform->internal_state = malloc(sizeof(internal_state));
    //Cast the internal state to the correct type
    internal_state *state = (internal_state *) platform->internal_state;

    //set our module handle to our owned HINSTANCE (this is the handle to our executable)
    state->hInstance = GetModuleHandle(NULL);

    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASS wc;
    memset(&wc, 0, sizeof(WNDCLASS));
    wc.style = CS_DBLCLKS; // Get double clicks
    wc.lpfnWndProc = win32_process_message; // handle our windows events
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "vlos_window_class";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Failed to register window class", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    //Adjust the window size to account for the window borders
    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, FALSE, window_ex_style);

    window_x += border_rect.left;;
    window_y += border_rect.top;
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowEx(window_ex_style, wc.lpszClassName, str, window_style, window_x, window_y, window_width,
                                 window_height, NULL, NULL, state->hInstance, NULL);

    if (!handle) {
        MessageBox(NULL, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
        vfatal("Failed to create window");
        return FALSE;
    } else {
        state->hWnd = handle;
        vdebug("Created window with handle %p", handle);
    }

    b32 should_activate = 1;
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state->hWnd, show_window_command_flags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64) frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
    return TRUE;
}

void platform_shutdown(platform_state *state) {
    internal_state *internal = (internal_state *) state->internal_state;
    if (internal->hWnd) {
        DestroyWindow(internal->hWnd);
        internal->hWnd = 0;
    }
}

b8 platform_pump_messages(platform_state *state) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

void *platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}

void platform_free(void *ptr, b8 aligned) {
    free(ptr);
}

void *platform_zero_memory(void *ptr, u64 size) {
    return memset(ptr, 0, size);
}

void *platform_copy_memory(void *dest, const void *src, u64 size) {
    return memcpy(dest, src, size);
}

void *platform_set_memory(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(conststr str, u8 color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[7] = {132, 140, 142, 143, 139, 141, 0};
    SetConsoleTextAttribute(hConsole, levels[color]);
    u64 length = strlen(str);
    LPDWORD written = 0;
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str, (DWORD) length, written, NULL);
}

void platform_console_write_error(conststr str, u8 color) {
    setvbuf(stdout, NULL, _IONBF, 0);
    HANDLE hConsole = GetStdHandle(STD_ERROR_HANDLE);
    //FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[7] = {132, 140, 142, 143, 139, 141, 0};
    SetConsoleTextAttribute(hConsole, levels[color]);
    u64 length = strlen(str);
    LPDWORD written = 0;
    WriteConsole(GetStdHandle(STD_ERROR_HANDLE), str, (DWORD) length, written, NULL);
}

f64 platform_get_time() {
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&frequency);
    return (f64) frequency.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

void platform_required_extensions(const char ***names_darray) {
    darray_push(*names_darray, &"VK_KHR_win32_surface");
}

b8 platform_create_vulkan_surface(platform_state *plat_state, vulkan_context *context) {
    // Simply cold-cast to the known type.
    internal_state *state = (internal_state *) plat_state->internal_state;

    VkWin32SurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = state->hInstance;
    create_info.hwnd = state->hWnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &state->surface);
    if (result != VK_SUCCESS) {
        vfatal("Vulkan surface creation failed.");
        return FALSE;
    }

    context->surface = state->surface;
    return TRUE;
}

LRESULT CALLBACK win32_process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_ERASEBKGND:return 1;
            //TODO: post an application quit message
        case WM_CLOSE: {
            event_context context;
            event_trigger(SYSTEM_EVENT_CODE_QUIT, NULL, context);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {
            //TODO: handle resizing
            vdebug("Resizing window")
        }
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            //TODO: handle keyboard input
            b8 pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            keys key = (keys) wParam;
            input_process_key(key, pressed);
        }
            break;
        case WM_MOUSEMOVE: {
            i32 x = GET_X_LPARAM(lParam);
            i32 y = GET_Y_LPARAM(lParam);
            input_process_mouse_position((i16) x, (i16) y);
        }
            break;
        case WM_MOUSEWHEEL: {
            i32 delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta != 0) {
                i8 flat = (delta < 0) ? -1 : 1;
                input_process_mouse_wheel(flat);
            }
        }
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            b8 pressed = (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN);
            buttons mouse_button = BUTTON_MAX_BUTTONS;
            switch (uMsg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:mouse_button = BUTTON_LEFT;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:mouse_button = BUTTON_RIGHT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:mouse_button = BUTTON_MIDDLE;
                    break;
            }
            //TODO: handle mouse button
            if (mouse_button != BUTTON_MAX_BUTTONS) {
                input_process_button(mouse_button, pressed);
            }

        }
            break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#endif