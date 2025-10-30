#include "Color.h"
#include "Global.h"


// 默认构造函数
Color::Color()
{
	this->r = this->g = this->b = this->a = 0.0f;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(float r, float a)
{
	this->r = this->g = this->b = r;
	this->a = a;
}

// 拷贝构造函数
Color::Color(const Color& rhs)
{
	this->r = rhs.r;
	this->g = rhs.g;
	this->b = rhs.b;
	this->a = rhs.a;
}

// 转换函数
Color::Color(const Vec3& rhs, float a)
{
	this->r = rhs.X();
	this->g = rhs.Y();
	this->b = rhs.Z();
	this->a = 1.0f;
}

Color::~Color()
{
}

float Color::R()const
{
	return r;
}
float Color::G()const
{
	return g;
}
float Color::B()const
{
	return b;
}
float Color::A()const
{
	return a;
}

/*
Vec3 Color::RGB()const
{
	return *new Vec3(r, g, b);
}
*/

void Color::SetR(float r)
{
	this->r = r;
}
void Color::SetG(float g)
{
	this->g = g;
}
void Color::SetB(float b)
{
	this->b = b;
}
void Color::SetA(float a)
{
	this->a = a;
}

// 赋值运算符重载
Color& Color::operator=(const Color& rhs)
{
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	return *this;
}

float& Color::operator[](int idx)
{
	if (idx == 0)return r;
	else if (idx == 1)return g;
	else if (idx == 2)return b;
	else return a;
}

const float& Color::operator[](int idx)const 
{
	if (idx == 0)return r;
	else if (idx == 1)return g;
	else if (idx == 2)return b;
	else return a;
}

// 向量加法
Color operator+(const Color& a, const Color& b)
{
	return Color(a.R() + b.R(), a.G() + b.G(), a.B() + b.B(), a.A() + b.A());
}
// 向量减法
Color operator-(const Color& a, const Color& b)
{
	return Color(a.R() - b.R(), a.G() - b.G(), a.B() - b.B(), a.A() - b.A());
}
// 向量数乘
Color operator*(float k, const Color& c)
{
	return Color(k * c.R(), k * c.G(), k * c.B(), k * c.A());
}
// 向量数除
Color operator/(const Color& c, float k)
{
	return Color(c.R() / k, c.G() / k, c.B() / k, c.A() / k);
}
// 输出运算符重载
std::ostream& operator<<(std::ostream& out, const Color& color)
{
	out << color.R() << " " << color.G() << " " << color.B();
	return out;
}
// 颜色对应分量相乘
Color operator*(const Color& a, const Color& b)
{
	Color res(1.0f);
	for (int i = 0; i < 4; ++i) {
		res[i] = a[i] * b[i];
	}
	return res;
}

bool operator==(const Color& a, const Color& b)
{
	return FloatEqual(a.R(), b.R()) && FloatEqual(a.G(), b.G()) && FloatEqual(a.B(), b.B());
}
bool operator!=(const Color& a, const Color& b)
{
	return !(a == b);
}

