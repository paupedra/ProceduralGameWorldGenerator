#pragma once

class Position
{
public:

	Position()
	{

	}

	Position(float x, float y) :
		x(x),
		y(y) 
	{ }

	Position& operator+=(Position pos)
	{
		x += pos.x;
		y += pos.y;

		return *this;
	}

	Position& operator*(float i)
	{
		x *= i;
		y *= i;

		return *this;
	}

	float x = 0.f;
	float y = 0.f;
};