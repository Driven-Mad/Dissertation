#ifndef APPLICATION_H
#define APPLICATION_H
//------------------------------------------------------------------
/// \file    ProgramLoader.h
/// \author  Lloyd Phillips
/// \brief   This is the Application loader class
//------------------------------------------------------------------
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "gtc/matrix_access.hpp"
class Application
{
public:
	/// \brief constructor for the application class.
	Application(void);
	/// \brief deconstructor for the application class.
	~Application(void);
	/// \brief check if Glew has been initialised.
	bool InitGL();
	/// \brief initalises the application
	void init();
	/// \brief runs the application, uses Delta Time
	void run();
	/// \brief draws the application to screen
	void draw();
	/// \brief updates application based on Delta Time
	void update();
	/// \brief input handler for the application
	void inputHandler();
	/// \brief returns delta time of application
	float getDeltaTime();
	/// \brief returns screen width of application
	int getWidth();
	/// \brief returns screen height of application
	int getHeight();

private:
	int mouse_X, mouse_Y; ///<Mouse Co-ordinates
	SDL_Window *window; ///< Window for SDL
	SDL_Renderer *renderer; ///< Renderer for SDL
	SDL_GLContext glContext; ///< GL context for window
	unsigned int lastTime, current; ///< seed for delta time
	int winPosX; ///<window X position
	int winPosY; ///< window Y poisition
	int winWidth; ///< window width
	int winHeight; ///< window Height
	glm::mat4 projectionMatrix, viewMatrix; ///< matrix for viewing
	float delta_Time; ///< Deltat time for frame rate.
	Model *skyDome, *house, *plane, *shelter,*car, *light, *rain;
	TextureLoader *textLoad, *textLoad2, *textLoad3,*textLoad4,*textLoad5, *textLoad6;
	FrameBuffer *fBuffer;
	glm::vec4 lightPosition;
	bool leftShiftPressed,leftAltPressed;
	float pers_val;
	glm::vec3 cameraPosition, cameraFront, cameraUp;
	float field_of_view;

};
#endif; //!APPLICATION_H
