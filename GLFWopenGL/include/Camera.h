#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Define several possible options for camera movement. Used as abstraction 
enum Camera_Movement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default values.
const float SPEED = 2.5f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ZOOM = 45.0f;
const float SENSITIVITY = 0.1f;

// Abstract camera class that processes input and also the corresponding angles vectors 
// Matrices, for use in OpenGL.
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Angles
	float Yaw;
	float Pitch;
	// Options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor for vectors.
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Constructor for scalar.
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Function to process Input
	// Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
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
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;
	}

	// Process the input received from a mous input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float a_xOffset, float a_yOffset, GLboolean constrainPitch = true)
	{
		// To calculate pitch and yaw values we need to tell GLFW to listen to mouse-movement events. 
		a_xOffset *= MouseSensitivity;
		a_yOffset *= MouseSensitivity;
		// When handling mouse input for an FPS style camera, serveral steps should be done to retrieve direction vector.
		Yaw += a_xOffset;
		Pitch += a_yOffset;

		// Pitch needs to be fixed out of bounds, so that the screen does not get flipped.
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

	// Process input recieved from scroll wheel.
	void ProcessMouseScroll(float a_yOffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= a_yOffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculating the front vector from the camera's EULer angles.
	void updateCameraVectors()
	{
		// Calculate actual direction vector from the resulting yaw and pitch value from above.
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		// Also recalculate the right and up vector.
		Right = glm::normalize(glm::cross(Front, WorldUp)); 
		Up = glm::normalize(glm::cross(Right, Front));
	}

};

#endif CAMERA_H
