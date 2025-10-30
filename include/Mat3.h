#pragma once
#include "Mat4.h"
#include <vector>
class Mat4;
class Mat3 {
public:
	Mat3();                                 // 默认构造函数
	Mat3(const Mat3& rhs);                  // 拷贝构造函数
	Mat3(float value);
	Mat3(const Mat4& rhs);                  // 转换函数
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

Mat3 operator*(const Mat3& a, const Mat3& b); // 矩阵乘法
Mat3 operator+(const Mat3& a, const Mat3& b); // 矩阵加法
Mat3 operator*(float k, const Mat3& M);       // 矩阵数乘
Mat3 operator/(const Mat3& M, float k);       // 矩阵数除

