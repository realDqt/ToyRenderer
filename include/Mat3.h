#pragma once
#include "Mat4.h"
#include <vector>
class Mat4;
class Mat3 {
public:
	Mat3();                                 // Default constructor
	Mat3(const Mat3& rhs);                  // Copy constructor
	Mat3(float value);
	Mat3(const Mat4& rhs);                  // Conversion constructor
	~Mat3();
	Mat3& operator= (const Mat3& rhs);
	std::vector<float>& operator[](int idx);
	const std::vector<float>& operator[](int idx)const;
	Mat3 Transpose()const;
	Mat3 Inverse()const;
	float Det()const;
private:
	std::vector<float> data[3];
};

Mat3 operator*(const Mat3& a, const Mat3& b); // Matrix multiplication
Mat3 operator+(const Mat3& a, const Mat3& b); // Matrix addition
Mat3 operator*(float k, const Mat3& M);       // Scalar multiplication
Mat3 operator/(const Mat3& M, float k);       // Scalar division

