#include "mathUtils.h"

const float EPSILON = 1E-5;

float Max(float a, float b)
{
	return (a>b)?a:b;
}

float Min(float a, float b)
{
	return (a<b)?a:b;
}

float Norm(float a, float b)
{
  return sqrt(a * a + b * b);
}

float ScolarProd(float x1, float y1, float x2, float y2)
{
	return x1 * x2 + y1 * y2;
}

int Sign(float value)
{
	if(fabs(value) < EPSILON) return 0;
	else if(value > 0) return 1;
	else return -1;
}
