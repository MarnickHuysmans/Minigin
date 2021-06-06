#pragma once
inline float randF(float min, float max)
{
	const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	const float diff = max - min;
	const float r = random * diff;
	return min + r;
}