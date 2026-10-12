//DB
#include "Inspector.h"
#include <SDL3/SDL.h>
#include <iostream>

// TEMP TEXT DRAWING (no SDL_ttf yet)
static void DrawText(SDL_Renderer* renderer, const std::string& text, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, x, y, x + (int)text.size() * 7, y);
}

// Convert pixel mouse coords → logical window coords
static void GetLogicalMouse(const SDL_Event& e, SDL_Window* window, float& lx, float& ly)
{
    int winW, winH;
    int pixelW, pixelH;

    SDL_GetWindowSize(window, &winW, &winH);
    SDL_GetWindowSizeInPixels(window, &pixelW, &pixelH);

    float scaleX = (float)winW / (float)pixelW;
    float scaleY = (float)winH / (float)pixelH;

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
        e.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        lx = e.button.x * scaleX;
        ly = e.button.y * scaleY;
    }
    else if (e.type == SDL_EVENT_MOUSE_MOTION)
    {
        lx = e.motion.x * scaleX;
        ly = e.motion.y * scaleY;
    }
}

// SLIDER
static bool DrawSlider(SDL_Renderer* renderer,
    float x, float y,
    float width,
    float* value,
    float min, float max,
    const SDL_Event* e,
    SDL_Window* window,
    bool& dragging,
    float*& activeValue,
    float& activeMin,
    float& activeMax,
    float& activeX,
    float& activeY,
    float& activeWidth)
{
    const float barHeight = 14.0f;

    SDL_FRect bar = { x, y, width, barHeight };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bar);

    float t = (*value - min) / (max - min);
    t = SDL_clamp(t, 0.0f, 1.0f);

    float handleX = x + t * width;

    SDL_FRect handle = { handleX - 6.0f, y - 4.0f, 12.0f, barHeight + 8.0f };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &handle);

    if (e)
    {
        float mx = 0, my = 0;
        GetLogicalMouse(*e, window, mx, my);

        if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (mx >= x && mx <= x + width &&
                my >= y && my <= y + barHeight)
            {
                dragging = true;
                activeValue = value;
                activeMin = min;
                activeMax = max;
                activeX = x;
                activeY = y;
                activeWidth = width;

                float newT = (mx - x) / width;
                newT = SDL_clamp(newT, 0.0f, 1.0f);
                *value = min + newT * (max - min);
                return true;
            }
        }
        else if (e->type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            dragging = false;
            activeValue = nullptr;
        }
        else if (e->type == SDL_EVENT_MOUSE_MOTION &&
            dragging && activeValue == value)
        {
            float newT = (mx - activeX) / activeWidth;
            newT = SDL_clamp(newT, 0.0f, 1.0f);

            *activeValue = activeMin + newT * (activeMax - activeMin);
            return true;
        }
    }

    return false;
}

// CHECKBOX
static bool DrawCheckbox(SDL_Renderer* renderer,
    float x, float y,
    bool* value,
    const SDL_Event* e,
    SDL_Window* window)
{
    SDL_FRect box = { x, y, 18.0f, 18.0f };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &box);

    if (*value)
    {
        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(renderer, &box);
    }

    if (e && e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mx, my;
        GetLogicalMouse(*e, window, mx, my);

        if (mx >= x && mx <= x + 18.0f &&
            my >= y && my <= y + 18.0f)
        {
            *value = !(*value);
            return true;
        }
    }

    return false;
}

// EVENT HANDLING
void Inspector::HandleEvent(const SDL_Event& e)
{
    lastEvent = e;
    hasLastEvent = true;

    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_F1)
        open = !open;

    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        draggingSlider = false;
        activeSliderValue = nullptr;
    }
}

// DRAW
void Inspector::Draw(SDL_Renderer* renderer)
{
    if (!open) return;

    SDL_FRect panel = { 20.0f, 20.0f, 340.0f, 700.0f };
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 220);
    SDL_RenderFillRect(renderer, &panel);

    const auto& vars = ExportRegistry::Instance().GetAll();

    const SDL_Event* ePtr = hasLastEvent ? &lastEvent : nullptr;

    float y = 40.0f;

    for (const auto& var : vars)
    {
        DrawText(renderer, var.name, 30, (int)y);

        if (var.typeName.find("float") != std::string::npos)
        {
            float* v = (float*)var.valuePtr;

            DrawSlider(renderer, 30.0f, y + 20.0f, 200.0f,
                v, -10.0f, 10.0f,
                ePtr,
                inspectorWindow,
                draggingSlider,
                activeSliderValue,
                activeSliderMin,
                activeSliderMax,
                activeSliderX,
                activeSliderY,
                activeSliderWidth);

            y += 60.0f;
        }
        else if (var.typeName.find("bool") != std::string::npos)
        {
            bool* v = (bool*)var.valuePtr;

            DrawCheckbox(renderer, 30.0f, y + 20.0f, v, ePtr, inspectorWindow);
            y += 50.0f;
        }
        else if (var.typeName.find("vec3") != std::string::npos)
        {
            glm::vec3* v = (glm::vec3*)var.valuePtr;

            DrawSlider(renderer, 30, y + 20, 200, &v->x, -10, 10,
                ePtr, inspectorWindow, draggingSlider, activeSliderValue,
                activeSliderMin, activeSliderMax,
                activeSliderX, activeSliderY, activeSliderWidth);

            DrawSlider(renderer, 30, y + 40, 200, &v->y, -10, 10,
                ePtr, inspectorWindow, draggingSlider, activeSliderValue,
                activeSliderMin, activeSliderMax,
                activeSliderX, activeSliderY, activeSliderWidth);

            DrawSlider(renderer, 30, y + 60, 200, &v->z, -10, 10,
                ePtr, inspectorWindow, draggingSlider, activeSliderValue,
                activeSliderMin, activeSliderMax,
                activeSliderX, activeSliderY, activeSliderWidth);

            y += 100;
        }
        else
        {
            DrawText(renderer, "(unsupported type)", 30, (int)(y + 20));
            y += 40.0f;
        }
    }
}
