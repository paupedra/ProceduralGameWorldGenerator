#pragma once

class Position
{
public:

	Position()
	{

	}

	Position(int x, int y) :
		x(x),
		y(y) 
	{ }

	Position& operator+=(Position pos)
	{
		x += pos.x;
		y += pos.y;

		return *this;
	}

	float x = 0.f;
	float y = 0.f;
};