#pragma once
#include "Vec2.h"
#include "Vec4.h"
#include "Mat4.h"
class Triangle {
public:
	
	Triangle();                                                       // Default constructor
	Triangle(Triangle& rhs);                                          // Copy constructor
	Triangle(Vec4* points, Vec2* texCoords, Vec3* normals = NULL);
	~Triangle();
	Vec4 GetA();
	Vec4 GetB();
	Vec4 GetC();
	void SetA(Vec4& A);
	void SetB(Vec4& B);
	void SetC(Vec4& C);
	Vec3 Barycentric(Vec2 point);
	void Transform(const Mat4& mvp, int width, int height, bool print = false); // Apply MVP, perspective divide, and viewport transform
	void CalcWorldPoints(const Mat4& model);                           // Compute vertex positions in world space
	Vec4* GetWorldPoints();                                            // Get vertex positions in world space
	Vec3* GetNormals();                                                // Get the vertex normals
	Vec2* GetTexCoords();                                              // Get the vertex texture coordinates
	Vec4& operator[](int idx);                                         
	Vec3 GetPlaneNormal()const;										   // Get the triangle face normal
	const Vec4& operator[](int idx)const;
private:
	Vec4 points[3];                                                    // Screen-space coordinates
	Vec4 worldPoints[3];                                               // World-space coordinates
	Vec4 oriPoints[3];                                                 // Original vertex coordinates
	Vec2 texCoords[3];                                                 // Texture coordinates
	Vec3 normals[3];                                                   // Vertex normals
};
