#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="startPosition">The camera's position in the world space</param>
	/// <param name="startUp">The global upward direction</param>
	/// <param name="startYaw">defines the camera's direction. LEFT-RIGHT</param>
	/// <param name="startPitch">camera's direction. UP-DOWN</param>
	/// <param name="startMoveSpeed">The speed at which the camera moves</param>
	/// <param name="startTurnSpeed">The speed at which the camera rotates </param>
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

