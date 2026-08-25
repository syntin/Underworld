#pragma once
#include <GLFW/glfw3.h>
#include <Windows.h>

class Window
{
public:
	Window();
	virtual ~Window();

public:
	void CreateGLFWwindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
	HWND GetHWnd() { return _hWnd; }

protected:
	void CreateBasicWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
	int RunMessageLoop();
	void Cleanup();

private:
	GLFWwindow* _glfwWindow;
	HWND _hWnd;
};

