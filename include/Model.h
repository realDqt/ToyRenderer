#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Image.h"
#include <vector>
class Model {
public:
	Model(const char* filePath);         // 根据文件路径加载模型
	~Model(); 
	int NumOfVertices();                 // 顶点数
	int NumOfFaces();                    // 面数
	Vec3 Vertex(int idx);                // 索引为idx的位置坐标
	Vec3 Normal(int idx);                // 索引为idx的法向量
	Vec2 TexCoord(int idx);              // 索引为idx的纹理坐标
	Vec3 Vertex(int iFace, int iVertex); // 第iFace个面的第iVertex个顶点的各索引
	Image* GetDiffuseMap();              // 获取漫反射贴图
private:
	std::vector<Vec3> vertices;           // 顶点位置坐标       
	std::vector<Vec3> normals;            // 顶点法向量
	std::vector<Vec2> texCoords;          // 顶点纹理坐标
	std::vector<std::vector<Vec3>> faces; // 每个面的每个顶点的位置、纹理和法线的索引
	Image* diffuseMap;                    // 漫反射贴图
};
