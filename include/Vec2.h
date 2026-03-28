#pragma once
#include <iostream>
class Vec2 {
public:
	Vec2();                                  // Default constructor
	Vec2(const Vec2& rhs);                   // Copy constructor
	Vec2(float x, float y);
	Vec2(float x);
	~Vec2();
	float X()const;
	float Y()const;
	void SetX(float x);
	void SetY(float y);
	Vec2& operator= (const Vec2& rhs);        // Assignment operator
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float x, y;
};

Vec2 operator+(const Vec2& a, const Vec2& b);  // Vector addition
Vec2 operator-(const Vec2& a, const Vec2& b);  // Vector subtraction
Vec2 operator*(float k, const Vec2& v);        // Scalar multiplication
Vec2 operator/(const Vec2& v, float k);        // Scalar division
std::ostream& operator<<(std::ostream& out, const Vec2& v);

