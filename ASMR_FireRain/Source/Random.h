#pragma once
#include <stdlib.h>

class Random
{
	Random() = delete;

public:

	static float range01() { return rand() / (float)RAND_MAX; }
	static float range(float minVal, float maxVal) { float t = range01(); return minVal + (maxVal - minVal) * t; }
	static unsigned int range(unsigned int minVal, unsigned int maxVal) 
	{  
		float t = range01();
		return (unsigned int)((float)minVal + (float)(maxVal - minVal) * t);
	}
};