//DB
#include "Inspector.h"

// Simple text drawing stub
static void DrawText(SDL_Renderer* renderer, const std::string& text, int x, int y)
{
    // TODO: integrate SDL_ttf later
}

// Slider helper
static bool DrawSlider(SDL_Renderer* renderer,
    float x, float y,
    float width,
    float* value,
    float min, float max,
    const SDL_Event* e,
    bool& dragging,
    float*& activeValue,
    float& activeMin,
    float& activeMax,
    float& activeX,
    float& activeY,
    float& activeWidth)
{
    SDL_FRect bar = { x, y, width, 6.0f };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bar);

    float t = (*value - min) / (max - min);
    t = SDL_clamp(t, 0.0f, 1.0f);

    float handleX = x + t * width;

    SDL_FRect handle = { handleX - 5.0f, y - 4.0f, 10.0f, 14.0f };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &handle);

    if (e)
    {
        if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            float mx = e->button.x;
            float my = e->button.y;

            if (mx >= x && mx <= x + width && my >= y && my <= y + 6.0f)
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
        else if (e->type == SDL_EVENT_MOUSE_MOTION && dragging && activeValue == value)
        {
            float mx = e->motion.x;

            float newT = (mx - activeX) / activeWidth;
            newT = SDL_clamp(newT, 0.0f, 1.0f);

            *activeValue = activeMin + newT * (activeMax - activeMin);
            return true;
        }
    }

    return false;
}

// Checkbox helper
static bool DrawCheckbox(SDL_Renderer* renderer,
    float x, float y,
    bool* value,
    const SDL_Event* e)
{
    SDL_FRect box = { x, y, 16.0f, 16.0f };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &box);

    if (*value)
    {
        SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
        SDL_RenderFillRect(renderer, &box);
    }

    if (e && e->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        float mx = e->button.x;
        float my = e->button.y;

        if (mx >= x && mx <= x + 16.0f && my >= y && my <= y + 16.0f)
        {
            *value = !(*value);
            return true;
        }
    }

    return false;
}

void Inspector::HandleEvent(const SDL_Event& e)
{
    lastEvent = &e;

    // SDL3 keyboard event change
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_F1)
        open = !open;

    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        draggingSlider = false;
        activeSliderValue = nullptr;
    }

    if (e.type == SDL_EVENT_MOUSE_MOTION && draggingSlider && activeSliderValue)
    {
        float mx = e.motion.x;

        float newT = (mx - activeSliderX) / activeSliderWidth;
        newT = SDL_clamp(newT, 0.0f, 1.0f);

        *activeSliderValue = activeSliderMin + newT * (activeSliderMax - activeSliderMin);
    }
}

void Inspector::Draw(SDL_Renderer* renderer)
{
    if (!open) return;

    SDL_FRect panel = { 20.0f, 20.0f, 320.0f, 600.0f };
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 220);
    SDL_RenderFillRect(renderer, &panel);

    const auto& vars = ExportRegistry::Instance().GetAll();

    float y = 40.0f;
    for (size_t i = 0; i < vars.size(); ++i)
    {
        const auto& var = vars[i];

        DrawText(renderer, var.name, 30, (int)y);

        if (var.typeName == typeid(float).name())
        {
            float* v = (float*)var.valuePtr;
            DrawSlider(renderer, 30.0f, y + 20.0f, 200.0f,
                v, -10.0f, 10.0f,
                lastEvent,
                draggingSlider,
                activeSliderValue,
                activeSliderMin,
                activeSliderMax,
                activeSliderX,
                activeSliderY,
                activeSliderWidth);
            y += 60.0f;
        }
        else if (var.typeName == typeid(bool).name())
        {
            bool* v = (bool*)var.valuePtr;
            DrawCheckbox(renderer, 30.0f, y + 20.0f, v, lastEvent);
            y += 50.0f;
        }
        else
        {
            DrawText(renderer, "(unsupported type)", 30, (int)(y + 20.0f));
            y += 40.0f;
        }
    }

    lastEvent = nullptr;
}
