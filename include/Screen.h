#pragma once
#define NOMINMAX
#include "Color.h"
#include "Triangle.h"
#include "Model.h"
#include <easyx.h>
class Camera; // Forward declaration
class Screen {
public:
	Screen(int width, int height, int depth, Color bgColor);
	~Screen();
	void Create();                                                      // Create the window
	void Close();                                                       // Close the window
	void SetSSAAEnabled(bool enabled);                                  // Enable or disable SSAA
	bool IsSSAAEnabled() const;                                         // Query whether SSAA is enabled
	int GetWidth()const;
	int GetHeight()const;
	float* GetDepthMap()const;                                          // Get the depth map
	Color GetPixel(int x, int y);                                       // Get the color at a pixel
	void SetPixel(int x, int y, Color color);                           // Set the color at a pixel
	void RasterizeTriangle(Triangle& triangle, Color* pointColors);     // Rasterize a triangle in screen space
	void RasterizeTriangleDepthMap(Triangle& triangle);                 // Rasterize a triangle into the depth map
	void RasterizeTriangleBoundingBox(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow = false);  // Rasterize a triangle and shade it in screen space
	void RasterizeTriangleScanline(const Mat4& p, const Mat4& normalMatrix, Image* diffuseMap, Triangle& triangle, const Vec3& lightPos, const Vec3& viewPos, bool shadow = false);		// Rasterize a triangle with scanlines and shade it
	void RenderModel(const Mat4& m, const Mat4& p, const Mat4& mvp, Model& model, const Vec3& lightPos, const Camera& camera, bool shadow = false);										// Render a model
	void RenderModel(Model& model);                                     // Render the original model
	void ConstructDepthMap(const Mat4& m,const Mat4& mvp, Model& model);// Render from the light view and build the depth map
	void ClearZ();                                                      // Clear the z-buffer
	void ClearDepth();                                                  // Clear the depth map
private:
	int PixelIndex(int x, int y) const;
	int SampleIndex(int x, int y, int sampleIdx) const;                            
	void ResolveSsaaPixel(int x, int y);

	int width;                                                          // Window width
	int height;                                                         // Window height
	int depth;                                                          // Window depth
	Color bgColor;                                                      // Background color
	float* zBuffer;                                                     // Z-buffer
	float* depthMap;                                                    // Depth map
	float* ssaaDepthBuffer;                                             // SSAA sample depth
	Color* ssaaColorBuffer;                                             // SSAA sample color
	bool ssaaEnabled;                                                   // Whether 4x SSAA is enabled
};

