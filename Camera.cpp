#include "Camera.h"

Camera::Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(YAW), Pitch(PITCH), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
{
	Position = position;
	WorldUp = up;

	UpdateCameraVectors();
}

Camera::Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
{
	Position = glm::vec3(posx, posy, posz);
	WorldUp = glm::vec3(upx, upy, upz);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetviewMatrix()
{
	return glm::lookAt(Position,Position+Front,WorldUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float delta)
{
	float velocity = MovementSpeed * delta;
	if (direction == FORWARD) Position += Front * velocity;
	if (direction == BACKWARD) Position -= Front * velocity;
	if (direction == LEFT) Position -= Right * velocity;
	if (direction == RIGHT) Position += Right * velocity;
	if (direction == UP) Position -= Up * velocity;
	if (direction == DOWN) Position += Up * velocity;
}

void Camera::ProcessMouseInput(float xoffset, float yoffset, bool constrainpitch = true)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainpitch)
	{
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	UpdateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset)
{
	Fov -= (float)yoffset;
	if (Fov < 1.0f) Fov = 1.0f;
	if (Fov > 45.0f) Fov = 45.0f;
}
void Camera::UpdateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(direction);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Front, Right));
}