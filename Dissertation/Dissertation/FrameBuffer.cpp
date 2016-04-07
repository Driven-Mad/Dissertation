#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(char *fragmentShaderFilepath,char *vertexShaderFilepath){
	rendTexture[0] = 0;
	quad_vertexbuffer = 0;
	fragFP = fragmentShaderFilepath;
	vertFP = vertexShaderFilepath;
	PL =  new ProgramLoader();
	outPut = 1;
}


FrameBuffer::~FrameBuffer(void){
}

void FrameBuffer::init(int winWidth,int winHeight){
	PL->loadProgram(vertFP, fragFP);
	glGenFramebuffers(1, &FBuffer[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, FBuffer[0]);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	
	glGenTextures(2, rendTexture);
	for(GLuint i = 0; i<2; i++){
		glBindTexture(GL_TEXTURE_2D, rendTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, winWidth, winHeight,0, GL_RGBA, GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + i,rendTexture[i],0);
	}
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	DrawBuffers[1] = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(2, DrawBuffers);
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

void FrameBuffer::initBlur(int winWidth,int winHeight){
	PL->loadProgram(vertFP, fragFP);
	glGenFramebuffers(2, FBuffer);
	glGenTextures(2, rendTexture);
	//glBindFramebuffer(GL_FRAMEBUFFER, FBuffer);
	for(GLuint i = 0; i<2; i++){
		glBindFramebuffer(GL_FRAMEBUFFER, FBuffer[i]);
		glBindTexture(GL_TEXTURE_2D, rendTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, winWidth, winHeight,0, GL_RGBA, GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 ,rendTexture[i],0);
	}
	//glDrawBuffers(2, DrawBuffers);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			return;
	}
	
	
	//glGenRenderbuffers(1, &depthRenderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, winWidth, winHeight);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	GLboolean horizontal = true, firstItter = true;
	GLuint amount = 10;
	for(GLuint i= 0; i<amount; i++){
		glBindFramebuffer(GL_FRAMEBUFFER, FBuffer[horizontal]);
		glUniform1i(glGetUniformLocation(PL->getProgram(), "horizontal"),horizontal);
		glBindTexture(GL_TEXTURE_2D,firstItter ? rendTexture[1] : FBuffer[!horizontal]);
		//renderQuad
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
		outputLocation = glGetUniformLocation(quad_programID, "output");
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glEnableVertexAttribArray(quad_attrib);
		glVertexAttribPointer(quad_attrib, 2, GL_FLOAT, false, 0, 0);

		horizontal = !horizontal;
		if(firstItter)
			firstItter = false;

		glBindVertexArray(0);
	}
	
	
	
}


void FrameBuffer::draw(int output){
	outPut = output;
	glUseProgram(quad_programID);
	glBindVertexArray(quad_VertexArrayID);
	glEnableVertexAttribArray(quad_attrib);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendTexture[0]);
	glUniform1i(glGetUniformLocation(quad_programID, "input_texture"), 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUniform1i(outputLocation,outPut);
}

void FrameBuffer::bind( int winWidth,int winHeight){
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, FBuffer[0]);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, winWidth, winHeight);

}
void FrameBuffer::unbind( int winWidth,int winHeight){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,winWidth,winHeight);
}

void FrameBuffer::cleanUp(){
	glDisableVertexAttribArray(quad_attrib);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glFinish();
}