//DB
#pragma once
#include <glm/glm.hpp>
#include <array>

class Input
{
public:
	static constexpr int KEY_COUNT = 512;

	void Update();

	bool IsKeyDown(int key) const;
	bool WasKeyPressed(int key) const;
	bool WasKeyReleased(int key) const;

	glm::vec2 GetMousePosition() const { return mousePos; }
	glm::vec2 GetMouseDelta() const { return mouseDelta; }
	float GetScrollDelta() const { return scrollDelta; }

	void SetKey(int key, bool down);
	void SetMousePosition(float x, float y);
	void AddScollDelta(float delta);

private:
	std::array<bool, KEY_COUNT> currentKeys{};
	std::array<bool, KEY_COUNT> previousKeys{};

	glm::vec2 mousePos = { 0,0 };
	glm::vec2 prevMousePos = { 0,0 };
	glm::vec2 mouseDelta = { 0,0 };

	float scrollDelta = 0.0f;
};