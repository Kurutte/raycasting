#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "v3math.h"

void v3_from_points(float *dst, float *a, float *b)
{
	float *vector = dst, *pointOne = a, *pointTwo = b;
	vector[0] = pointTwo[0] - pointOne[0];
	vector[1] = pointTwo[1] - pointOne[1];
	vector[2] = pointTwo[2] - pointOne[2];
	dst = vector;
}

void v3_add(float *dst, float *a, float *b)
{
	float *vectorOne = dst, *vectorTwo = a, *vectorThree = b;
	vectorOne[0] = vectorTwo[0] + vectorThree[0];
	vectorOne[1] = vectorTwo[1] + vectorThree[1];
	vectorOne[2] = vectorTwo[2] + vectorThree[2];
	dst = vectorOne;
}

void v3_subtract(float *dst, float *a, float *b)
{
	float *vectorOne = dst, *vectorTwo = a, *vectorThree = b;
	vectorOne[0] = vectorTwo[0] - vectorThree[0];
	vectorOne[1] = vectorTwo[1] - vectorThree[1];
	vectorOne[2] = vectorTwo[2] - vectorThree[2];
	dst = vectorOne;
}

float v3_dot_product(float *a, float *b)
{
	float *vectorOne = a, *vectorTwo = b;
	return vectorOne[0]*vectorTwo[0]+vectorOne[1]*vectorTwo[1]+vectorOne[2]*vectorTwo[2];
}

void v3_cross_product(float *dst, float *a, float *b)
{
	//x = 0 y = 1 z = 2 in reference to the picture from the slides
	float *vectorOne = dst, *vectorTwo = a, *vectorThree = b;
	vectorOne[0] = vectorTwo[1]*vectorThree[2] - vectorTwo[2]*vectorThree[1];
	vectorOne[1] = vectorTwo[2]*vectorThree[0] - vectorTwo[0]*vectorThree[2];
	vectorOne[2] = vectorTwo[0]*vectorThree[1] - vectorTwo[1]*vectorThree[0];
	dst = vectorOne;
}

void v3_scale(float *dst, float s)
{
	float *vectorOne = dst;
	vectorOne[0] *= s;
	vectorOne[1] *= s;
	vectorOne[2] *= s;
}

float v3_angle(float *a, float *b)
{

	return acos(v3_angle_quick(a,b));
}

float v3_angle_quick(float *a, float *b)
{
	float numerator = v3_dot_product(a, b);
	float denominator = v3_length(a)*v3_length(b);
	return numerator/denominator;
}

void v3_reflect(float *dst, float *v, float *n)
{ 
	//dst = v - 2(v*n)n
	float *vectorOne = dst, *vectorTwo = v, *vectorThree = n;
	vectorOne[0] = vectorTwo[0] - 2*(vectorTwo[0] * vectorThree[0]) * vectorThree[0];
	vectorOne[1] = vectorTwo[1] - 2*(vectorTwo[1] * vectorThree[1]) * vectorThree[1];
	vectorOne[2] = vectorTwo[2] - 2*(vectorTwo[2] * vectorThree[2]) * vectorThree[2];
}

float v3_length(float *a)
{
	float *vectorOne = a;
	float sum = vectorOne[0]*vectorOne[0] + vectorOne[1]*vectorOne[1] + vectorOne[2]*vectorOne[2];
	return sqrt(sum);
}

void v3_normalize(float *dst, float *a)
{
	float *vectorOne = dst, *vectorTwo = a;
	vectorOne[0]= vectorTwo[0] / v3_length(a);
	vectorOne[1]= vectorTwo[1] / v3_length(a);
	vectorOne[2]= vectorTwo[2] / v3_length(a);
}

bool v3_equals(float *a, float *b, float tolerance)
{
	bool equal = true;

	for(int i = 0; i < 3; i++)
	{
		if(fabs(a[i]-b[i]) >= tolerance)
		{
			equal = false;
		}
	}

	return equal;
}

bool equals(float a, float b, float tolerance)
{
	if(fabs(a-b) >= tolerance)
	{
		return false;
	}
	return true;
}