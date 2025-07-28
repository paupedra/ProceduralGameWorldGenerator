#include "UISlider.h"
#include <SDL3/SDL.h>

void UISlider::IncreaseValue()
{
	currentValue += valueIncrements;
}

void UISlider::DecreaseValue()
{
	currentValue -= valueIncrements;
}

void UISlider::RenderButtons(SDL_Renderer* renderer)
{

}
