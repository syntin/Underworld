#pragma once
#include <cstdint>

class WindowSize
{
public:
	WindowSize() {}
	virtual ~WindowSize() {}

public:
	void Set(uint32_t width, uint32_t height)
	{
		_width = width;
		_height = height;
	}
	void SetWidth(uint32_t width) { _width = width; }
	void SetHeight(uint32_t height) { _height = height; }
	uint32_t GetWidth() { return _width; }
	uint32_t GetHeight() { return _height; }

private:
	uint32_t _width;
	uint32_t _height;
};