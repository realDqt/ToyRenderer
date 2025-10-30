#pragma once
#include "Color.h"
#include "Vec2.h"
#include <easyx.h>
#define NOMINMAX
class Image {
public:
	Image(const char* filePath);
	~Image();
	Color GetPixel(float u, float v); // 获取指定位置的颜色
	Color GetPixel(Vec2 uv);          // 获取指定位置的颜色
private:
	IMAGE* img;
};
