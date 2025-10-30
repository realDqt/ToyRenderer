#pragma once
#include "Vec4.h"
#include <iostream>
class Vec4;
class Vec3 {
public:
	Vec3();                                                 // 默认构造函数
	Vec3(const Vec3& rhs);                                  // 拷贝构造函数
	Vec3(const Vec4& rhs);                                  // 转换函数
	Vec3(float x, float y, float z);
	Vec3(float x);
	~Vec3();
	float X()const;
	float Y()const;
	float Z()const;
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	Vec3& operator= (const Vec3& rhs);                      // 赋值运算符重载
	float& operator[](int idx);
	const float& operator[](int idx)const;
private:
	float x, y, z;
};

Vec3 operator+(const Vec3& a, const Vec3& b);               // 向量加法
Vec3 operator-(const Vec3& a, const Vec3& b);               // 向量减法
Vec3 operator*(float k, const Vec3& v);                     // 向量数乘
Vec3 operator/(const Vec3& v, float k);                     // 向量数除
std::ostream& operator<<(std::ostream& out, const Vec3& v); // 输出运算符重载

