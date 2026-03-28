#pragma once
#define NOMINMAX
#include "Vec3.h"
#include "Vec4.h"
#include "Mat3.h"
#include "Mat4.h"
#include "Color.h"
#include "Triangle.h"
#include "Image.h"
#include "Camera.h"
#include "Screen.h"

//class Screen;
//class Camera;

const float INF = 3.40282e+38;
const float PI = 3.1415926f;

// Check whether k is in the range [min, max]
bool InRange(float k, float min, float max);

// Check whether k is in the range [min, max]
bool InRange(int k, int min, int max);

// Compute the dot product of two Vec3 values
float Dot(const Vec3& a, const Vec3& b);

// Compute the cross product of two Vec3 values
Vec3 Cross(const Vec3& a, const Vec3& b);

// Compute the length of a Vec3
float Length(const Vec3& v);

// Normalize a Vec3
Vec3 Normalize(const Vec3& v);

// Convert degrees to radians
float Radians(float angle);

// Compare two floating-point values for near-equality
bool FloatEqual(float a, float b);

// Translation helper
Mat4 Translate(const Vec3& translate);

// Rotation helper
Mat4 Rotate(const Vec3& n, float alpha);

// Scaling helper
Mat4 Scale(const Vec3& scale);

// Orthographic projection helper
Mat4 Ortho(float l, float r, float t, float b, float n, float f);

// Perspective projection helper
Mat4 Perspective(float fov, float ratio, float zNear, float zFar);

// Multiply a Mat4 by a Vec4
Vec4 operator* (const Mat4& M, const Vec4& v);

// Blin-Phong
Color BlinPhong(const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos);

// Blin-Phong with ShadowMap
Color BlinPhongShadow(const Screen& screen, const Mat4& projection, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos);

// Maximum helper
float Max(float a, float b);

