#include "window.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <SDL3/SDL.h>
#include "utils.h"
#include <Windows.h>
#include <cassert>
#include "utils.h"
#include "windowSize.h"

 

Window::Window()
{

}

Window::~Window()
{

}

void Window::CreateGLFWwindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	CreateBasicWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);

	if (!glfwInit()) {
		throw(std::runtime_error("Failed to create GLFW Window"));
	}

	// 2. Set optional window hints (e.g., OpenGL 3.3 Core Profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 3. Create the window pointer
	_glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "My Window Title", NULL, NULL);
	if (!_glfwWindow) {
		Cleanup();
	}

	glfwMakeContextCurrent(_glfwWindow);
	glfwTerminate();

	ShowWindow(_hWnd, nCmdShow);
	RunMessageLoop();
}

void CreateSDLwindow(VkInstance vkInstance, VkSurfaceKHR surface, WindowSize windowSize)
{
	SDL_Window* window = SDL_CreateWindow("How to Vulkan", 1280u, 720u, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	assert(window);
	chk(SDL_Vulkan_CreateSurface(window, vkInstance, nullptr, &surface));
	chk(SDL_GetWindowSize(window, &windowSize.x, &windowSize.y));
	VkSurfaceCapabilitiesKHR surfaceCaps{};
	chk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[deviceIndex], surface, &surfaceCaps));
	VkExtent2D swapchainExtent{ surfaceCaps.currentExtent };
	if (surfaceCaps.currentExtent.width == 0xFFFFFFFF) {
		swapchainExtent = { .width = static_cast<uint32_t>(windowSize.x()), .height = static_cast<uint32_t>(windowSize.y()) };
	}
}

void Window::CreateBasicWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	const wchar_t className[] = L"Astoroth Render Engine";
	const wchar_t windowName[] = L"Wishbone Game Engine";

	WNDCLASSEXW wc = { };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassExW(&wc);

	_hWnd = CreateWindowExW(WS_OVERLAPPEDWINDOW,
		className,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
}

int Window::RunMessageLoop()
{
	MSG msg = { };
	while (PeekMessage(&msg, _hWnd, 0, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


void Window::Cleanup()
{
	if (_glfwWindow) {
		glfwDestroyWindow(_glfwWindow);
		glfwTerminate();
	}
}
