#pragma once
#include "Vec3.h"
class Color {
public:
	Color();                                          // 默认构造函数
	Color(const Color& rhs);                          // 拷贝构造函数
	Color(float r, float g, float b, float a = 1.0f); // r, g, b, a 取值范围都是[0, 1]
	Color(float r, float a = 1.0f);
	Color(const Vec3& rhs, float a = 1.0f);           // 转换函数
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
	float r, g, b, a;                                 // 红蓝绿分量及透明度
};

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(float k, const Color& c);
Color operator/(const Color& c, float k);
Color operator*(const Color& a, const Color& b);      // 向量对应分量相乘
std::ostream& operator<<(std::ostream& out, const Color& color);

bool operator==(const Color& a, const Color& b);
bool operator!=(const Color& a, const Color& b);