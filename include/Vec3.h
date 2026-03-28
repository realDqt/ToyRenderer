#pragma once
#include "Vec4.h"
#include <iostream>
class Vec4;
class Vec3 {
public:
	Vec3();                                                 // Default constructor
	Vec3(const Vec3& rhs);                                  // Copy constructor
	Vec3(const Vec4& rhs);                                  // Conversion constructor
	Vec3(float x, float y, float z);
	Vec3(float x);
	~Vec3();
	float X()const;
	float Y()const;
	float Z()const;
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	Vec3& operator= (const Vec3& rhs);                      // Assignment operator
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float x, y, z;
};

Vec3 operator+(const Vec3& a, const Vec3& b);               // Vector addition
Vec3 operator-(const Vec3& a, const Vec3& b);               // Vector subtraction
Vec3 operator*(float k, const Vec3& v);                     // Scalar multiplication
Vec3 operator/(const Vec3& v, float k);                     // Scalar division
std::ostream& operator<<(std::ostream& out, const Vec3& v); // Stream output operator

