#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include <easyx.h>
#define NOMINMAX

class Camera {
public:
	Camera();                                                                   // Default constructor
	Camera(const Vec3& position, const Vec3& worldUp, float yaw, float pitch);
	~Camera();
	Mat4 GetViewMatrix(bool print = false);
	void Listen(ExMessage* msg, float deltaTime, float xOffset, float yOffset); // Handle external input events
	Vec3& GetPosition();                                                        // Get the camera position
	const Vec3& GetPosition()const;                                             // Get the camera position
	const Vec3& GetFront()const;                                                // Get the camera forward direction
	Mat4 static LookAt(const Vec3& position, const Vec3& center, const Vec3& worldUp); // Build the view matrix
private:
	void UpdateCameraVectors();                                                 // Update front, up, and right from yaw and pitch
	void ProcessKeyboard(ExMessage* msg, float deltaTime);                      // Process keyboard input
	void ProcessMouseMovement(float xOffset, float yOffset);                    // Process mouse movement

	Vec3 position;                                                              // Camera position
	Vec3 front;                                                                 // Camera forward direction
	Vec3 up;                                                                    // Camera up direction
	Vec3 right;                                                                 // Camera right direction
	Vec3 worldUp;                                                               // World up direction
	float yaw;                                                                  // Yaw angle
	float pitch;                                                                // Pitch angle
};
