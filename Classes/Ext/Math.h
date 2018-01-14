#ifndef __COMMON_MATH_H__
#define __COMMON_MATH_H__

#include "cocos2d.h"

/*
 Math
*/

class Math
{
public:
	// math
	static const float PI;
	static float sin_d(float a);
	static float cos_d(float a);

	static float getRotation(const cocos2d::Point& from, const cocos2d::Point& to);

	// random
	static void randomize();
	static bool random();
	static int random(int max);
	static int random(int min, int max);
	static float random(float min, float max);
};

#endif //__COMMON_MATH_H__