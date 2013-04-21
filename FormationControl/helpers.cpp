#include <stdio.h>
#include <assert.h>
#include "helpers.h"

void f2i(float xf, float yf, int* xi, int* yi, int width, int height)
{
	assert(xi != NULL && yi != NULL);

	float xf_max = ((float)width) / ((float)height);
	float yf_max = 1;

	*yi = (int) (((yf_max - yf)/yf_max) * (height / 2));
	*xi = (int) (((xf_max + xf)/xf_max) * (width  / 2));
}

void i2f(float* xf, float* yf, int xi, int yi, int width, int height)
{
	assert(xf != NULL && yf != NULL);
	
	float xf_max = ((float)width) / ((float)height);
	float yf_max = 1;

	*yf = yf_max - ((float)(2*yi*yf_max))/((float)height);
	*xf = ((float)(2*xi*xf_max))/((float)width ) - xf_max;
}

void f2idx(float xf, int *xi, int width, int height)
{
	float xf_max = ((float)width) / ((float)height);
	*xi = (xf/xf_max) * (width/2);
}

void f2idy(float xy, int *xi, int width, int height)
{
	float yf_max = 1;
}

void i2fdx(float* xf, int *xi, int width, int height)
{
}

void i2fdy(float* yf, int *yi, int width, int height)
{
}
