#pragma once
#include "Model.h"
#include "Camera.h"
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class Rain
{
public:
	Rain(void);
	~Rain(void);
	void update(float DT, Camera *c);
	void draw(glm::mat4 viewMat, glm::mat4 projMat);
	void rainInput(float DT,SDL_Event &incomingEvent);
private:
	Model *doubleEndedCone;
	glm::vec3 cross;
	bool rotate,rotateNormal;
	float angle;
};

