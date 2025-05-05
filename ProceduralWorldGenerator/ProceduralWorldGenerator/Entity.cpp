#include "Application.h"
#include "Entity.h"
#include <SDL3/SDL.h>

void Entity::Render(SDL_Renderer* renderer, Position screenPos)
{
	SDL_FRect rect;
	rect.h = 9;
	rect.w = 9;
	
	rect.x = (position.x + screenPos.x);
	rect.y = (position.y + screenPos.y);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void Entity::Move(Position positionVector)
{
	position += positionVector;
}

void Entity::MoveTo(Position newPosition)
{
	position = newPosition;
}
