#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

//------------------------------------------------------------------
/// \file    FrameBuffer.h
/// \author  Lloyd Phillips
/// \brief   This is the frameBuffer loader class
//------------------------------------------------------------------
#include "glew.h"
#include "ProgramLoader.h"
class FrameBuffer
{
public:
	/// \brief constructor for frameBuffer, takes in shader file paths.
	FrameBuffer(char *fragmentShaderFilepath,char *vertexShaderFilepath);
	/// \brief deconstructor for frameBuffer.
	~FrameBuffer(void);
	/// \brief initialises the framebuffer.
	void init(int winWidth,int winHeight);
	/// \brief initialises the framebuffer.
	void initBlur(int winWidth,int winHeight);
	/// \brief draws framebuffer, output will be a selection from the user.
	void draw(int output);
	/// \brief binds the framebuffer. takes in windowWidth, winowHeight.
	void bind( int winWidth,int winHeight);
	/// \brief unbinds the framebuffer. takes in windowWidth, winowHeight.
	void unbind( int winWidth,int winHeight);
	/// \brief cleans up after itself. returning openGL to it's original state.
	void cleanUp();
	///\brief returns the program;
	ProgramLoader* getProgramLoader(){return PL;};
private:
	GLuint FBuffer[2]; ///< Storage for the framebuffer.
	ProgramLoader *PL; ///< Program that it's going to use.
	GLuint rendTexture[2]; ///< rendered texture output.
	GLuint depthRenderBuffer; ///< Depth buffer
	GLenum DrawBuffers[2]; ///< Draw buffer
	GLuint quad_VertexArrayID; ///< quad vertexarray ID
	GLuint quad_vertexbuffer; ///< quad vertex buffer
	GLuint quad_programID; ///< quad program ID
	GLint quad_attrib;///< quad Attribute
	char *fragFP, *vertFP; ///< File path for Shaders
	GLint outputLocation; ///< output location for uniform of user input.
	int outPut; ///< output from user.

};
#endif //!FRAMEBUFFER_H
