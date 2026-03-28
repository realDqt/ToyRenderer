#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Image.h"
#include <vector>
class Model {
public:
	Model(const char* filePath);         // Load a model from a file path
	~Model(); 
	int NumOfVertices();                 // Get the number of vertices
	int NumOfFaces();                    // Get the number of faces
	Vec3 Vertex(int idx);                // Get the vertex position at idx
	Vec3 Normal(int idx);                // Get the normal at idx
	Vec2 TexCoord(int idx);              // Get the texture coordinate at idx
	Vec3 Vertex(int iFace, int iVertex); // Get the indexed vertex triplet for a face vertex
	Image* GetDiffuseMap();              // Get the diffuse texture
private:
	std::vector<Vec3> vertices;           // Vertex positions
	std::vector<Vec3> normals;            // Vertex normals
	std::vector<Vec2> texCoords;          // Vertex texture coordinates
	std::vector<std::vector<Vec3>> faces; // Position/UV/normal indices for each face vertex
	Image* diffuseMap;                    // Diffuse texture
};
