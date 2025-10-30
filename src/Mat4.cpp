#include "Mat4.h"
#include "Global.h"
#include <assert.h>


// 默认构造函数
Mat4::Mat4()
{
	// 初始化容器大小为4
	for (int i = 0; i < 4; ++i)data[i].resize(4);
}

// 拷贝构造函数
Mat4::Mat4(const Mat4& rhs)
{
	// 初始化容器大小为4
	for (int i = 0; i < 4; ++i)data[i].resize(4);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			data[i][j] = rhs.data[i][j];
		}
	}
}

Mat4::Mat4(float value)
{
	// 初始化容器大小为4
	for (int i = 0; i < 4; ++i)data[i].resize(4);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j)data[i][j] = value;
			else data[i][j] = 0.0f;
		}
	}
}

// 转换函数
Mat4::Mat4(const Mat3& rhs)
{
	// 初始化容器大小为4
	for (int i = 0; i < 4; ++i)data[i].resize(4);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			data[i][j] = rhs[i][j];
		}
		data[i][3] = data[3][i] = 0.0f;
	}
	data[3][3] = 1.0f;
}

Mat4::~Mat4()
{
}

Mat4& Mat4::operator=(const Mat4& rhs)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			data[i][j] = rhs[i][j];
		}
	}
	return *this;
}

std::vector<float>& Mat4::operator[](int idx)
{
	return data[idx];
}

const std::vector<float>& Mat4::operator[](int idx)const
{
	return data[idx];
}

Mat4 Mat4::Transpose()const
{
	Mat4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[i][j] = data[j][i];
		}
	}
	return res;
}

Mat4 Mat4::Inverse()const
{
	/*
	std::cout << "Inverse data: " << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << data[i][j] << " ";
		}
		std::cout << std::endl;
	}
	*/
	// 构造伴随矩阵
	Mat4 AStar(1.0f);
	float tem[9] = { 0.0f };
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// 求Mji
			// 构造三阶方阵
			int len = 0;
			//std::cout << "j-i: " << j << "-" << i << std::endl;
			for (int p = 0; p < 4; ++p) {
				for (int q = 0; q < 4; ++q) {
					if (p == j || q == i)continue;
					//std::cout << p << "-" << q << ": " << data[p][q] << " ";
					tem[len++] = data[p][q];
				}
			}
			//std::cout << std::endl;
			//std::cout << "tem: " << std::endl;
			//for (int i = 0; i < len; ++i)std::cout << tem[i] << " ";
			//std::cout << std::endl;
			Mat3 M(1.0f);
			for (int p = 0; p < 3; ++p) {
				for (int q = 0; q < 3; ++q) {
					M[p][q] = tem[p * 3 + q];
				}
			}
			//std::cout << "M: " << std::endl << M << std::endl;
			float Mji = M.Det();
			//std::cout << "Mji: " << Mji << std::endl;
			AStar[i][j] = (i + j) & 1 ? -Mji : Mji;
		}
	}
	//std::cout << "Inverse Astar: " << std::endl << AStar << std::endl;
	// 求矩阵行列式的值
	float det = Det();
	//std::cout << "Inverse det: " << std::endl << det << std::endl;
	assert(!FloatEqual(det, 0.0f));
	// 返回结果
	return AStar / det;
}

float Mat4::Det()const
{
	float res = 0.0f;
	// 存储剩余元素
	float tem[9] = { 0 };
	for (int i = 0; i < 4; ++i) {
		int len = 0;
		for (int p = 1; p < 4; ++p) {
			for (int q = 0; q < 4; ++q) {
				if (q == i)continue;
				tem[len++] = data[p][q];
			}
		}
		// 构造Mat3
		Mat3 M(1.0f);
		for (int k = 0; k < 9; ++k) {
			M[k / 3][k % 3] = tem[k];
		}
		float det = M.Det();
		res += data[0][i] * (i & 1 ? -det : det);
	}
	return res;
}

Mat4 operator*(const Mat4& a, const Mat4& b)
{
	Mat4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			float sum = 0.0f;
			for (int k = 0; k < 4; ++k) {
				sum += a[i][k] * b[k][j];
			}
			res[i][j] = sum;
		}
	}
	return res;
}

Mat4 operator+(const Mat4& a, const Mat4& b)
{
	Mat4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[i][j] = a[i][j] + b[i][j];
		}
	}
	return res;
}

Mat4 operator*(float k, const Mat4& M)
{
	Mat4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[i][j] = k * M[i][j];
		}
	}
	return res;
}

Mat4 operator/(const Mat4& M, float k)
{
	Mat4 res = 1.0 / k * M;
	return res;
}

std::ostream& operator<<(std::ostream & out, const Mat4 & M)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			out << M[i][j] << " ";
		}
		if(i != 3)out << std::endl;
	}
	return out;
}