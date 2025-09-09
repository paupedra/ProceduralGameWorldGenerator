#include <iostream>
#include <string>
#include "UIElement.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

void UISlider::Init(SDL_Renderer* renderer, TTF_Font* font)
{
    this->font = font;

    SDL_Color color = { 255, 255, 255, 255 };
    textSurface = TTF_RenderText_Solid(font, string, 0, color);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    addTextSurface = TTF_RenderText_Solid(font, "+", 0, color);
    plusTextTexture = SDL_CreateTextureFromSurface(renderer, addTextSurface);

    subTextSurface = TTF_RenderText_Solid(font, "-", 0, color);
    minusTextTexture = SDL_CreateTextureFromSurface(renderer, subTextSurface);

    SDL_Color addCol = { 255, 0, 0, 255 };
    addColor = new SDL_Color(addCol);
    SDL_FRect add = { position.x + (textSurface->w / 2) + 5 ,position.y, buttonSize , buttonSize };
    sumDestRect = new SDL_FRect(add);

    SDL_Color subtCol = { 255, 0, 0, 255 };
    subtColor = new SDL_Color(subtCol);
    SDL_FRect subt = { position.x - (textSurface->w / 2) - 5 - buttonSize, position.y, buttonSize, buttonSize };
    minusDestRect = new SDL_FRect(subt);

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

    if ((x > sumDestRect->x && x < sumDestRect->x + sumDestRect->w) && (y > sumDestRect->y && y < sumDestRect->y + sumDestRect->h))
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
    if ((x > minusDestRect->x && x < minusDestRect->x + minusDestRect->w) && (y > minusDestRect->y && y < minusDestRect->y + minusDestRect->h))
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

    int windowLimitWithMargin = windowW - 10;

    sumDestRect->x = windowLimitWithMargin - buttonSize, position.y, buttonSize, buttonSize;
    sumDestRect->y = position.y;
    SDL_SetRenderDrawColor(renderer, addColor->r, addColor->g, addColor->b, 255);
    SDL_RenderFillRect(renderer, sumDestRect);
    
    minusDestRect->x = sumDestRect->x - textSurface->w - 5 - buttonSize;
    minusDestRect->y = position.y;
    SDL_SetRenderDrawColor(renderer, subtColor->r, subtColor->g, subtColor->b, 255);
    SDL_RenderFillRect(renderer, minusDestRect);

	// Render the text
	SDL_FRect dest = { sumDestRect->x - textSurface->w, position.y, textSurface->w, textSurface->h };
	SDL_RenderTexture(renderer, textTexture, NULL, &dest);
    SDL_FRect addDest = { sumDestRect->x +2, sumDestRect->y + 2, buttonSize, buttonSize };
    SDL_RenderTexture(renderer, plusTextTexture, NULL, &addDest);
    SDL_FRect subDest = { minusDestRect->x + 2, sumDestRect->y + 2, buttonSize, buttonSize };
    SDL_RenderTexture(renderer, minusTextTexture, NULL, &subDest);

    SDL_FRect valueDest = { windowLimitWithMargin - valueTextSurface->w, position.y + valueTextSurface->h + 2, valueTextSurface->w, valueTextSurface->h };
    SDL_RenderTexture(renderer, valueTextTexture, NULL, &valueDest);
}

void UISlider::UpdateValueTexture(SDL_Renderer* renderer)
{
    SDL_DestroySurface(valueTextSurface);
    SDL_DestroyTexture(valueTextTexture);

    SDL_Color color = { 255,255,255,255 };
    valueTextSurface = TTF_RenderText_Solid(font, floatToString2Decimal(*currentValue).c_str(), 0, color);
    valueTextTexture = SDL_CreateTextureFromSurface(renderer, valueTextSurface);
    if (valueTextTexture == NULL) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
    }
}

void UISlider::Clear()
{
    delete(font);

    delete(textSurface);
    delete(textTexture);

    delete(addTextSurface);
    delete(plusTextTexture);

    delete(subTextSurface);
    delete(minusTextTexture);

    delete(valueTextSurface);
    delete(valueTextTexture);

    delete(addButtonRect);
    delete(subtractButtonRect);
    delete(sumDestRect);
    delete(minusDestRect);

    delete(addColor);
    delete(subtColor);
}

void UIButton::Init(SDL_Renderer* renderer, TTF_Font* font)
{
    this->font = font;

    SDL_Color addCol = { 0, 255, 0, 255 };
    activeColor = new SDL_Color(addCol);
    addCol = { 255, 0, 0, 255 };
    inactiveColor = new SDL_Color(addCol);

    if (*value)
    {
        currentColor = activeColor;
    }
    else
    {
        currentColor = inactiveColor;
    }

    SDL_FRect add = { position.x ,position.y, buttonSize , buttonSize };
    buttonFRect = new SDL_FRect(add);

    SDL_Color color = { 255,255,255,255 };
    valueTextSurface = TTF_RenderText_Solid(font, string, 0, color);
    valueTextTexture = SDL_CreateTextureFromSurface(renderer, valueTextSurface);
}

void UIButton::Click(float x, float y, bool click, SDL_Renderer* renderer)
{
    if (click)
    {
        if ((x > buttonFRect->x && x < buttonFRect->x + buttonFRect->w) && (y > buttonFRect->y && y < buttonFRect->y + buttonFRect->h))
        {
            if (*value)
            {
                *value = false;
                currentColor = inactiveColor;
            }
            else
            {
                *value = true;
                currentColor = activeColor;
            }
        }
    }
}

void UIButton::Render(SDL_Renderer* renderer, SDL_Window* window)
{
    int windowW, windowH;
    SDL_GetWindowSize(window, &windowW, &windowH);

    int windowLimitMargin = windowW - 10;

    buttonFRect->x = windowLimitMargin - buttonSize, position.y, buttonSize, buttonSize;
    buttonFRect->y = position.y;
    SDL_SetRenderDrawColor(renderer, currentColor->r, currentColor->g, currentColor->b, 255);
    SDL_RenderFillRect(renderer, buttonFRect);

    SDL_FRect stringDest = { windowLimitMargin - valueTextSurface->w - buttonSize, position.y, valueTextSurface->w, valueTextSurface->h };
    SDL_RenderTexture(renderer, valueTextTexture, NULL, &stringDest);

}

void UIButton::Clear()
{
    delete(activeColor);
    delete(inactiveColor);
    delete(buttonRect);
    delete(valueTextSurface);
    delete(valueTextTexture);

}
