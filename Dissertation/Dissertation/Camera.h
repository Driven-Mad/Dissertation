#pragma once
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera(float screen_h, float screen_w);
	~Camera(void);

	glm::mat4 getProjectionMatrix(){return projectionMatrix;};
	glm::mat4 getViewMatrix(){return viewMatrix;};
	glm::vec3 getCameraPosition(){return cameraPosition;};
	glm::vec3 getCameraFront(){return cameraFront;};
	glm::vec3 getCameraVelocity(){return velocity;};
	glm::vec3 getCameraDirection(){return displacment;};
	void cameraMovement(float DT,SDL_Event &incomingEvent);
	void update(float DT);

private:
	glm::mat4 projectionMatrix, viewMatrix; 
	float pers_val,field_of_view;
	glm::vec3 cameraPosition, cameraFront, cameraUp, cameraOldPosition;
	bool leftAltPressed;
	float mouse_X, mouse_Y; ///<Mouse Co-ordinates
	float old_mouse_X, old_mouse_Y;
	float pitch, yaw;
	float mouse_X_offset;
	float mouse_Y_offset;
	float sensitivity;
	glm::vec3 front;
	glm::vec3 displacment;
	glm::vec3 velocity;
	float cameraSpeed;
	bool moveL, moveR, moveF, moveB;
};

