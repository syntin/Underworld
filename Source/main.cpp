#include <stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>
#include <GLFW/glfw3.h>
#include "utils.h"

#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const wchar_t className[] = L"Vulkan Window";
    const wchar_t windowName[] = L"Vulkan Engine";
    WNDCLASSEX wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className; // <-- Setting the unique identifier
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowExW(WS_OVERLAPPEDWINDOW, className, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    // Initialize Vulkan

    ShowWindow(hWnd, nShowCmd);

    // 5. Run the message loop
    MSG msg = { };
    while (PeekMessage(&msg, hWnd, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}