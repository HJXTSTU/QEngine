#include "gl_camera.h"

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT, float _near, float _far) {
	return glm::perspective(glm::radians(Zoom), SCR_WIDTH / SCR_HEIGHT, _near, _far);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 90.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 90.0f)
		Zoom = 90.0f;
}

void Camera::UpdateCameraVectorForce() {
	Front = glm::normalize(Front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, Up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::OrthoProjection(float left, float right, float bottom, float top) {
	return glm::ortho(left, right, bottom, top, 0.01f, 1000.0f);
}

glm::vec3  Camera::ScreenToWorld(glm::vec2 screenPoint, float depth, float _near,float _far)
{
	glm::vec4 ndc = glm::vec4(screenPoint, depth*2.0f-1.0f, 1.0f);
	glm::mat4 inverse_proj = glm::inverse(this->GetProjectionMatrix(SRC_WIDTH, SRC_HEIGHT, _near, _far));
	glm::mat4 inverse_view = glm::inverse(this->GetViewMatrix());
	glm::vec4 worldPoint = inverse_view * inverse_proj *ndc;
	worldPoint /= worldPoint.w;
	return worldPoint;
}

glm::vec3 Camera::WolrdToScreen(glm::vec3 worldPoint, float _near, float _far)
{
	glm::mat4 projection = this->GetProjectionMatrix(SRC_WIDTH, SRC_HEIGHT, _near, _far);
	glm::mat4 view = this->GetViewMatrix();
	glm::vec4 screenPoint = projection * view*glm::vec4(worldPoint, 1.0f);
	screenPoint /= screenPoint.w;
	return screenPoint;
}

glm::vec3 Camera::CameraSpacePoint(glm::vec3 worldPoint) {
	glm::mat4 view = this->GetViewMatrix();
	return view * glm::vec4(worldPoint, 1.0f);
}
