#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
public:
	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera Option
	float MovementSpeed;
	float MouseSensitivity;
	float Fov;
	Camera(glm::vec3 position, glm::vec3 up);
	Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch);
	glm::mat4 GetviewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float delta);
	void ProcessMouseInput(float xoffset, float yoffset, bool constrainpitch);
	void ProcessMouseScroll(float yoffset);

private:
	void UpdateCameraVectors();

};

#endif // !CAMERA_H

