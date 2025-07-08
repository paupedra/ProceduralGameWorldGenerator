#pragma once
#include "Position.h"

struct SDL_Renderer;

class Color
{
public:
	Color() {}

	Color(int r, int g, int b) :
		r(r),
		g(g),
		b(b)
	{

	}

	int r = 0, g = 0, b = 0;
};

class Entity
{
public: 
	Entity (Position initialPosition):
		position(initialPosition)
	{
		
	}

	void Render(SDL_Renderer* renderer, Position screenPos);

	void Move(Position positionVector);
	void MoveTo(Position newPosition);

	Position position;
	Color color;
};