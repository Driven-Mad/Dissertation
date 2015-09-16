#pragma once
#include "glew.h"
#include "ProgramLoader.h"
class FrameBuffer
{
public:
	FrameBuffer(char *fragmentShaderFilepath,char *vertexShaderFilepath);
	~FrameBuffer(void);
	void init();
	void draw(int output);
	void bind();
	void unbind();
	void cleanUp();
private:
	GLuint FBuffer;
	ProgramLoader *PL;
	GLuint rendTexture;
	GLuint depthRenderBuffer;
	GLenum DrawBuffers[1];
	GLuint quad_VertexArrayID;
	GLuint quad_vertexbuffer;
	GLuint quad_programID;
	GLint quad_attrib;
	char *fragFP, *vertFP;
	GLint outputLocation;
	int outPut;

};

