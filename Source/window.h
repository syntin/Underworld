#pragma once
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <Windows.h>
#include <vector>
#include "windowSize.h"

class Window
{
public:
	Window();
	virtual ~Window();

public:
	void CreateGLFWwindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
	void CreateSDLwindow(VkInstance vkInstance, VkSurfaceKHR surface, WindowSize windowSize);
	//void CreateSDLwindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
	HWND GetHWnd() { return _hWnd; }

protected:
	void CreateBasicWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
	int RunMessageLoop();
	void Cleanup();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

private:
	GLFWwindow* _glfwWindow;
	SDL_Window* _sdlWindow;
	std::vector<VkPhysicalDevice> _physicalDevice;
	uint32_t _deviceIndex = 0;
	HWND _hWnd;
};

