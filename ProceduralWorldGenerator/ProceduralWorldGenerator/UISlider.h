#pragma once

struct SDL_Renderer;

class UISlider
{
	char* string;
	float slideMaxValue = 0.f;
	float slideMinValue = 100.f;

	float currentValue = 0.f;
	float valueIncrements = 1.f;

	void IncreaseValue();
	void DecreaseValue();
	
	void RenderButtons(SDL_Renderer* renderer);
};