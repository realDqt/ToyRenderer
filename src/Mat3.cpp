#include "Mat3.h"
#include <assert.h>

// Default constructor
Mat3::Mat3()
{
	// Initialize a 3x3 storage container
	for (int i = 0; i < 3; ++i)data[i].resize(3);
}

// Copy constructor
Mat3::Mat3(const Mat3& rhs)
{
	// Initialize a 3x3 storage container
	for (int i = 0; i < 3; ++i)data[i].resize(3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			data[i][j] = rhs.data[i][j];
		}
	}
}

Mat3::Mat3(float value)
{
	// Initialize a 3x3 storage container
	for (int i = 0; i < 3; ++i)data[i].resize(3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (i == j)data[i][j] = value;
			else data[i][j] = 0.0f;
		}
	}
}

// Conversion constructor
Mat3::Mat3(const Mat4& rhs)
{
	// Initialize a 3x3 storage container
	for (int i = 0; i < 3; ++i)data[i].resize(3);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			data[i][j] = rhs[i][j];
		}
	}
}

Mat3::~Mat3()
{
}

Mat3& Mat3::operator=(const Mat3& rhs)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			data[i][j] = rhs.data[i][j];
		}
	}
	return *this;
}

std::vector<float>& Mat3::operator[](int idx)
{
	return data[idx];
}

const std::vector<float>& Mat3::operator[](int idx)const
{
	return data[idx];
}

Mat3 Mat3::Transpose()const
{
	Mat3 mt(1.0f);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mt[i][j] = data[j][i];
		}
	}
	return mt;
}

float Mat3::Det()const
{
	float a = data[0][0] * (data[1][1] * data[2][2] - data[2][1] * data[1][2]);
	float b = -data[0][1] * (data[1][0] * data[2][2] - data[2][0] * data[1][2]);
	float c = data[0][2] * (data[1][0] * data[2][1] - data[2][0] * data[1][1]);
	return a + b + c;
}

Mat3 Mat3::Inverse()const
{
	// Build the adjugate matrix
	Mat3 AStar(1.0f);
	float tem[4] = { 0.0f };
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			// Compute minor Mji
			// Copy the remaining elements into the temporary buffer
			int len = 0;
			for (int p = 0; p < 3; ++p) {
				for (int q = 0; q < 3; ++q) {
					if (p == j || q == i)continue;
					tem[len++] = data[p][q];
				}
			}
			float Mji = tem[0] * tem[3] - tem[1] * tem[2];
			AStar[i][j] = (i + j) & 1 ? -Mji : Mji;
		}
	}
	// Compute the determinant
	float det = Det();
	assert(det != 0);
	// Return the result
	return AStar / det;
}

Mat3 operator*(const Mat3& a, const Mat3& b)
{
	Mat3 res(1.0f);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			float sum = 0;
			for (int k = 0; k < 3; ++k) {
				sum += a[i][k] * b[k][j];
			}
			res[i][j] = sum;
		}
	}
	return res;
}

Mat3 operator+(const Mat3& a, const Mat3& b)
{
	Mat3 res(1.0f);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

Mat3 operator*(float k, const Mat3& M)
{
	Mat3 res(1.0f);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			res[i][j] = k * M[i][j];
		}
	}
	return res;
}

Mat3 operator/(const Mat3& M, float k)
{
	Mat3 res = 1.0 / k * M;
	return res;
}
