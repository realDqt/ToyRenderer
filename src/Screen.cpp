#include "Screen.h"
#include "Global.h"
#include <assert.h>
#include <cstring>
#include <algorithm>
#include <iostream>
Screen::Screen(int width, int height, int depth, Color bgColor)
{
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->bgColor = bgColor;
	
	// 为zBuffer申请内存
	zBuffer = new float[width * height];
	// 为深度贴图申请内存
	depthMap = new float[width * height];
	// 初始化为最远值
	for (int i = 0; i < width * height; ++i)depthMap[i] = zBuffer[i] = -INF;
}

Screen::~Screen()
{
	delete[] zBuffer;
	delete[] depthMap;
}

void Screen::Create()
{
	initgraph(width, height);
	// 从[0, 1]映射至[0, 255]
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

// 获取指定位置的颜色
Color Screen::GetPixel(int x, int y)
{
	COLORREF tem = getpixel(x, height - y);
	// 从[0, 255]映射至[0, 1]
	float r = static_cast<float>(GetRValue(tem) / 255.0f);
	float g = static_cast<float>(GetGValue(tem) / 255.0f);
	float b = static_cast<float>(GetBValue(tem) / 255.0f);
	Color res(r, g, b);
	return res;
}

// 将指定位置设置为指定颜色
void Screen::SetPixel(int x, int y, Color color)
{
	// 从[0, 1]映射至[0, 255]
	int r = static_cast<int>(color.R() * 255);
	int g = static_cast<int>(color.G() * 255);
	int b = static_cast<int>(color.B() * 255);
	putpixel(x, height - y, RGB(r, g, b));
}

// 光栅化三角形，三角形的顶点坐标是屏幕坐标
void Screen::RasterizeTriangle(Triangle& triangle, Color* pointColors)
{

	//std::cout << "In Screen!" << std::endl;
	//for (int i = 0; i < 3; ++i)std::cout << triangle[i] << std::endl;
	//std::cout << std::endl;
	// 计算三角形包围盒
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
	// 偏移量数组
	float offsetX[4] = { -0.25f, 0.25f, 0.25f, -0.25f };
	float offsetY[4] = { 0.25f, 0.25f, -0.25f, -0.25f };
	// 遍历包围盒中的每一个像素
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// 计算重心坐标
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			// 判断是否在三角形内部
			if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))continue;
			// 插值计算深度
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// 判断是否在屏幕内部
			if (!InRange(z, -1.0, 1.0f))continue;
			// 深度测试
			int idx = (height - y - 1) * width + x;
			if (z < zBuffer[idx])continue;
			// 更新深度缓冲
			zBuffer[idx] = z;
			// 插值计算颜色
			Color color = bary.X() * pointColors[0] + bary.Y() * pointColors[1] + bary.Z() * pointColors[2];
			// 着色
			SetPixel(x, y, color);
		}
	}
}

// 光栅化三角形，构造depthMap
void Screen::RasterizeTriangleDepthMap(Triangle& triangle)
{
	// 计算三角形包围盒
	Vec2 bboxmin(INF, INF);
	Vec2 bboxmax(-INF, -INF);
	for (int i = 0; i < 3; ++i) {
		bboxmin.SetX(std::min(bboxmin.X(), triangle[i].X()));
		bboxmin.SetY(std::min(bboxmin.Y(), triangle[i].Y()));

		bboxmax.SetX(std::max(bboxmax.X(), triangle[i].X()));
		bboxmax.SetY(std::max(bboxmax.Y(), triangle[i].Y()));
	}
	// 遍历包围盒中的每一个像素
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// 计算重心坐标
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			// 判断是否在三角形内部
			if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))continue;
			// 插值计算深度
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// 判断是否在屏幕内部
			if (!InRange(z, -1.0, 1.0f))continue;
			// 深度测试
			int idx = (height - y - 1) * width + x;
			if (z < depthMap[idx])continue;
			// 更新深度缓冲
			depthMap[idx] = z;
		}
	}
}

// MSAA
void Screen::RasterizeTriangleMSAA(Triangle& triangle, Color* pointColors)
{
	//std::cout << "In Screen!" << std::endl;
	//for (int i = 0; i < 3; ++i)std::cout << triangle[i] << std::endl;
	//std::cout << std::endl;
	// 计算三角形包围盒
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
	// 偏移量数组
	float offsetX[4] = { -0.25f, 0.25f, 0.25f, -0.25f };
	float offsetY[4] = { 0.25f, 0.25f, -0.25f, -0.25f };
	// 遍历包围盒中的每一个像素
	for (int x = bboxmin.X(); x <= bboxmax.X(); ++x) {
		for (int y = bboxmin.Y(); y <= bboxmax.Y(); ++y) {
			// 判断是否在屏幕内部
			if (!InRange(x, 0, width - 1) || !InRange(y, 0, height - 1))continue;
			// 深度测试
			// 计算重心坐标
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// 判断是否在屏幕内部
			if (!InRange(z, -1.0, 1.0f))continue;
			// 二维坐标转换至一维坐标，注意反转y轴
			int idx = (height - y - 1) * width + x;
			assert(idx >= 0 && idx < width * height);
			if (z < zBuffer[idx])continue;
			// 更新zBuffer
			zBuffer[idx] = z;

			// 多重采样抗锯齿
			Color color(0.0f, 0.0f, 0.0f, 1.0f);
			for (int i = 0; i < 4; ++i) {
				// 计算偏移后的坐标
				float nx = static_cast<float>(x) + offsetX[i];
				float ny = static_cast<float>(y) + offsetY[i];
				// 计算重心坐标
				Vec3 bary = triangle.Barycentric(Vec2(nx, ny));
				// 判断是否在三角形内部
				if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))
					continue;
				// 插值计算颜色
				Color sampleColor = bary.X() * pointColors[0] + bary.Y() * pointColors[1] + bary.Z() * pointColors[2];
				// 累加至color
				color = color + 0.25 * sampleColor;
			}
			if(color != Color(0.0f, 0.0f, 0.0f, 1.0f))SetPixel(x, y, color);
		}
	}
}

// 屏幕空间光栅化三角形，正式进行光照计算(包围盒版本)
void Screen::RasterizeTriangleBoundingBox(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow)
{
	//for (int i = 0; i < 3; ++i)std::cout << triangle[i] << std::endl;
	// 计算三角形包围盒
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
			// 判断是否在屏幕内部
			if (!InRange(x, 0, width - 1) || !InRange(y, 0, height - 1))continue;
			// 计算重心坐标
			Vec3 bary = triangle.Barycentric(Vec2(x, y));
			// 判断是否在三角形内部
			if (!InRange(bary.X(), 0.0f, 1.0f) || !InRange(bary.Y(), 0.0f, 1.0f) || !InRange(bary.Z(), 0.0f, 1.0f))
				continue;
			// 深度测试
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();
			// 判断是否在屏幕内部
			if (!InRange(z, -1.0, 1.0f))continue;
			int idx = (height - y - 1) * width + x;
			assert(idx >= 0 && idx < width * height);
			if (z < zBuffer[idx])continue;
			// 更新zBuffer
			zBuffer[idx] = z;
			// 光照计算
			Color color(0.0f);
			if (shadow)color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
			else color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
			// 着色
			SetPixel(x, y, color);
		}
	}
}

// 屏幕空间光栅化三角形，正式进行光照计算 (扫描线版本)
void Screen::RasterizeTriangleScanline(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow)
{
	// 1. 获取三角形的三个顶点，并按照 Y 坐标从小到大进行排序
	// 排序后保证：v0.y < v1.y < v2.y
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
	// 如果总高度为 0，说明这是一条水平线（退化三角形），直接丢弃不渲染
	if (total_height == 0) return;

	// 2. 逐行扫描：从最低点 y0 一直扫描到最高点 y2
	for (int y = y0; y <= y2; ++y) {
		// 视口外裁剪，避免越界
		if (!InRange(y, 0, height - 1)) continue;

		// 判断当前扫描线是处于三角形的“下半部分”(v0 到 v1) 还是“上半部分”(v1 到 v2)
		bool second_half = y > y1 || y1 == y0;
		int segment_height = second_half ? (y2 - y1) : (y1 - y0);

		// 计算当前 Y 在整个三角形高度 (alpha) 以及在当前半段高度 (beta) 中的比例
		float alpha = (float)(y - y0) / total_height;
		float beta = (segment_height == 0) ? 1.0f : (float)(y - (second_half ? y1 : y0)) / segment_height;

		// 动态计算当前扫描线与三角形左右两边的交点 X 坐标
		// xA 是与“长边”(v0 -> v2) 的交点
		int xA = std::round(v0.X() + (v2.X() - v0.X()) * alpha);
		// xB 是与“短边”(v0 -> v1 或 v1 -> v2) 的交点
		int xB = second_half ?
			std::round(v1.X() + (v2.X() - v1.X()) * beta) :
			std::round(v0.X() + (v1.X() - v0.X()) * beta);

		int x_left = std::min(xA, xB) - 1;
		int x_right = std::max(xA, xB) + 1;

		// 3. 仅仅在当前扫描线确定的 [x_left, x_right] 有效区间内进行片元处理
		for (int x = x_left; x <= x_right; ++x) {
			// 视口外裁剪
			if (!InRange(x, 0, width - 1)) continue;

			// 计算重心坐标 (用于属性插值)
			Vec3 bary = triangle.Barycentric(Vec2(x, y));

			const float EPS = 1e-5f;

			// 放宽判定条件
			if (!InRange(bary.X(), -EPS, 1.0f + EPS) ||
				!InRange(bary.Y(), -EPS, 1.0f + EPS) ||
				!InRange(bary.Z(), -EPS, 1.0f + EPS)) {
				continue;
			}

			// 深度测试
			float z = bary.X() * triangle[0].Z() + bary.Y() * triangle[1].Z() + bary.Z() * triangle[2].Z();

			if (!InRange(z, -1.0f, 1.0f)) continue;

			int idx = (height - y - 1) * width + x;
			assert(idx >= 0 && idx < width * height);

			// Z 越大越靠近相机
			if (z < zBuffer[idx]) continue;

			// 更新 zBuffer
			zBuffer[idx] = z;

			// 光照计算
			Color color(0.0f);
			if (shadow) {
				color = BlinPhongShadow(*this, p, normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
			}
			else {
				color = BlinPhong(normalMatrix, diffuseMap, triangle, bary, lightPos, viewPos);
			}

			// 着色
			SetPixel(x, y, color);
		}
	}
}

// 绘制模型
void Screen::RenderModel(const Mat4& m, const Mat4& p, const Mat4& mvp, Model& model, const Vec3& lightPos, const Camera& camera, bool shadow)
{
	int nFaces = model.NumOfFaces();
	Image* diffuseMap = model.GetDiffuseMap();
	//std::cout << "RenderModel m: " << std::endl << m << std::endl;
	//std::cout << "RenderModel m.inverse: " << std::endl << m.Inverse() << std::endl;
	Mat4 normalMatrix = m.Inverse().Transpose();
	//std::cout << "RenderModel normalMatrix: " << std::endl << normalMatrix << std::endl;
	for (int i = 0; i < nFaces; ++i) {
		// 构造三角形
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
		
		// 坐标变换
		triangle.Transform(mvp, width, height);

		// 背面剔除
		Vec3 planeNormal = triangle.GetPlaneNormal();
		Vec3 cameraFront = camera.GetFront();
		if (Dot(planeNormal, cameraFront) > 0.f) continue;
		// 光栅化
		RasterizeTriangleScanline(p, normalMatrix, diffuseMap, triangle, lightPos, camera.GetPosition(), shadow);

		// 释放内存
		delete[] points;
		delete[] texCoords;
		delete[] normals;
	}
}

// 绘制原始模型
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
		// 构造三角形
		Vec4* points = new Vec4[3];
		Vec2* texCoords = new Vec2[3];
		for (int j = 0; j < 3; ++j) {
			Vec3 idx = model.Vertex(i, j);
			for (int k = 0; k < 3; ++k)points[j][k] = model.Vertex(idx[0])[k];
			points[j][3] = 1.0f;
			// 映射至屏幕坐标
			points[j][0] = (points[j][0] - xl) / (xr - xl) * width;
			points[j][1] = (points[j][1] - yl) / (yr - yl) * height;
			texCoords[j] = model.TexCoord(idx[1]);
		}
		Triangle triangle(points, texCoords);

		// 光栅化
		Color* pointColors = new Color[3];
		for (int j = 0; j < 3; ++j) {
			pointColors[j] = model.GetDiffuseMap()->GetPixel(texCoords[j]);
		}
		RasterizeTriangle(triangle, pointColors);

		// 释放内存
		delete[] points;
		delete[] texCoords;
		delete[] pointColors;
	}
}

// 光源视角渲染，构造depthMap
void Screen::ConstructDepthMap(const Mat4& m, const Mat4& mvp, Model& model)
{
	int nFaces = model.NumOfFaces();
	for (int i = 0; i < nFaces; ++i) {
		// 构造三角形
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

		// 坐标变换
		triangle.Transform(mvp, width, height);

		// 光栅化，构造depthMap
		RasterizeTriangleDepthMap(triangle);

		// 释放内存
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

// 清理zBuffer
void Screen::ClearZ()
{
	for (int i = 0; i < height * width; ++i)zBuffer[i] = -INF;
}

// 清理depth
void Screen::ClearDepth()
{
	for (int i = 0; i < height * width; ++i)depthMap[i] = -INF;
}