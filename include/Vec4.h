#pragma once
#include "Vec3.h"
#include <iostream>
class Vec3;
class Vec4 {
public:
	Vec4();                                                // Default constructor
	Vec4(const Vec4& rhs);                                 // Copy constructor
	Vec4(float x, float y, float z, float w = 1.0f); 
	Vec4(Vec3 v, float w = 1.0f);
	~Vec4();
	float X()const;
	float Y()const;
	float Z()const;
	float W()const;
	Vec3 XYZ()const;                                       // Get a Vec3 built from the xyz components
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);
	Vec4& operator= (const Vec4& rhs);                     // Assignment operator
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float x, y, z, w;
};

Vec4 operator+(const Vec4& a, const Vec4& b);               // Vector addition
Vec4 operator-(const Vec4& a, const Vec4& b);               // Vector subtraction
Vec4 operator*(float k, const Vec4& v);                     // Scalar multiplication
Vec4 operator/(const Vec4& v, float k);                     // Scalar division
Vec4 operator*(const Vec4& a, const Vec4& b);               // Component-wise multiplication
std::ostream& operator<<(std::ostream& out, const Vec4& v); // Stream output operator

