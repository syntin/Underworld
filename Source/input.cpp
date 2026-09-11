//DB
#include "input.h"

void Input::Update()
{
	previousKeys = currentKeys;

	mouseDelta = mousePos - prevMousePos;
	prevMousePos = mousePos;

	scrollDelta = 0.0f;
}

bool Input::IsKeyDown(int key) const
{
	return currentKeys[key];
}

bool Input::WasKeyPressed(int key) const
{
	return currentKeys[key] && !previousKeys[key];
}

bool Input::WasKeyReleased(int key) const
{
	return !currentKeys[key] && previousKeys[key];
}

void Input::SetKey(int key, bool down)
{
	currentKeys[key] = down;
}

void Input::SetMousePosition(float x, float y)
{
	mousePos = {x, y};
}

void Input::AddScollDelta(float delta)
{
	scrollDelta += delta;
}