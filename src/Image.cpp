#include "Image.h"

Image::Image(const char* filePath)
{
	img = new IMAGE;
	loadimage(img, _T("../models/textures/Dungeon_Aiz_diff.png"));
}

Image::~Image()
{
	delete img;
}

// 获取指定位置的颜色
Color Image::GetPixel(float u, float v)
{
	// 反转y轴
	v = 1.0f - v;
	int width = img->getwidth(), height = img->getheight();
	int x = static_cast<int>(u * width), y = static_cast<int>(v * height);
	SetWorkingImage(img);
	COLORREF tem = getpixel(x, y);
	float r = static_cast<float>(GetRValue(tem) / 255.0f);
	float g = static_cast<float>(GetGValue(tem) / 255.0f);
	float b = static_cast<float>(GetBValue(tem) / 255.0f);
	SetWorkingImage(NULL);
	return Color(r, g, b);
}

// 获取指定位置的颜色
Color Image::GetPixel(Vec2 uv)
{
	return GetPixel(uv[0], uv[1]);
}