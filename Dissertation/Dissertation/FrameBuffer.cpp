#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(char *fragmentShaderFilepath,char *vertexShaderFilepath)
{
	FBuffer = 0;
	rendTexture = 0;
	quad_vertexbuffer = 0;
	fragFP = fragmentShaderFilepath;
	vertFP = vertexShaderFilepath;
	PL =  new ProgramLoader();
	outPut = 1;
}


FrameBuffer::~FrameBuffer(void)
{
}

void FrameBuffer::init(){
	
	PL->loadProgram(vertFP, fragFP);
	glGenFramebuffers(1, &FBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FBuffer);

	
	glGenTextures(1, &rendTexture);
	glBindTexture(GL_TEXTURE_2D, rendTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480,0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 640, 480);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,rendTexture,0);
	DrawBuffers[1] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, DrawBuffers);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			return;
	}

	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);
	GLfloat g_quad_vertex_buffer_data[] =
	 {
    -1, -1,
    1, -1,
    -1, 1,
    1, 1,
	 };
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	
	quad_programID = PL->getProgram();

	quad_attrib = glGetAttribLocation(quad_programID, "vs_position");

	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glEnableVertexAttribArray(quad_attrib);
	glVertexAttribPointer(quad_attrib, 2, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);

	outputLocation = glGetUniformLocation(quad_programID, "output");


}

void FrameBuffer::draw(int output){
	outPut = output;
	glUseProgram(quad_programID);
	glBindVertexArray(quad_VertexArrayID);
	glEnableVertexAttribArray(quad_attrib);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendTexture);
	glUniform1i(glGetUniformLocation(quad_programID, "input_texture"), 0);
	// 'Draw'
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUniform1i(outputLocation,outPut);
}

void FrameBuffer::bind(){
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, FBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 640, 480);

}
void FrameBuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,640,480);
}

void FrameBuffer::cleanUp(){
	glDisableVertexAttribArray(quad_attrib);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glFinish();
}