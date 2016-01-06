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
	void cameraMovement(float DT,SDL_Event &incomingEvent);
	void update(float DT);
private:
	glm::mat4 projectionMatrix, viewMatrix; 
	float pers_val,field_of_view;
	glm::vec3 cameraPosition, cameraFront, cameraUp;
	bool leftAltPressed;
	int mouse_X, mouse_Y; ///<Mouse Co-ordinates

};

