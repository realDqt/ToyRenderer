#include "Screen.h"
#include "Global.h"
#include <assert.h>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace {
constexpr int kSsaaSampleCount = 4;
constexpr float kBarycentricEpsilon = 1e-5f;
const Vec2 kSsaaSampleOffsets[kSsaaSampleCount] = {
	Vec2(-0.25f, 0.25f),
	Vec2(0.25f, 0.25f),
	Vec2(0.25f, -0.25f),
	Vec2(-0.25f, -0.25f)
};

bool IsInsideTriangle(const Vec3& bary)
{
	return InRange(bary.X(), -kBarycentricEpsilon, 1.0f + kBarycentricEpsilon) &&
		InRange(bary.Y(), -kBarycentricEpsilon, 1.0f + kBarycentricEpsilon) &&
		InRange(bary.Z(), -kBarycentricEpsilon, 1.0f + kBarycentricEpsilon);
}
}

Screen::Screen(int width, int height, int depth, Color bgColor)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->bgColor = bgColor;
	this->ssaaEnabled = true;
	
	// Allocate memory for the z-buffer
	zBuffer = new float[width * height];
	// Allocate memory for the depth map
	depthMap = new float[width * height];
	ssaaDepthBuffer = new float[width * height * kSsaaSampleCount];
	ssaaColorBuffer = new Color[width * height * kSsaaSampleCount];
	// Initialize to the farthest depth
	for (int i = 0; i < width * height; ++i)depthMap[i] = zBuffer[i] = -INF;
	for (int i = 0; i < width * height * kSsaaSampleCount; ++i) {
		ssaaDepthBuffer[i] = -INF;
		ssaaColorBuffer[i] = bgColor;
	}
}

Screen::~Screen()
{
	delete[] zBuffer;
	delete[] depthMap;
	delete[] ssaaDepthBuffer;
	delete[] ssaaColorBuffer;
}

void Screen::Create()
{
	initgraph(width, height);
	// Map [0, 1] to [0, 255]
	int r = static_cast<int>(bgColor.R() * 255);
	int g = static_cast<int>(bgColor.G() * 255);
	int b = static_cast<int>(bgColor.B() * 255);
	setbkcolor(RGB(r, g, b));
	cleardevice();
}

void Screen::Close()
{
	closegraph();
}

void Screen::SetSSAAEnabled(bool enabled)
{
	ssaaEnabled = enabled;
}

bool Screen::IsSSAAEnabled() const
{
	return ssaaEnabled;
}

int Screen::GetWidth()const
{
	return width;
}

int Screen::GetHeight()const
{
	return height;
}

float* Screen::GetDepthMap()const
{
	return depthMap;
}

int Screen::PixelIndex(int x, int y) const
{
	return (height - y - 1) * width + x;
}

int Screen::SampleIndex(int x, int y, int sampleIdx) const
{
	return PixelIndex(x, y) * kSsaaSampleCount + sampleIdx;
}

void Screen::ResolveSsaaPixel(int x, int y)
{
	Color color(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < kSsaaSampleCount; ++i) {
		color = color + ssaaColorBuffer[SampleIndex(x, y, i)];
	}
	SetPixel(x, y, color / static_cast<float>(kSsaaSampleCount));
}


// Get the color at the given pixel
Color Screen::GetPixel(int x, int y)
{
	COLORREF tem = getpixel(x, height - y);
	// Map [0, 255] to [0, 1]
	float r = static_cast<float>(GetRValue(tem) / 255.0f);
	float g = static_cast<float>(GetGValue(tem) / 255.0f);
	float b = static_cast<float>(GetBValue(tem) / 255.0f);
	Color res(r, g, b);
	return res;
}

// Set the color at the given pixel
void Screen::SetPixel(int x, int y, Color color)
{
	// Map [0, 1] to [0, 255]
	int r = static_cast<int>(color.R() * 255);
	int g = static_cast<int>(color.G() * 255);
	int b = static_cast<int>(color.B() * 255);
	putpixel(x, height - y, RGB(r, g, b));
}

// Rasterize a triangle whose vertices are already in screen space
void Screen::RasterizeTriangle(Triangle& triangle, Color* pointColors)
{

	//std::cout << "In Screen!" << std::endl;
	//for (int i = 0; i < 3; ++i)std::cout << triangle[i] << std::endl;
	//std::cout << std::endl;
	// Compute the triangle bounding box
	Vec2 bboxmin(INF, INF);
	Vec2 bboxmax(-INF, -INF);
	for (int i = 0; i < 3; ++i) {
		bboxmin.SetX(std::min(bboxmin.X(), triangle[i].X()));
		bboxmin.SetY(std::min(bboxmin.Y(), triangle[i].Y()));

		bboxmax.SetX(std::max(bboxmax.X(), triangle[i].X()));
		bboxmax.SetY(std::max(bboxmax.Y(), triangle[i].Y()));
	}
	//std::cout << "minx: " << bboxmin.X() << " miny: " << bboxmin.Y() << std::endl;
	//std::cout << "maxx: " << bboxmax.X() << " maxy: " << bboxmax.Y() << std::endl;
	// Subsample offsets
	float offsetX[4] = { -0.25f, 0.25f, 0.25f, -0.25f };
	float offsetY[4] = { 0.25f, 0.25f, -0.25f, -0.25f };
	// Traverse each pixel in the bounding box
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// Compute barycentric coordinates
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			// Check whether the point is inside the triangle
			if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))continue;
			// Interpolate depth
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// Check whether the point is inside the screen volume
			if (!InRange(z, -1.0, 1.0f))continue;
			// Depth test
			int idx = (height - y - 1) * width + x;
			if (z < zBuffer[idx])continue;
			// Update the z-buffer
			zBuffer[idx] = z;
			// Interpolate the color
			Color color = bary.X() * pointColors[0] + bary.Y() * pointColors[1] + bary.Z() * pointColors[2];
			// Shade the pixel
			SetPixel(x, y, color);
		}
	}
}

// Rasterize a triangle into the depth map
void Screen::RasterizeTriangleDepthMap(Triangle& triangle)
{
	// Compute the triangle bounding box
	Vec2 bboxmin(INF, INF);
	Vec2 bboxmax(-INF, -INF);
	for (int i = 0; i < 3; ++i) {
		bboxmin.SetX(std::min(bboxmin.X(), triangle[i].X()));
		bboxmin.SetY(std::min(bboxmin.Y(), triangle[i].Y()));

		bboxmax.SetX(std::max(bboxmax.X(), triangle[i].X()));
		bboxmax.SetY(std::max(bboxmax.Y(), triangle[i].Y()));
	}
	// Traverse each pixel in the bounding box
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// Compute barycentric coordinates
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			// Check whether the point is inside the triangle
			if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))continue;
			// Interpolate depth
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// Check whether the point is inside the screen volume
			if (!InRange(z, -1.0, 1.0f))continue;
			// Depth test
			int idx = (height - y - 1) * width + x;
			if (z < depthMap[idx])continue;
			// Update the depth map
			depthMap[idx] = z;
		}
	}
}

// Rasterize and shade a triangle in screen space using a bounding box
void Screen::RasterizeTriangleBoundingBox(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow)
{
	//for (int i = 0; i < 3; ++i)std::cout << triangle[i] << std::endl;
	// Compute the triangle bounding box
	Vec2 bboxmin(INF, INF);
	Vec2 bboxmax(-INF, -INF);
	for (int i = 0; i < 3; ++i) {
		bboxmin.SetX(std::min(bboxmin.X(), triangle[i].X()));
		bboxmin.SetY(std::min(bboxmin.Y(), triangle[i].Y()));

		bboxmax.SetX(std::max(bboxmax.X(), triangle[i].X()));
		bboxmax.SetY(std::max(bboxmax.Y(), triangle[i].Y()));
	}
	assert(bboxmax.X() > bboxmin.X() && bboxmax.Y() > bboxmin.Y());
	
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// Check whether the pixel is inside the screen
			if (!InRange(x, 0, width - 1) || !InRange(y, 0, height - 1))continue;
			if (!ssaaEnabled) {
				// Compute barycentric coordinates
				Vec3 bary = triangle.Barycentric(Vec2(x + .5f, y + .5f));
				// Check whether the point is inside the triangle
				if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))
					continue;
				// Depth test
				float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
				// Check whether the point is inside the screen volume
				if (!InRange(z, -1.0, 1.0f))continue;
				int idx = (height - y - 1) * width + x;
				assert(idx >= 0 && idx < width * height);
				if (z < zBuffer[idx])continue;
				// Update the z-buffer
				zBuffer[idx] = z;
				// Evaluate the lighting
				Color color(0.0f);
				if (shadow)color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				else color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				// Shade the pixel
				SetPixel(x, y, color);
				continue;
			}
			
			bool pixelUpdated = false;
			for (int sample = 0; sample < kSsaaSampleCount; ++sample) {
				Vec2 samplePoint(
					static_cast<float>(x + .5f) + kSsaaSampleOffsets[sample].X(),
					static_cast<float>(y + .5f) + kSsaaSampleOffsets[sample].Y()
				);
				Vec3 bary = triangle.Barycentric(samplePoint);
				if (!IsInsideTriangle(bary)) continue;

				float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
				if (!InRange(z, -1.0f, 1.0f)) continue;

				int sampleIdx = SampleIndex(x, y, sample);
				assert(sampleIdx >= 0 && sampleIdx < width * height * kSsaaSampleCount);
				if (z < ssaaDepthBuffer[sampleIdx]) continue;

				ssaaDepthBuffer[sampleIdx] = z;

				Color color(0.0f);
				if (shadow) {
					color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				else {
					color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				ssaaColorBuffer[sampleIdx] = color;
				pixelUpdated = true;
			}

			if (pixelUpdated) {
				ResolveSsaaPixel(x, y);
			}
		}
	}
}

// Rasterize and shade a triangle in screen space using scanlines
void Screen::RasterizeTriangleScanline(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow)
{
	// 1. Get the three triangle vertices and sort them by y
	// After sorting we have v0.y < v1.y < v2.y
	auto v0 = triangle[0];
	auto v1 = triangle[1];
	auto v2 = triangle[2];

	if (v0.Y() > v1.Y()) std::swap(v0, v1);
	if (v0.Y() > v2.Y()) std::swap(v0, v2);
	if (v1.Y() > v2.Y()) std::swap(v1, v2);

	int y0 = std::round(v0.Y());
	int y1 = std::round(v1.Y());
	int y2 = std::round(v2.Y());

	int total_height = y2 - y0;
	// A total height of 0 means the triangle is degenerate, so skip it
	if (total_height == 0) return;

	// 2. Sweep scanlines from the lowest y0 to the highest y2
	for (int y = y0; y <= y2; ++y) {
		// Clip against the viewport to avoid out-of-range access
		if (!InRange(y, 0, height - 1)) continue;

		// Decide whether the current scanline lies on the lower or upper half
		bool second_half = y > y1 || y1 == y0;
		int segment_height = second_half ? (y2 - y1) : (y1 - y0);

		// Compute the interpolation factors along the full height and the current segment
		float alpha = (float)(y - y0) / total_height;
		float beta = (segment_height == 0) ? 1.0f : (float)(y - (second_half ? y1 : y0)) / segment_height;

		// Compute the intersections with the two active edges
		// xA lies on the long edge (v0 -> v2)
		int xA = std::round(v0.X() + (v2.X() - v0.X()) * alpha);
		// xB lies on the short edge (v0 -> v1 or v1 -> v2)
		int xB = second_half ?
			std::round(v1.X() + (v2.X() - v1.X()) * beta) :
			std::round(v0.X() + (v1.X() - v0.X()) * beta);

		int x_left = std::min(xA, xB) - 1;
		int x_right = std::max(xA, xB) + 1;

		// 3. Process fragments only inside the valid [x_left, x_right] interval
		for (int x = x_left; x <= x_right; ++x) {
			// Clip against the viewport
			if (!InRange(x, 0, width - 1)) continue;

			if (!ssaaEnabled) {
				Vec3 bary = triangle.Barycentric(Vec2(static_cast<float>(x), static_cast<float>(y)));
				if (!IsInsideTriangle(bary)) continue;

				float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
				if (!InRange(z, -1.0f, 1.0f)) continue;

				int idx = PixelIndex(x, y);
				assert(idx >= 0 && idx < width * height);
				if (z < zBuffer[idx]) continue;

				zBuffer[idx] = z;

				Color color(0.0f);
				if (shadow) {
					color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				else {
					color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				SetPixel(x, y, color);
				continue;
			}

			bool pixelUpdated = false;
			for (int sample = 0; sample < kSsaaSampleCount; ++sample) {
				Vec2 samplePoint(
					static_cast<float>(x) + kSsaaSampleOffsets[sample].X(),
					static_cast<float>(y) + kSsaaSampleOffsets[sample].Y()
				);
				Vec3 bary = triangle.Barycentric(samplePoint);
				if (!IsInsideTriangle(bary)) continue;

				float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
				if (!InRange(z, -1.0f, 1.0f)) continue;

				int sampleIdx = SampleIndex(x, y, sample);
				assert(sampleIdx >= 0 && sampleIdx < width * height * kSsaaSampleCount);
				if (z < ssaaDepthBuffer[sampleIdx]) continue;

				ssaaDepthBuffer[sampleIdx] = z;

				Color color(0.0f);
				if (shadow) {
					color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				else {
					color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
				}
				ssaaColorBuffer[sampleIdx] = color;
				pixelUpdated = true;
			}

			if (pixelUpdated) {
				ResolveSsaaPixel(x, y);
			}
		}
	}
}

// Render a model
void Screen::RenderModel(const Mat4& m, const Mat4& p, const Mat4& mvp, Model& model, const Vec3& lightPos, const Camera& camera, bool shadow)
{
	int nFaces = model.NumOfFaces();
	Image* diffuseMap = model.GetDiffuseMap();
	//std::cout << "RenderModel m: " << std::endl << m << std::endl;
	//std::cout << "RenderModel m.inverse: " << std::endl << m.Inverse() << std::endl;
	Mat4 normalMatrix = m.Inverse().Transpose();
	//std::cout << "RenderModel normalMatrix: " << std::endl << normalMatrix << std::endl;
	for (int i = 0; i < nFaces; ++i) {
		// Build the triangle data
		Vec4* points = new Vec4[3];
		Vec2* texCoords = new Vec2[3];
		Vec3* normals = new Vec3[3];
		for (int j = 0; j < 3; ++j) {
			Vec3 idx = model.Vertex(i, j);
			for (int k = 0; k < 3; ++k)points[j][k] = model.Vertex(idx[0])[k];
			points[j][3] = 1.0f;
			texCoords[j] = model.TexCoord(idx[1]);
			//std::cout << "RenderModel texCoords: " << std::endl;
			//std::cout << texCoords[j] << std::endl;
			normals[j] = model.Normal(idx[2]);
			//std::cout << "RenderModel normals: " << std::endl;
			//std::cout << normals[j] << std::endl;
		}
		Triangle triangle(points, texCoords, normals);
		triangle.CalcWorldPoints(m);
		
		// Transform the triangle
		triangle.Transform(mvp, width, height);

		// Back-face culling
		Vec3 planeNormal = triangle.GetPlaneNormal();
		Vec3 cameraFront = camera.GetFront();
		if (Dot(planeNormal, cameraFront) > 0.f) continue;
		// Rasterize the triangle
		RasterizeTriangleBoundingBox(p, normalMatrix, diffuseMap, triangle, lightPos, camera.GetPosition(), shadow);

		// Release temporary memory
		delete[] points;
		delete[] texCoords;
		delete[] normals;
	}
}

// Render the original model
void Screen::RenderModel(Model& model)
{
	int nFaces = model.NumOfFaces();
	int nVertices = model.NumOfVertices();

	float xl = INF, xr = -INF, yl = INF, yr = -INF;
	for (int i = 0; i < nVertices; ++i) {
		xl = std::min(xl, model.Vertex(i)[0]);
		xr = std::max(xr, model.Vertex(i)[0]);
		yl = std::min(yl, model.Vertex(i)[1]);
		yr = std::max(yr, model.Vertex(i)[1]);
	}

	for (int i = 0; i < nFaces; ++i) {
		// Build the triangle data
		Vec4* points = new Vec4[3];
		Vec2* texCoords = new Vec2[3];
		for (int j = 0; j < 3; ++j) {
			Vec3 idx = model.Vertex(i, j);
			for (int k = 0; k < 3; ++k)points[j][k] = model.Vertex(idx[0])[k];
			points[j][3] = 1.0f;
			// Map the model to screen space
			points[j][0] = (points[j][0] - xl) / (xr - xl) * width;
			points[j][1] = (points[j][1] - yl) / (yr - yl) * height;
			texCoords[j] = model.TexCoord(idx[1]);
		}
		Triangle triangle(points, texCoords);

		// Rasterize the triangle
		Color* pointColors = new Color[3];
		for (int j = 0; j < 3; ++j) {
			pointColors[j] = model.GetDiffuseMap()->GetPixel(texCoords[j]);
		}
		RasterizeTriangle(triangle, pointColors);

		// Release temporary memory
		delete[] points;
		delete[] texCoords;
		delete[] pointColors;
	}
}

// Render from the light view to construct the depth map
void Screen::ConstructDepthMap(const Mat4& m, const Mat4& mvp, Model& model)
{
	int nFaces = model.NumOfFaces();
	for (int i = 0; i < nFaces; ++i) {
		// Build the triangle data
		Vec4* points = new Vec4[3];
		Vec2* texCoords = new Vec2[3];
		Vec3* normals = new Vec3[3];
		for (int j = 0; j < 3; ++j) {
			Vec3 idx = model.Vertex(i, j);
			for (int k = 0; k < 3; ++k)points[j][k] = model.Vertex(idx[0])[k];
			points[j][3] = 1.0f;
			texCoords[j] = model.TexCoord(idx[1]);
			normals[j] = model.Normal(idx[2]);
		}
		Triangle triangle(points, texCoords, normals);
		triangle.CalcWorldPoints(m);

		// Transform the triangle
		triangle.Transform(mvp, width, height);

		// Rasterize the triangle into the depth map
		RasterizeTriangleDepthMap(triangle);

		// Release temporary memory
		delete[] points;
		delete[] texCoords;
		delete[] normals;
	}
	/*
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int idx = i * width + j;
			if(depthMap[idx] != -INF)std::cout << depthMap[idx] << std::endl;
		}
	}
	*/
}

// Clear the z-buffer
void Screen::ClearZ()
{
	for (int i = 0; i < height * width; ++i)zBuffer[i] = -INF;
	for (int i = 0; i < height * width * kSsaaSampleCount; ++i) {
		ssaaDepthBuffer[i] = -INF;
		ssaaColorBuffer[i] = bgColor;
	}
}

// Clear the depth map
void Screen::ClearDepth()
{
	for (int i = 0; i < height * width; ++i)depthMap[i] = -INF;
}

