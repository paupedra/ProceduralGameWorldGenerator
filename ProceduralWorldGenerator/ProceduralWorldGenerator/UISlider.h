#pragma once
#include "Position.h"

struct TTF_Font;
struct SDL_Renderer;
struct SDL_Window;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_FRect;
struct SDL_Color;

class UISlider
{
public:
	UISlider(float* value) :
		currentValue(value) {
	};

	const char* string = "";
	float maxValue = 0.f;
	float minValue = 100.f;

	float* currentValue = nullptr;
	float valueIncrements = 1.f;

	Position position;

	TTF_Font* font = nullptr;
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	SDL_Surface* addTextSurface = nullptr;
	SDL_Texture* plusTextTexture = nullptr;

	SDL_Surface* subTextSurface = nullptr;
	SDL_Texture* minusTextTexture = nullptr;

	SDL_Surface* valueTextSurface = nullptr;
	SDL_Texture* valueTextTexture = nullptr;

	int buttonSize = 18;
	SDL_Rect* addButtonRect = nullptr;
	SDL_Rect* subtractButtonRect = nullptr;
	SDL_FRect* sumDestRect = nullptr;
	SDL_FRect* minusDestRect = nullptr;
	SDL_Color* addColor = nullptr;
	SDL_Color* subtColor = nullptr;

	void Init(SDL_Renderer* renderer, TTF_Font* font);

	void Click(float x, float y, bool click, SDL_Renderer* renderer = nullptr);

	void IncreaseValue(SDL_Renderer* renderer);
	void DecreaseValue(SDL_Renderer* renderer);
	
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	void UpdateValueTexture(SDL_Renderer* renderer);

	void Clear();
};

class UIButton
{
public:
	UIButton(bool* value) :
		value(value) {
	};

	Position position;

	bool* value = nullptr;

	const char* string = "";

	int buttonSize = 18;
	SDL_Rect* buttonRect = nullptr;
	SDL_FRect* buttonFRect = nullptr;
	SDL_Color* activeColor = nullptr;
	SDL_Color* inactiveColor = nullptr;
	SDL_Color* currentColor = nullptr;

	TTF_Font* font = nullptr;

	SDL_Surface* valueTextSurface = nullptr;
	SDL_Texture* valueTextTexture = nullptr;

	void Init(SDL_Renderer* renderer, TTF_Font* font);
	void Click(float x, float y, bool click, SDL_Renderer* renderer = nullptr);
	void Render(SDL_Renderer* renderer, SDL_Window* window);
	void Clear();
};