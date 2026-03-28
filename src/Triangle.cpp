#include "Triangle.h"
#include "Global.h"
#include <assert.h>
#include <iostream>

// Default constructor
Triangle::Triangle()
{
}

// Copy constructor
Triangle::Triangle(Triangle& rhs)
{
	for (int i = 0; i < 3; ++i) {
		this->points[i] = rhs.points[i];
		this->worldPoints[i] = rhs.worldPoints[i];
		this->oriPoints[i] = rhs.oriPoints[i];
		this->texCoords[i] = rhs.texCoords[i];
	}
}

Triangle::Triangle(Vec4* points, Vec2* texCoords, Vec3* normals)
{
	for (int i = 0; i < 3; ++i) {
		if(points)this->oriPoints[i] = this->points[i] = points[i];
		if(texCoords)this->texCoords[i] = texCoords[i];
		//if(texCoords)std::cout << "Triangle texCoords: " << texCoords[i] << std::endl;
		if(normals)this->normals[i] = normals[i];
	}
}

Triangle::~Triangle()
{
}

Vec4 Triangle::GetA()
{
	return points[0];
}
Vec4 Triangle::GetB()
{
	return points[1];
}
Vec4 Triangle::GetC()
{
	return points[2];
}

void Triangle::SetA(Vec4& A)
{
	points[0] = A;
}
void Triangle::SetB(Vec4& B)
{
	points[1] = B;
}
void Triangle::SetC(Vec4& C)
{
	points[2] = C;
}

Vec4& Triangle::operator[](int idx)
{
	return points[idx];
}

const Vec4& Triangle::operator[](int idx)const
{
	return points[idx];
}

Vec3 Triangle::Barycentric(Vec2 point)
{
	// Normalize the homogeneous coordinate
	for (int i = 0; i < 3; ++i)points[i] = points[i] / points[i].W();

	// Read the vertex coordinates
	float x = point.X(), y = point.Y();
	float xa = points[0].X(), xb = points[1].X(), xc = points[2].X();
	float ya = points[0].Y(), yb = points[1].Y(), yc = points[2].Y();

	// Compute barycentric coordinates
	float alpha = -(x - xb) * (yc - yb) + (y - yb) * (xc - xb);
	alpha /= -(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb);

	float beta = -(x - xc) * (ya - yc) + (y - yc) * (xa - xc);
	beta /= -(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc);

	float gamma = 1 - alpha - beta;

	return Vec3(alpha, beta, gamma);
}

// Apply MVP, perspective divide, and viewport transform
void Triangle::Transform(const Mat4& mvp, int width, int height, bool print)
{
	// Apply the MVP transform
	if (print)std::cout << "after mvp: " << std::endl;
	for (int i = 0; i < 3; ++i) {
		points[i] = mvp * oriPoints[i];
		if (print)std::cout << points[i] << std::endl;
	}
	if (print)std::cout << std::endl;

	
	// Perform perspective divide
	if (print)std::cout << "after perspective divide: " << std::endl;
	for (int i = 0; i < 3; ++i) {
		assert(!FloatEqual(points[i].W(), 0.0f));
		points[i] = points[i] / points[i].W();
		if(print)std::cout << points[i] << std::endl;
	}
	if(print)std::cout << std::endl;
	

	// Apply viewport transform
	Vec3 scale(width / 2.0f, height / 2.0f, 1.0f);
	Vec3 translate(width / 2.0f, height / 2.0f, 0.0f);
	Mat4 viewport = Translate(translate) * Scale(scale);
	if(print)std::cout << "after viewport transform: " << std::endl;
	for (int i = 0; i < 3; ++i) {
		points[i] = viewport * points[i];
		if (print)std::cout << points[i] << std::endl;
	}
	if(print)std::cout << std::endl;
}

// Compute vertex positions in world space
void Triangle::CalcWorldPoints(const Mat4& model)
{
	for (int i = 0; i < 3; ++i) {
		worldPoints[i] = model * oriPoints[i];
	}
}

// Get vertex positions in world space
Vec4* Triangle::GetWorldPoints()
{
	return worldPoints;
}

// Get vertex normals
Vec3* Triangle::GetNormals()
{
	//std::cout << "GetNormals: " << normals[0] << std::endl;
	return normals;
}

// Get vertex texture coordinates
Vec2* Triangle::GetTexCoords()
{
	//std::cout << "GetTexCoords texCoords[0]: " << texCoords[0] << std::endl;
	return texCoords;
}

// Get the triangle face normal
Vec3 Triangle::GetPlaneNormal() const
{
	Vec4 v1 = worldPoints[1] - worldPoints[0];
	Vec4 v2 = worldPoints[2] - worldPoints[1];

	float x = v1.Y() * v2.Z() - v1.Z() * v2.Y();
	float y = v1.Z() * v2.X() - v1.X() * v2.Z();
	float z = v1.X() * v2.Y() - v1.Y() * v2.X();
	float length = std::sqrt(x * x + y * y + z * z);
	if (length > 1e-6f)
	{
		x /= length;
		y /= length;
		z /= length;
	}
	else
	{
		// Degenerate triangle: fall back to a zero normal
		x = 0.0f; y = 0.0f; z = 0.0f;
	}
	return Vec3(x, y, z);
}
