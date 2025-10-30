#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include <easyx.h>
#define NOMINMAX

class Camera {
public:
	Camera();                                                                   // 默认构造函数
	Camera(const Vec3& position, const Vec3& worldUp, float yaw, float pitch);
	~Camera();
	Mat4 GetViewMatrix(bool print = false);
	void Listen(ExMessage* msg, float deltaTime, float xOffset, float yOffset); // 监听外部事件
	Vec3& GetPosition();                                                        // 获取摄像机位置
	const Vec3& GetPosition()const;                                             // 获取摄像机位置
	Mat4 static LookAt(const Vec3& position, const Vec3& center, const Vec3& worldUp); // 计算view矩阵
private:
	void UpdateCameraVectors();                                                 // 根据yaw和pitch，更新front、up和right
	void ProcessKeyboard(ExMessage* msg, float deltaTime);                      // 处理键盘按下
	void ProcessMouseMovement(float xOffset, float yOffset);                    // 处理鼠标移动

	Vec3 position;                                                              // 摄像机位置
	Vec3 front;                                                                 // 摄像机前方
	Vec3 up;                                                                    // 摄像机上方
	Vec3 right;                                                                 // 摄像机右方
	Vec3 worldUp;                                                               // 世界上方
	float yaw;                                                                  // 偏航角
	float pitch;                                                                // 俯仰角
};