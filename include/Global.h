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

class Screen;
class Camera;

const float INF = 3.40282e+38;
const float PI = 3.1415926f;

// 判断k是否属于区间[min, max]
bool InRange(float k, float min, float max);

// 判断k是否属于区间[min, max]
bool InRange(int k, int min, int max);

// 计算Vec3内积
float Dot(const Vec3& a, const Vec3& b);

// 计算Vec3外积
Vec3 Cross(const Vec3& a, const Vec3& b);

// 计算Vec3长度
float Length(const Vec3& v);

// 标准化Vec3
Vec3 Normalize(const Vec3& v);

// 角度转弧度
float Radians(float angle);

// 判断两个浮点数相等
bool FloatEqual(float a, float b);

// 平移函数
Mat4 Translate(const Vec3& translate);

// 旋转函数
Mat4 Rotate(const Vec3& n, float alpha);

// 缩放函数
Mat4 Scale(const Vec3& scale);

// 正交投影
Mat4 Ortho(float l, float r, float t, float b, float n, float f);

// 透视投影
Mat4 Perspective(float fov, float ratio, float zNear, float zFar);

// Mat4×Vec4
Vec4 operator* (const Mat4& M, const Vec4& v);

// Blin-Phong
Color BlinPhong(const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos);

// Blin-Phong with ShadowMap
Color BlinPhongShadow(const Screen& screen, const Mat4& projection, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos);

// max
float Max(float a, float b);

