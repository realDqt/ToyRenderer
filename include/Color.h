#pragma once
#include "Vec3.h"
class Color {
public:
	Color();                                          // Default constructor
	Color(const Color& rhs);                          // Copy constructor
	Color(float r, float g, float b, float a = 1.0f); // r, g, b, and a are all in [0, 1]
	Color(float r, float a = 1.0f);
	Color(const Vec3& rhs, float a = 1.0f);           // Conversion constructor
	~Color();
	float R()const;
	float G()const;
	float B()const;
	float A()const;
	//Vec3 RGB()const;
	void SetR(float r);
	void SetG(float g);
	void SetB(float b);
	void SetA(float a);
	Color& operator=(const Color& rhs);
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float r, g, b, a;                                 // Red, green, blue, and alpha channels
};

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(float k, const Color& c);
Color operator/(const Color& c, float k);
Color operator*(const Color& a, const Color& b);      // Component-wise multiplication
std::ostream& operator<<(std::ostream& out, const Color& color);

bool operator==(const Color& a, const Color& b);
bool operator!=(const Color& a, const Color& b);
