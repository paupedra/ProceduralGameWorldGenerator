#pragma once

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