#include "Color.h"
#include "Global.h"


// Default constructor
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

// Copy constructor
Color::Color(const Color& rhs)
{
	this->r = rhs.r;
	this->g = rhs.g;
	this->b = rhs.b;
	this->a = rhs.a;
}

// Conversion constructor
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

// Assignment operator
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

// Vector addition
Color operator+(const Color& a, const Color& b)
{
	return Color(a.R() + b.R(), a.G() + b.G(), a.B() + b.B(), a.A() + b.A());
}
// Vector subtraction
Color operator-(const Color& a, const Color& b)
{
	return Color(a.R() - b.R(), a.G() - b.G(), a.B() - b.B(), a.A() - b.A());
}
// Scalar multiplication
Color operator*(float k, const Color& c)
{
	return Color(k * c.R(), k * c.G(), k * c.B(), k * c.A());
}
// Scalar division
Color operator/(const Color& c, float k)
{
	return Color(c.R() / k, c.G() / k, c.B() / k, c.A() / k);
}
// Stream output operator
std::ostream& operator<<(std::ostream& out, const Color& color)
{
	out << color.R() << " " << color.G() << " " << color.B();
	return out;
}
// Component-wise color multiplication
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

