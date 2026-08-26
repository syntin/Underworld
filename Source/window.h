#pragma once
#include <GLFW/glfw3.h>
#include <Windows.h>

class Window
{
public:
	Window();
	virtual ~Window();

public:
	void CreateGLFWwindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
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
	HWND _hWnd;
};

