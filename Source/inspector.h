//DB
#pragma once
#include "exportSystem.h"
#include <SDL3/SDL.h>

class Inspector
{
public:
    SDL_Window* inspectorWindow = nullptr;

    void HandleEvent(const SDL_Event& e);
    void Draw(SDL_Renderer* renderer);

    bool open = true;

    bool   draggingSlider = false;
    float* activeSliderValue = nullptr;
    float  activeSliderMin = 0.0f;
    float  activeSliderMax = 0.0f;
    float  activeSliderX = 0.0f;
    float  activeSliderY = 0.0f;
    float  activeSliderWidth = 0.0f;

    SDL_Event lastEvent{};
    bool      hasLastEvent = false;
};
