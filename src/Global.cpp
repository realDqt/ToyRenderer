#include "global.h"
#include <assert.h>
#include <algorithm>
#include <cmath>
#define NOMINMAX

// Check whether k is in the range [min, max]
bool InRange(float k, float min, float max)
{
	return k >= min && k <= max;
}

// Check whether k is in the range [min, max]
bool InRange(int k, int min, int max)
{
	return k >= min && k <= max;
}

// Compute the dot product of two Vec3 values
float Dot(const Vec3& a, const Vec3& b)
{
	return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
}

// Compute the cross product of two Vec3 values
Vec3 Cross(const Vec3& a, const Vec3& b)
{
	float x = a.Y() * b.Z() - a.Z() * b.Y();
	float y = a.Z() * b.X() - a.X() * b.Z();
	float z = a.X() * b.Y() - a.Y() * b.X();
	return Vec3(x, y, z);
}

// Compute the length of a Vec3
float Length(const Vec3& v)
{
	return sqrt(v.X() * v.X() + v.Y() * v.Y() + v.Z() * v.Z());
}

// Normalize a Vec3
Vec3 Normalize(const Vec3& v)
{
	float len = Length(v);
	assert(!FloatEqual(len, 0.0f));
	return v / len;
}

// Convert degrees to radians
float Radians(float angle)
{
	return angle / 180.0f * PI;
}

// Compare two floating-point values for near-equality
bool FloatEqual(float a, float b)
{
	return fabs(a - b) < 0.0001f;
}

// Translation helper
Mat4 Translate(const Vec3& translate)
{
	Mat4 res(1.0f);
	for (int i = 0; i < 3; ++i)res[i][3] = translate[i];
	return res;
}

// Rotation helper
Mat4 Rotate(const Vec3& n, float alpha)
{
	float cosAlpha = cos(alpha);
	float sinAlpha = sin(alpha);
	Mat3 I(1.0f);

	// Compute nn^T
	Mat3 nnT(1.0f);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			nnT[i][j] = n[i] * n[j];
		}
	}

	// Build the skew-symmetric matrix N
	Mat3 N(0.0f);
	N[0][1] = -n.Z();
	N[0][2] = n.Y();
	N[1][0] = n.Z();
	N[1][2] = -n.X();
	N[2][0] = -n.Y();
	N[2][1] = n.X();

	// Rodrigues' Rotate Formula
	Mat3 res = cosAlpha * I + (1 - cosAlpha) * nnT + sinAlpha * N;
	return Mat4(res);
}

// Scaling helper
Mat4 Scale(const Vec3& scale)
{
	Mat4 res(1.0f);
	for (int i = 0; i < 3; ++i)res[i][i] = scale[i];
	return res;
}

// Orthographic projection helper
Mat4 Ortho(float l, float r, float t, float b, float n, float f)
{
	// n and f are both negative here
	Vec3 scale(2.0f / (r - l), 2.0f / (t - b), 2.0f / (n - f));
	Vec3 translate(-(l + r) / 2.0f, -(t + b) / 2.0f, -(n + f) / 2.0f);
	Mat4 res = Scale(scale) * Translate(translate);
	return res;
}

// Perspective projection helper
Mat4 Perspective(float fov, float ratio, float zNear, float zFar)
{
	// ratio is the aspect ratio; zNear and zFar are positive
	// Compute l, r, t, b, n, and f
	float n = -zNear, f = -zFar;
	float t = tan(fov / 2.0f) * zNear;
	float b = -t;
	float r = ratio * (t - b) / 2.0f;
	float l = -r;

	// Build the perspective-to-orthographic matrix
	Mat4 Mp2o(0.0f);
	Mp2o[0][0] = n;
	Mp2o[1][1] = n;
	Mp2o[2][2] = n + f;
	Mp2o[2][3] = -n * f;
	Mp2o[3][2] = 1.0f;

	return Ortho(l, r, t, b, n, f) * Mp2o;
}

// Multiply a Mat4 by a Vec4
Vec4 operator*(const Mat4& M, const Vec4& v)
{
	Vec4 res(1.0f);
	for (int i = 0; i < 4; ++i) {
		float sum = 0;
		for (int j = 0; j < 4; ++j) {
			sum += M[i][j] * v[j];
		}
		res[i] = sum;
	}
	return res;
}

// Blin-Phong
Color BlinPhong(const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos)
{
	// Compute the world-space position
	Vec4* worldPoints = triangle.GetWorldPoints();
	// Normalize the homogeneous coordinate if needed
	//for (int i = 0; i < 3; ++i)worldPoints[i] = worldPoints[i] / worldPoints[i].W();
	Vec3 worldPos = bary.X() * worldPoints[0].XYZ() + bary.Y() * worldPoints[1].XYZ() + bary.Z() * worldPoints[2].XYZ();


	// Compute the interpolated normal
	Vec3* normals = triangle.GetNormals();
	Vec3 normal = normalMatrix * Vec4(bary.X() * normals[0] + bary.Y() * normals[1] + bary.Z() * normals[2], 0.0f);
	normal = Normalize(normal);
	//std::cout << "Blin-Phong Matrix: " <<std::endl << normalMatrix << std::endl;
	//std::cout << "Blin-Phong normal: " << normal << std::endl;
	

	// Compute the texture color
	Vec2* texCoords = triangle.GetTexCoords();
	Vec2 uv = bary.X() * texCoords[0] + bary.Y() * texCoords[1] + bary.Z() * texCoords[2];
	//std::cout << "Blin-Phong texCoords:" << std::endl;
	//for (int i = 0; i < 3; ++i) {
		//std::cout << texCoords[i] << std::endl;
	//}
	//std::cout << "Blin-Phong bary: " << bary << std::endl;
	//std::cout << "Blin-Phong uv: " << uv << std::endl;
	Color color = diffuseMap->GetPixel(uv);

	// Define the light intensity
	Color lightIntensity(1.0f);

	// Ambient term
	float ambi = 0.2;
	Color ambient = ambi * lightIntensity * color;

	// Diffuse term
	Vec3 lightDir = Normalize(lightPos - worldPos);
	float diff = std::max(Dot(lightDir, normal), 0.0f);
	Color diffuse = diff * lightIntensity * color;

	// Specular term
	Vec3 viewDir = Normalize(viewPos - worldPos);
	Vec3 halfVec = Normalize(lightDir + viewDir);
	// Specular exponent
	float shininess = 8.0f;
	float spec = std::pow(std::max(Dot(halfVec, normal), 0.0f), shininess);
	Color specular = spec * lightIntensity * color;

	Color res(0.0f);
	for (int i = 0; i < 3; ++i) {
		res[i] = std::min(ambient[i] + diffuse[i] + specular[i], 1.0f);
	}
	//std::cout << color << std::endl;
	return res;
	//return (normal + Vec3(1.0f)) / 2.0f;
}

// Blin-Phong with ShadowMap
Color BlinPhongShadow(const Screen& screen, const Mat4& projection, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& bary, const Vec3& lightPos, const Vec3& viewPos)
{
	// Compute the world-space position
	Vec4* worldPoints = triangle.GetWorldPoints();
	// Normalize the homogeneous coordinate if needed
	//for (int i = 0; i < 3; ++i)worldPoints[i] = worldPoints[i] / worldPoints[i].W();
	Vec3 worldPos = bary.X() * worldPoints[0].XYZ() + bary.Y() * worldPoints[1].XYZ() + bary.Z() * worldPoints[2].XYZ();


	// Compute the interpolated normal
	Vec3* normals = triangle.GetNormals();
	Vec3 normal = normalMatrix * Vec4(bary.X() * normals[0] + bary.Y() * normals[1] + bary.Z() * normals[2], 0.0f);
	normal = Normalize(normal);
	//std::cout << "Blin-Phong Matrix: " <<std::endl << normalMatrix << std::endl;
	//std::cout << "Blin-Phong normal: " << normal << std::endl;


	// Compute the texture color
	Vec2* texCoords = triangle.GetTexCoords();
	Vec2 uv = bary.X() * texCoords[0] + bary.Y() * texCoords[1] + bary.Z() * texCoords[2];
	//std::cout << "Blin-Phong texCoords:" << std::endl;
	//for (int i = 0; i < 3; ++i) {
		//std::cout << texCoords[i] << std::endl;
	//}
	//std::cout << "Blin-Phong bary: " << bary << std::endl;
	//std::cout << "Blin-Phong uv: " << uv << std::endl;
	Color color = diffuseMap->GetPixel(uv);

	// Define the light intensity
	Color lightIntensity(1.0f);

	// Ambient term
	float ambi = 0.2;
	Color ambient = ambi * lightIntensity * color;

	// Apply the light-space transform
	int width = screen.GetWidth(), height = screen.GetHeight();
	Mat4 view = Camera::LookAt(lightPos, Vec3(0.0f, -0.8f, -1.5f), Vec3(0.0f, 1.0f, 0.0f));
	//Mat4 projection = Perspective(Radians(90.0f), (float)width / height, 0.1f, 100.0f);
	Vec4 coord = projection * view * worldPos;
	// Perform perspective divide
	coord = coord / coord.W();
	// Apply viewport transform
	Vec3 translate(width / 2.0f, height / 2.0f, 0.0f), scale(width / 2.0f, height / 2.0f, 1.0f);
	Mat4 viewport = Translate(translate) * Scale(scale);
	coord = viewport * coord;
	int x = coord.X(), y = coord.Y();
	float z = coord.Z();

	// Determine whether the fragment is in shadow
	// pcf
	float* depthMap = screen.GetDepthMap();
	int cnt = 0;
	int validSamples = 0;
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			int nx = x + dx, ny = y + dy;
			if (!InRange(nx, 0, width - 1) || !InRange(ny, 0, height - 1)) continue;
			int idx = (height - ny - 1) * width + nx;
			++validSamples;
			if (z + 0.001f < depthMap[idx])++cnt;
		}
	}
	float k = validSamples > 0 ? (static_cast<float>(validSamples) - cnt) / validSamples : 1.0f;

	// Diffuse term
	Vec3 lightDir = Normalize(lightPos - worldPos);
	float diff = std::max(Dot(lightDir, normal), 0.0f);
	Color diffuse = diff * lightIntensity * color;

	// Specular term
	Vec3 viewDir = Normalize(viewPos - worldPos);
	Vec3 halfVec = Normalize(lightDir + viewDir);
	// Specular exponent
	float shininess = 8.0f;
	float spec = std::pow(std::max(Dot(halfVec, normal), 0.0f), shininess);
	Color specular = spec * lightIntensity * color;

	Color res(0.0f);
	for (int i = 0; i < 3; ++i) {
		res[i] = std::min(ambient[i] + k*(diffuse[i] + specular[i]), 1.0f);
	}
	//std::cout << color << std::endl;
	return res;
	//return (normal + Vec3(1.0f)) / 2.0f;
}

// max
float Max(float a, float b)
{
	return a > b ? a : b;
}


