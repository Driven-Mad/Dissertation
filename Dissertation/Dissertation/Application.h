#ifndef APPLICATION_H
#define APPLICATION_H
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Application
{
public:
	Application(void);
	~Application(void);
	bool InitGL();
	void init();
	void run(float DT);
	void draw();
	void update(float DT);
	void inputHandler();
	float getDeltaTime();

private:
	int mouse_X, mouse_Y;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_GLContext glContext;
	unsigned int lastTime, current;
	int winPosX;
	int winPosY;
	int winWidth;
	int winHeight;
	float camera_Zoom, camera_Left_Right, camera_Up_Down;
	glm::mat4 projectionMatrix, viewMatrix;
	float delta_Time;
};
#endif;
