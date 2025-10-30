#include "Vec3.h"

// 默认构造函数
Vec3::Vec3()
{
	this->x = this->y = this->z = 0.0f;
}

// 转换函数
Vec3::Vec3(const Vec4& rhs)
{
	this->x = rhs.X();
	this->y = rhs.Y();
	this->z = rhs.Z();
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3::Vec3(float x)
{
	this->x = this->y = this->z = x;
}

// 拷贝构造函数
Vec3::Vec3(const Vec3& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
}

Vec3::~Vec3()
{
}

float Vec3::X()const
{
	return x;
}
float Vec3::Y()const
{
	return y;
}
float Vec3::Z()const
{
	return z;
}

void Vec3::SetX(float x)
{
	this->x = x;
}
void Vec3::SetY(float y)
{
	this->y = y;
}
void Vec3::SetZ(float z)
{
	this->z = z;
}

Vec3& Vec3::operator=(const Vec3& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	return *this;
}

float& Vec3::operator[](int idx)
{
	if (idx == 0)return x;
	else if (idx == 1)return y;
	else return z;
}

const float& Vec3::operator[](int idx)const
{
	if (idx == 0)return x;
	else if (idx == 1)return y;
	else return z;
}

// 向量加法
Vec3 operator+(const Vec3& a, const Vec3& b)
{
	Vec3 res(a.X() + b.X(), a.Y() + b.Y(), a.Z() + b.Z());
	return res;
}
// 向量减法
Vec3 operator-(const Vec3& a, const Vec3& b)
{
	Vec3 res(a.X() - b.X(), a.Y() - b.Y(), a.Z() - b.Z());
	return res;
}
// 向量数乘
Vec3 operator*(float k, const Vec3& v)
{
	Vec3 res(k * v.X(), k * v.Y(), k * v.Z());
	return res;
}
// 向量数除
Vec3 operator/(const Vec3& v, float k)
{
	Vec3 res(v.X() / k, v.Y() / k, v.Z() / k);
	return res;
}

std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	out << v.X() << " " << v.Y() << " " << v.Z();
	return out;
}