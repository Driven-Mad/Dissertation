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
#include "Rain.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "gtc/matrix_access.hpp"
#include "Camera.h"
#include <vector>
#include <gtc/type_ptr.hpp>
#include "Lights.h"
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
	bool run();
	/// \brief draws the application to screen
	void draw();
	/// \brief updates application based on Delta Time
	void update();
	/// \brief returns delta time of application
	float getDeltaTime();
	/// \brief returns screen width of application
	int getWidth();
	/// \brief returns screen height of application
	int getHeight();

private:
	SDL_Window *window; ///< Window for SDL
	SDL_Renderer *renderer; ///< Renderer for SDL
	SDL_GLContext glContext; ///< GL context for window
	unsigned int lastTime, current; ///< seed for delta time
	int winPosX; ///<window X position
	int winPosY; ///< window Y poisition
	int winWidth; ///< window width
	int winHeight; ///< window Height
	float delta_Time; ///< Deltat time for frame rate.
	Model *skyDome, *house, *plane, *shelter,*car;
	FrameBuffer *fBuffer;
	Camera *camera;
	bool exit;
	Rain *rain;
	Lights *lightHandler;
};
#endif; //!APPLICATION_H
