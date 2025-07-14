#include "Application.h"
#include "Entity.h"

void Entity::Render(SDL_Renderer* renderer, Position screenPos, float zoom)
{
	SDL_FRect rect;
	rect.h = size.y * zoom;
	rect.w = size.x * zoom;
	
	rect.x = (position.x + screenPos.x) * zoom;
	rect.y = (position.y + screenPos.y) * zoom;
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
