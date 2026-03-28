#pragma once
#include "Mat3.h"
#include <vector>
#include <iostream>
class Mat3;
class Mat4 {
public:
	Mat4();                                  // Default constructor
	Mat4(const Mat4& rhs);                   // Copy constructor
	Mat4(float value);
	Mat4(const Mat3& rhs);                   // Conversion constructor
	~Mat4();
	Mat4& operator= (const Mat4& rhs);
	std::vector<float>& operator[](int idx);
	const std::vector<float>& operator[](int idx)const;
	Mat4 Transpose()const;
	Mat4 Inverse()const;
	float Det()const;
private:
	std::vector<float> data[4];
};

Mat4 operator*(const Mat4& a, const Mat4& b); // Matrix multiplication
Mat4 operator+(const Mat4& a, const Mat4& b); // Matrix addition
Mat4 operator*(float k, const Mat4& M);       // Scalar multiplication
Mat4 operator/(const Mat4& M, float k);       // Scalar division
std::ostream& operator<<(std::ostream& out, const Mat4& M);
