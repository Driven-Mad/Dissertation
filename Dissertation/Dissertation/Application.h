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
#include "Lightning.h"
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

	void RenderSmallQuad(GLint program);
	void RenderQuad(GLint program);

	void storm(float deltaTime);
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
	//FrameBuffer *fBuffer, *bloomFBuffer;
	Camera *camera;
	bool exit;
	Rain *rain;
	Lights *lightHandler;
	Lightning *lightning;
	glm::vec4 lightningLightA,lightningLightB,lightningLightC;
	float lightningTimer;

	///FRAME BUFFERS
	ProgramLoader *ShaderBlurFB,*ShaderBloomFinal, *PLCubePointLights;// *ShaderBloomFB;
	GLuint HDRFBuffer, PingPongFBuffer[2], depthMapFbuffer, cubeMapFrameBuffer[5]; ///< Storage for the framebuffer.
	GLuint rendTexture[2], PingPongRendTextures[2], depthMap,depthCubeMap[5]; ///< rendered texture output.
	GLuint depthRenderBuffer; ///< Depth buffer
	GLboolean bloom;
	GLuint quad_VertexArrayID; ///< quad vertexarray ID
	GLuint quad_vertexbuffer; ///< quad vertex buffer
	GLuint quad_programID; ///< quad program ID
	GLint quad_attrib;///< quad Attribute
	GLuint quadVAO, quadSmallVAO;
	GLuint quadVBO,quadSmallVBO;
	GLfloat exposure;
	bool shadowRendered;
	float timerStorm, timerFlash;
	bool rainAndLightningOn;
	int flashCount;
	bool flashed,finishedForking;
};
#endif; //!APPLICATION_H
