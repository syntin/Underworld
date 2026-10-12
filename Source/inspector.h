//DB
#pragma once
#include "exportSystem.h"
#include <SDL3/SDL.h>

class Inspector
{
public:
    void Draw(SDL_Renderer* renderer);
    void HandleEvent(const SDL_Event& e);

private:
    bool open = true;

    // Event pointer for this frame
    const SDL_Event* lastEvent = nullptr;

    // Slider dragging state
    bool draggingSlider = false;
    float* activeSliderValue = nullptr;
    float activeSliderMin = 0.0f;
    float activeSliderMax = 1.0f;
    float activeSliderX = 0.0f;
    float activeSliderY = 0.0f;
    float activeSliderWidth = 200.0f;
};