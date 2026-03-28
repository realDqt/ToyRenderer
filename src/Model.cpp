#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Load a model from a file path
Model::Model(const char* filePath)
{
	std::ifstream in;
	// Open the file
	in.open(filePath, std::ifstream::in);
	// Return early if opening fails
	if (in.fail())return;
	// Store the contents of each OBJ line
	std::string line;
	while (!in.eof()) {
		// Read one line from the file
		std::getline(in, line);
		// Initialize iss with the line for later parsing
		std::istringstream iss(line.c_str());
		// Store characters we do not care about
		char trash;
		if (!line.compare(0, 2, "v ")) {
			// Read a vertex position
			iss >> trash;
			Vec3 vertex;
			for (int i = 0; i < 3; ++i)iss >> vertex[i];
			vertices.push_back(vertex);
		}
		else if (!line.compare(0, 3, "vn ")) {
			// Read a normal
			iss >> trash >> trash;
			Vec3 normal;
			for (int i = 0; i < 3; ++i)iss >> normal[i];
			normals.push_back(normal);
		}
		else if (!line.compare(0, 3, "vt ")) {
			// Read a texture coordinate
			iss >> trash >> trash;
			Vec2 texCoord;
			for (int i = 0; i < 2; ++i)iss >> texCoord[i];
			texCoords.push_back(texCoord);
		}
		else if (!line.compare(0, 2, "f ")) {
			// Read all index data for one face
			iss >> trash;
			std::vector<Vec3> face;
			// Only handle triangular faces
			for (int i = 0; i < 3; ++i) {
				Vec3 idx;
				iss >> idx[0] >> trash >> idx[1] >> trash >> idx[2];
				// Convert to zero-based indices
				for (int j = 0; j < 3; ++j)idx[j]--;
				face.push_back(idx);
			}
			faces.push_back(face);
		}
	}
	// Load the diffuse texture
	diffuseMap = new Image("nothing");
	// debug
	std::cout << "NumOfVertices: " << vertices.size() << std::endl;
	std::cout << "NumOfTexCoords: " << texCoords.size() << std::endl;
	std::cout << "NumOfFaces: " << faces.size() << std::endl;
}

Model::~Model()
{
}

// Get the number of vertices
int Model::NumOfVertices()
{
	return vertices.size();
}

// Get the number of faces
int Model::NumOfFaces()
{
	return faces.size();
}

// Get the vertex position at idx
Vec3 Model::Vertex(int idx)
{
	return vertices[idx];
}

// Get the normal at idx
Vec3 Model::Normal(int idx)
{
	return normals[idx];
}

// Get the texture coordinate at idx
Vec2 Model::TexCoord(int idx)
{
	return texCoords[idx];
}

// Get the indexed triplet for one face vertex
Vec3 Model::Vertex(int iFace, int iVertex)
{
	return faces[iFace][iVertex];
}

// Get the diffuse texture
Image* Model::GetDiffuseMap()
{
	return diffuseMap;
}
