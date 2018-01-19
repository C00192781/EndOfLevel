#include "Bloom.h"

Bloom::Bloom()
{
	PI = 3.14159265359;
}

Bloom::~Bloom()
{

}

void Bloom::getConvolution(int kernelRadius)
{
	double sum = 0.0f;
	// STANDARD DEVIATION
	// ignoring a Gaussian function beyond a radius of 3·sigma 
	// still leaves you with more than 97% of its total information
	double sigma = kernelRadius / 3.0f;

	//double sigma = 1.0f;
	//double mean = width / 2;


	//const float height = 5.0f;
	//const float width = height / 2;

	// FWHM
	/*const double width = sigma * sqrt(8 * log(2));
	const double height = width * 2;*/

	float sigmaSquared = pow(sigma, 2.0f);

	float height = 2 * kernelRadius + 1;

	Matrix kernel2D(height, Row(height));

	// accumulate
	for (int row = 0; row < kernel2D.size(); row++)
	{
		for (int col = 0; col < kernel2D[row].size(); col++)
		{
			kernel2D[row][col] =
				(1 / (2 * PI * sigmaSquared) *
					exp(-(pow(row, 2.0f) + pow(col, 2.0f)) / (2 * sigmaSquared)));
			sum += kernel2D[row][col];
		}
	}
	// normalize
	for (int row = 0; row < kernel2D.size(); row++)
	{
		for (int col = 0; col < kernel2D[row].size(); col++)
		{
			kernel2D[row][col] /= sum;
		}
	}

	cout << sum << endl;
}