#pragma once

#include <vector>
#include <iostream>

#include "Texture.h"

using namespace std;

class Bloom
{
public:
	Bloom();
	~Bloom();

	typedef std::vector<double> Row;
	typedef std::vector<Row> Matrix;

	void getConvolution(int kernelRadius);

private:

	float PI;
};
