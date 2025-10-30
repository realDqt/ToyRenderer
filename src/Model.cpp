#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// 根据文件路径加载模型
Model::Model(const char* filePath)
{
	std::ifstream in;
	// 打开文件
	in.open(filePath, std::ifstream::in);
	// 若打开失败，则返回
	if (in.fail())return;
	// 存储obj文件每一行的内容
	std::string line;
	while (!in.eof()) {
		// 读取文件的一行
		std::getline(in, line);
		// 用line初始化iss，方便后续的读取
		std::istringstream iss(line.c_str());
		// 存储文件中的不想要的字符
		char trash;
		if (!line.compare(0, 2, "v ")) {
			// 读取顶点的位置坐标
			iss >> trash;
			Vec3 vertex;
			for (int i = 0; i < 3; ++i)iss >> vertex[i];
			vertices.push_back(vertex);
		}
		else if (!line.compare(0, 3, "vn ")) {
			// 读取法向量
			iss >> trash >> trash;
			Vec3 normal;
			for (int i = 0; i < 3; ++i)iss >> normal[i];
			normals.push_back(normal);
		}
		else if (!line.compare(0, 3, "vt ")) {
			// 读取纹理坐标
			iss >> trash >> trash;
			Vec2 texCoord;
			for (int i = 0; i < 2; ++i)iss >> texCoord[i];
			texCoords.push_back(texCoord);
		}
		else if (!line.compare(0, 2, "f ")) {
			// 读取面的各顶点索引信息
			iss >> trash;
			std::vector<Vec3> face;
			// 仅考虑三角形面
			for (int i = 0; i < 3; ++i) {
				Vec3 idx;
				iss >> idx[0] >> trash >> idx[1] >> trash >> idx[2];
				// 索引减1
				for (int j = 0; j < 3; ++j)idx[j]--;
				face.push_back(idx);
			}
			faces.push_back(face);
		}
	}
	// 加载纹理贴图
	diffuseMap = new Image("nothing");
	// debug
	std::cout << "NumOfVertices: " << vertices.size() << std::endl;
	std::cout << "NumOfTexCoords: " << texCoords.size() << std::endl;
	std::cout << "NumOfFaces: " << faces.size() << std::endl;
}

Model::~Model()
{
}

// 顶点数
int Model::NumOfVertices()
{
	return vertices.size();
}

// 面数
int Model::NumOfFaces()
{
	return faces.size();
}

// 索引为idx的位置坐标
Vec3 Model::Vertex(int idx)
{
	return vertices[idx];
}

// 索引为idx的法向量
Vec3 Model::Normal(int idx)
{
	return normals[idx];
}

// 索引为idx的纹理坐标
Vec2 Model::TexCoord(int idx)
{
	return texCoords[idx];
}

// 第iFace个面的第iVertex个顶点的各索引
Vec3 Model::Vertex(int iFace, int iVertex)
{
	return faces[iFace][iVertex];
}

// 获取漫反射贴图
Image* Model::GetDiffuseMap()
{
	return diffuseMap;
}