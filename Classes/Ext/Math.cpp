#include "Math.h"
#include <math.h>

USING_NS_CC;

const float Math::PI = 3.14159265f;

float Math::sin_d(float a)
{
	return sin(a * PI / 180);
}

float Math::cos_d(float a)
{
	return cos(a * PI / 180);
}

float Math::getRotation(const Point& from, const Point& to)
{
	Point d = to - from;
	float angle = atan2(d.x, d.y);
	return angle * 180 / PI;
}

void Math::randomize()
{
	srand(time(nullptr));
}

bool Math::random()
{
	return rand() % 2 == 0;
}

int Math::random(int max)
{
	return random(0, max);
}

int Math::random(int min, int max)
{
	if (min >= max)
		return min;
	return min + rand() % (max - min);
}

float Math::random(float min, float max)
{
	return min + (max - min) * (rand() % 4096) / 4095;
}
