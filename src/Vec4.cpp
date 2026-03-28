#include "Vec4.h"

// Default constructor
Vec4::Vec4()
{
	this->x = this->y = this->z = this->w = 0.0f;
}

Vec4::Vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4::Vec4(Vec3 v, float w)
{
	this->x = v.X();
	this->y = v.Y();
	this->z = v.Z();
	this->w = w;
}

// Copy constructor
Vec4::Vec4(const Vec4& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;
}

Vec4::~Vec4()
{
}

float Vec4::X()const
{
	return x;
}
float Vec4::Y()const
{
	return y;
}
float Vec4::Z()const
{
	return z;
}
float Vec4::W()const
{
	return w;
}

// Get a Vec3 built from the xyz components
Vec3 Vec4::XYZ()const
{
	return Vec3(x, y, z);
}

void Vec4::SetX(float x)
{
	this->x = x;
}
void Vec4::SetY(float y)
{
	this->y = y;
}
void Vec4::SetZ(float z)
{
	this->z = z;
}
void Vec4::SetW(float w)
{
	this->w = w;
}

Vec4& Vec4::operator=(const Vec4& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;
	return *this;
}

float& Vec4::operator[](int idx)
{
	if (idx == 0)return x;
	else if (idx == 1)return y;
	else if (idx == 2)return z;
	else return w;
}

const float& Vec4::operator[](int idx)const
{
	if (idx == 0)return x;
	else if (idx == 1)return y;
	else if (idx == 2)return z;
	else return w;
}

// Stream output operator
std::ostream& operator<<(std::ostream& out, const Vec4& v)
{
	out << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
	return out;
}

// Vector addition
Vec4 operator+(const Vec4& a, const Vec4& b)
{
	Vec4 res(a.X() + b.X(), a.Y() + b.Y(), a.Z() + b.Z(), a.W() + b.W());
	return res;
}
// Vector subtraction
Vec4 operator-(const Vec4& a, const Vec4& b)
{
	Vec4 res(a.X() - b.X(), a.Y() - b.Y(), a.Z() - b.Z(), a.W() - b.W());
	return res;
}
// Scalar multiplication
Vec4 operator*(float k, const Vec4& v)
{
	Vec4 res(k * v.X(), k * v.Y(), k * v.Z(), k * v.W());
	return res;
}
// Scalar division
Vec4 operator/(const Vec4& v, float k)
{
	Vec4 res(v.X() / k, v.Y() / k, v.Z() / k, v.W() / k);
	return res;
}
// Component-wise multiplication
Vec4 operator*(const Vec4& a, const Vec4& b)
{
	Vec4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		res[i] = a[i] * b[i];
	}
	return res;
}
