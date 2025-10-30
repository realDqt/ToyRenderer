#include "Screen.h"
#include "Camera.h"
#include "Mat4.h"
#include "Global.h"
#include "Model.h"
#include "Image.h"
#include <iostream>
#include <conio.h>

const int SCR_WIDTH = 800;              // 屏幕宽度
const int SCR_HEIGHT = 600;             // 屏幕高度
const int SCR_DEPTH = 1000;             // 屏幕深度

float lastX = 0.0f, lastY = 0.0f;       // 上次鼠标的坐标
float lastFrame = 0.0f;                 // 上次时间

const Vec3 position(0.0f, 0.0f, 0.0f);  // 摄像机初始位置
const Vec3 worldUp(0.0f, 1.0f, 0.0f);   // 世界上方
float yaw = -90.0f;                     // 偏航角
float pitch = 0.0f;                     // 俯仰角

Vec3 lightPos(2.0f, 2.0f, 2.0f);        // 光源位置
Vec3 viewPos(0.0f, 0.0f, 0.0f);         // 观察位置


int main()
{
	// 创建屏幕
	Screen screen(SCR_WIDTH, SCR_HEIGHT, SCR_DEPTH, Color(1.0f, 1.0f, 1.0f, 1.0f));
	screen.Create();

	// 创建摄像机
	Camera camera(position, worldUp, yaw, pitch);
	std::cout << std::endl;

	// 加载模型
	const char* filePath = "../models/source/Aiz_v1.0_2.79.obj";
	Model myModel(filePath);

	// 计算model矩阵
	Vec3 translate(0.0f, -0.8f, -1.0f);
	Vec3 nx(1.0f, 0.0f, 0.0f), ny(0.0f, 1.0f, 0.0f), nz(0.0f, 0.0f, 1.0f);
	Vec3 scale(1.0f, 1.0f, 1.0f);
	Mat4 model = Translate(translate) * Rotate(ny, 0.5f);

	// 计算projection矩阵
	float fov = Radians(90.0f);
	float ratio = (float)SCR_WIDTH / SCR_HEIGHT;
	float zNear = 0.1f;
	float zFar = 100.0f;
	Mat4 projection = Perspective(fov, ratio, zNear, zFar);
	
	// 存储键盘和鼠标的信息
	ExMessage *msg = new ExMessage;
	
	// 初始化
	lastFrame = GetTickCount();
	lastX = msg->x;
	lastY = msg->y;
	bool first = true;

	
	BeginBatchDraw();
	
	// 渲染循环
	while (1) {
		cleardevice();
		//peekmessage(msg, EX_MOUSE);
		// 计算deltaTime
		float currentFrame = GetTickCount();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 计算xOffset和yOffset
		float xOffset = msg->x - lastX;
		// 注意y轴反转
		float yOffset = lastY - msg->y;
		lastX = msg->x;
		lastY = msg->y;

		// 监听鼠标和键盘操作
		//camera.Listen(msg, deltaTime, xOffset, yOffset);

		//lightPos = Rotate(ny, 0.3f) * Vec4(lightPos, 0.0f);

		// 构造DepthMap
		Mat4 view = Camera::LookAt(lightPos, translate, Vec3(0.0f, 1.0f, 0.0f));
		//Mat4 view(1.0f);
		Mat4 mvp = projection * view * model;
		screen.ConstructDepthMap(model, mvp, myModel);

		// 正式渲染
		view = camera.GetViewMatrix();
		model = Translate(translate) * Rotate(ny, 0.3f) * Translate(-1.0f * translate) * model;
		mvp = projection * view * model;
		viewPos = camera.GetPosition();
		screen.RenderModel(model, projection, mvp, myModel, lightPos, viewPos, true);

		FlushBatchDraw();

		// 清理zBuffer
		screen.ClearZ();
		// 清理depthMap
		screen.ClearDepth();
	}
	


	// debug
	/*
	Mat4 test(1.0f);
	for (int i = 0; i < 4; ++i)test[i][i] = static_cast<float>(i + 1);
	Mat4 invT = test.Inverse().Transpose();
	std::cout << "test.inverse: " << std::endl << test.Inverse() << std::endl;
	std::cout << "test.transpose: " << std::endl << test.Transpose() << std::endl;
	std::cout << "test.inverse.transpose: " << std::endl << invT << std::endl;
	*/
	EndBatchDraw();
	_getch();
	screen.Close();
}