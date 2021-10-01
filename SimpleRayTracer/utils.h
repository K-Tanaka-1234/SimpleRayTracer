#pragma once

inline double clamp(double x, double min, double max) 
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}