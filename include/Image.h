#pragma once
#include "Color.h"
#include "Vec2.h"
#include <easyx.h>
#define NOMINMAX
class Image {
public:
	Image(const char* filePath);
	~Image();
	Color GetPixel(float u, float v); // Get the color at the given UV
	Color GetPixel(Vec2 uv);          // Get the color at the given UV
private:
	IMAGE* img;
};
