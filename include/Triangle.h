#pragma once
#include "Vec2.h"
#include "Vec4.h"
#include "Mat4.h"
class Triangle {
public:
	
	Triangle();                                                       // 默认构造函数
	Triangle(Triangle& rhs);                                          // 拷贝构造函数
	Triangle(Vec4* points, Vec2* texCoords, Vec3* normals = NULL);
	~Triangle();
	Vec4 GetA();
	Vec4 GetB();
	Vec4 GetC();
	void SetA(Vec4& A);
	void SetB(Vec4& B);
	void SetC(Vec4& C);
	Vec3 Barycentric(Vec2 point);
	void Transform(const Mat4& mvp, int width, int height, bool print = false); // 完成MVP变换、透视除法和视口变换 
	void CalcWorldPoints(const Mat4& model);                           // 计算各顶点世界坐标
	Vec4* GetWorldPoints();                                            // 获取各顶点世界坐标
	Vec3* GetNormals();                                                // 获取各顶点法向量
	Vec2* GetTexCoords();                                              // 获取各顶点纹理坐标
	Vec4& operator[](int idx);
	const Vec4& operator[](int idx)const;
private:
	Vec4 points[3];                                                    // 顶点坐标
	Vec4 worldPoints[3];                                               // 世界坐标
	Vec4 oriPoints[3];                                                 // 最初的顶点坐标
	Vec2 texCoords[3];                                                 // 纹理坐标
	Vec3 normals[3];                                                   // 顶点法向量
};
