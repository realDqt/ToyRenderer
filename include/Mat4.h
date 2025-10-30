#pragma once
#include "Mat3.h"
#include <vector>
#include <iostream>
class Mat3;
class Mat4 {
public:
	Mat4();                                  // 默认构造函数
	Mat4(const Mat4& rhs);                   // 拷贝构造函数
	Mat4(float value);
	Mat4(const Mat3& rhs);                   // 转换函数
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

Mat4 operator*(const Mat4& a, const Mat4& b); // 矩阵乘法
Mat4 operator+(const Mat4& a, const Mat4& b); // 矩阵加法
Mat4 operator*(float k, const Mat4& M);       // 矩阵数乘
Mat4 operator/(const Mat4& M, float k);       // 矩阵数除
std::ostream& operator<<(std::ostream& out, const Mat4& M);
