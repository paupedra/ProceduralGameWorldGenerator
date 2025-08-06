#include <iostream>
#include <string>
#include "UISlider.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

void UISlider::Init(SDL_Renderer* renderer)
{

    font = TTF_OpenFont("Assets/Fonts/console.ttf", 23);
    if (!font) {
        printf("Failed to load font: %s\n", SDL_GetError());
    }

    SDL_Color color = { 255, 255, 255, 255 };
    textSurface = TTF_RenderText_Solid(font, string, 0, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    addTextSurface = TTF_RenderText_Solid(font, "+", 0, color);
    addTextTexture = SDL_CreateTextureFromSurface(renderer, addTextSurface);

    subTextSurface = TTF_RenderText_Solid(font, "-", 0, color);
    subTextTexture = SDL_CreateTextureFromSurface(renderer, subTextSurface);

    SDL_Color addCol = { 255, 0, 0, 255 };
    addColor = new SDL_Color(addCol);
    SDL_FRect add = { position.x + (textSurface->w / 2) + 5 ,position.y, buttonSize , buttonSize };
    addRect = new SDL_FRect(add);

    SDL_Color subtCol = { 255, 0, 0, 255 };
    subtColor = new SDL_Color(subtCol);
    SDL_FRect subt = { position.x - (textSurface->w / 2) - 5 - buttonSize, position.y, buttonSize, buttonSize };
    subRect = new SDL_FRect(subt);

    UpdateValueTexture(renderer);
}

void UISlider::Click(float x, float y, bool click, SDL_Renderer* renderer)
{
    addColor->r = 255;
    addColor->g = 0;
    addColor->b = 0;
    subtColor->r = 255;
    subtColor->g = 0;
    subtColor->b = 0;

    if ((x > addRect->x && x < addRect->x + addRect->w) && (y > addRect->y && y < addRect->y + addRect->h))
    {
        addColor->r = 255;
        addColor->g = 255;
        addColor->b = 0;

        if (click)
        {
            IncreaseValue(renderer);
            return;
        }
    }
    if ((x > subRect->x && x < subRect->x + subRect->w) && (y > subRect->y && y < subRect->y + subRect->h))
    {
        subtColor->r = 255;
        subtColor->g = 255;
        subtColor->b = 0;
        if (click)
        {
            DecreaseValue(renderer);
            return;
        }
    }
}

void UISlider::IncreaseValue(SDL_Renderer* renderer)
{
    if ((*currentValue + valueIncrements) > maxValue)
    {
        *currentValue = maxValue;
    }
    else
    {
        *currentValue += valueIncrements;
    }
	
    UpdateValueTexture(renderer);
}

void UISlider::DecreaseValue(SDL_Renderer* renderer)
{

    if ((*currentValue - valueIncrements) < minValue)
    {
        *currentValue = minValue;
    }
    else 
    {
        *currentValue -= valueIncrements;
    }

    UpdateValueTexture(renderer);
}

std::string floatToString2Decimal(float value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    return std::string(buffer);
}

void UISlider::Render(SDL_Renderer* renderer, SDL_Window* window)
{
    int windowW, windowH;
    SDL_GetWindowSize(window, &windowW, &windowH);

    int windowLimitMargin = windowW - 10;

    addRect->x = windowW - buttonSize, position.y, buttonSize, buttonSize;
    addRect->y = position.y;
    SDL_SetRenderDrawColor(renderer, addColor->r, addColor->g, addColor->b, 255);
    SDL_RenderFillRect(renderer, addRect);
    
    subRect->x = addRect->x - textSurface->w - 5 - buttonSize;
    subRect->y = position.y;
    SDL_SetRenderDrawColor(renderer, subtColor->r, subtColor->g, subtColor->b, 255);
    SDL_RenderFillRect(renderer, subRect);

	// Render the text
	SDL_FRect dest = { addRect->x - textSurface->w, position.y, textSurface->w, textSurface->h };
	SDL_RenderTexture(renderer, textTexture, NULL, &dest);
    SDL_FRect addDest = { addRect->x, addRect->y, buttonSize, buttonSize };
    SDL_RenderTexture(renderer, addTextTexture, NULL, &addDest);
    SDL_FRect subDest = { subRect->x , addRect->y, buttonSize, buttonSize };
    SDL_RenderTexture(renderer, subTextTexture, NULL, &subDest);

    SDL_Color color = { 255,255,255,255 };
    valueTextSurface = TTF_RenderText_Solid(font, floatToString2Decimal(*currentValue).c_str(), 0, color);
    valueTextTexture = SDL_CreateTextureFromSurface(renderer, valueTextSurface);

    SDL_FRect valueDest = { windowW - valueTextSurface->w, position.y + valueTextSurface->h, valueTextSurface->w, valueTextSurface->h };
    SDL_RenderTexture(renderer, valueTextTexture, NULL, &valueDest);
    SDL_DestroySurface(valueTextSurface);
    SDL_DestroyTexture(valueTextTexture);
}

void UISlider::UpdateValueTexture(SDL_Renderer* renderer)
{
    //if (valueTextTexture) SDL_DestroyTexture(valueTextTexture);

    //SDL_Color color = { 255,255,255,255 };

    //// Create new texture with updated text
    //valueTextSurface = TTF_RenderText_Blended(font, std::to_string(*currentValue).c_str(),0, color);
    //if (!valueTextSurface) {
    //    SDL_Log("Failed to create text surface: %s", SDL_GetError());
    //    return;
    //}

    //valueTextTexture = SDL_CreateTextureFromSurface(renderer, valueTextSurface);
    //SDL_DestroySurface(valueTextSurface);

    //if (!valueTextTexture) {
    //    SDL_Log("Failed to create texture: %s", SDL_GetError());
    //}
}

void UISlider::Clear()
{
    delete(font);

    delete(textSurface);
    delete(textTexture);

    delete(addTextSurface);
    delete(addTextTexture);

    delete(subTextSurface);
    delete(subTextTexture);

    delete(valueTextSurface);
    delete(valueTextTexture);

    delete(addButtonRect);
    delete(subtractButtonRect);
    delete(addRect);
    delete(subRect);

    delete(addColor);
    delete(subtColor);
}
