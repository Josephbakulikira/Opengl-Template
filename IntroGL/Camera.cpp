#include "Camera.h"

// init camera - constructors
Camera::Camera() {}
Camera::Camera(glm::vec3 initialPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnspeed) {
	position = initialPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnspeed;

	Update();
}
// camera update
void Camera::Update() {
	front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)) );
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)) );
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}
// Handle camera controls
void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {

	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

Camera::~Camera(){}
