#pragma once
#include "Position.h"

struct SDL_Renderer;

class Color
{
public:
	Color() {}

	Color(int r, int g, int b, int a) :
		r(r),
		g(g),
		b(b),
		a(a)
	{

	}

	int r = 0, g = 0, b = 0, a = 0;
};

class Entity
{
public: 
	Entity (Position initialPosition):
		position(initialPosition)
	{
		
	}

	void Render(SDL_Renderer* renderer, Position screenPos, float zoom);

	void Move(Position positionVector);
	void MoveTo(Position newPosition);

	Position position;
	Position size;
	Color color;
};