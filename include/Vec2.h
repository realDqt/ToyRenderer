#pragma once
#include <iostream>
class Vec2 {
public:
	Vec2();                                  // 默认构造函数
	Vec2(const Vec2& rhs);                   // 拷贝构造函数
	Vec2(float x, float y);
	Vec2(float x);
	~Vec2();
	float X()const;
	float Y()const;
	void SetX(float x);
	void SetY(float y);
	Vec2& operator= (const Vec2& rhs);        // 赋值运算符重载
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float x, y;
};

Vec2 operator+(const Vec2& a, const Vec2& b);  // 向量加法
Vec2 operator-(const Vec2& a, const Vec2& b);  // 向量减法
Vec2 operator*(float k, const Vec2& v);        // 向量数乘
Vec2 operator/(const Vec2& v, float k);        // 向量数除
std::ostream& operator<<(std::ostream& out, const Vec2& v);

