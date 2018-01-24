#pragma once

#include <vector>
#include <iostream>
#include "Texture.h"

using namespace std;


template <typename t>
t clamp(t x, t min, t max)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}

class Bloom
{
public:
	Bloom();
	~Bloom();

	//typedef std::vector<double> Row;
	//typedef std::vector<Row> Matrix;

	void gaussianKernel(float ** kernel, int kernelRadius);
	Uint32 * Blur(Texture* texture, int kernelRadius, int totalPixels, SDL_Surface* surface);
	Uint32 * BrightPass(Texture* texture, SDL_Surface* surface);

private:

	float PI;
};
