#include "Screen.h"
#include "Camera.h"
#include "Mat4.h"
#include "Global.h"
#include "Model.h"
#include "Image.h"
#include <iostream>
#include <conio.h>

const int SCR_WIDTH = 800;              // Screen width
const int SCR_HEIGHT = 600;             // Screen height
const int SCR_DEPTH = 1000;             // Screen depth
const bool ENABLE_SSAA = true;          // Toggle 4x SSAA on or off

float lastX = 0.0f, lastY = 0.0f;       // Previous mouse position
float lastFrame = 0.0f;                 // Previous frame time

const Vec3 position(0.0f, 0.0f, 0.0f);  // Initial camera position
const Vec3 worldUp(0.0f, 1.0f, 0.0f);   // World up direction
float yaw = -90.0f;                     // Yaw angle
float pitch = 0.0f;                     // Pitch angle

Vec3 lightPos(2.0f, 2.0f, 2.0f);        // Light position
Vec3 viewPos(0.0f, 0.0f, 0.0f);         // View position


int main()
{
	// Create the screen
	Screen screen(SCR_WIDTH, SCR_HEIGHT, SCR_DEPTH, Color(1.0f, 1.0f, 1.0f, 1.0f));
	screen.SetSSAAEnabled(ENABLE_SSAA);
	screen.Create();

	// Create the camera
	Camera camera(position, worldUp, yaw, pitch);
	std::cout << std::endl;

	// Load the model
	const char* filePath = "../models/source/Aiz_v1.0_2.79.obj";
	Model myModel(filePath);

	// Build the model matrix
	Vec3 translate(0.0f, -0.8f, -1.0f);
	Vec3 nx(1.0f, 0.0f, 0.0f), ny(0.0f, 1.0f, 0.0f), nz(0.0f, 0.0f, 1.0f);
	Vec3 scale(1.0f, 1.0f, 1.0f);
	Mat4 model = Translate(translate) * Rotate(ny, 0.5f);

	// Build the projection matrix
	float fov = Radians(90.0f);
	float ratio = (float)SCR_WIDTH / SCR_HEIGHT;
	float zNear = 0.1f;
	float zFar = 100.0f;
	Mat4 projection = Perspective(fov, ratio, zNear, zFar);
	
	// Store keyboard and mouse state
	ExMessage *msg = new ExMessage;
	
	// Initialize state
	lastFrame = GetTickCount();
	lastX = msg->x;
	lastY = msg->y;
	bool first = true;

	
	BeginBatchDraw();
	
	// Render loop
	while (1) {
		cleardevice();
		//peekmessage(msg, EX_MOUSE);
		// Compute delta time
		float currentFrame = GetTickCount();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Compute mouse offsets
		float xOffset = msg->x - lastX;
		// Note that the y axis is flipped
		float yOffset = lastY - msg->y;
		lastX = msg->x;
		lastY = msg->y;

		// Handle mouse and keyboard input
		//camera.Listen(msg, deltaTime, xOffset, yOffset);

		//lightPos = Rotate(ny, 0.3f) * Vec4(lightPos, 0.0f);

		// Build the depth map
		Mat4 view = Camera::LookAt(lightPos, translate, Vec3(0.0f, 1.0f, 0.0f));
		//Mat4 view(1.0f);
		Mat4 mvp = projection * view * model;
		screen.ConstructDepthMap(model, mvp, myModel);

		// Main render pass
		view = camera.GetViewMatrix();
		model = Translate(translate) * Rotate(ny, 0.3f) * Translate(-1.0f * translate) * model;
		mvp = projection * view * model;
		screen.RenderModel(model, projection, mvp, myModel, lightPos, camera, true);

		FlushBatchDraw();

		// Clear the z-buffer
		screen.ClearZ();
		// Clear the depth map
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
