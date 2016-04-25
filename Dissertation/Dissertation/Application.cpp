#include "Application.h"


Application::Application(void)
{
	winPosX = 0;
	winPosY = 0;
	winWidth = 1920;
	winHeight = 1080;
	lastTime = SDL_GetTicks();
	exit = false;
	bloom = false;
	exposure = 0.4f;
	shadowRendered = false;
	rainAndLightningOn = true;
	timerStorm = rand() % 20 + 2;
	timerFlash = 10.0f;
	flashCount = 5;
	flashed= false;
	finishedForking = true;
}


Application::~Application(void)
{
	SDL_GL_DeleteContext( glContext );
	SDL_DestroyWindow( window );
	SDL_Quit();
}

bool Application::InitGL(){
	//Enable experimental features.
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if( GLEW_OK != error){
		std::cerr<<"Error: GLEW failed to initialise with message: "<< glewGetErrorString(error) <<std::endl;
		return false;
	}
	std::cout<<"INFO: Using GLEW "<< glewGetString(GLEW_VERSION)<<std::endl;
	std::cout<<"INFO: OpenGL Vendor: "<< glGetString( GL_VENDOR ) << std::endl;
	std::cout<<"INFO: OpenGL Renderer: "<< glGetString( GL_RENDERER ) << std::endl;
	std::cout<<"INFO: OpenGL Version: "<< glGetString( GL_VERSION ) << std::endl;
	std::cout<<"INFO: OpenGL Shading Language Version: "<< glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

	return true;
}

void Application::init(){
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		std::cout <<"SDL Could Not Initalise." <<std::endl;
		return;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow("Dissertation", winPosX,winPosY,winWidth,winHeight,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED);
	renderer = SDL_CreateRenderer (window, -1,0);
	glContext = SDL_GL_CreateContext(window);
	if(!InitGL() ){
		return;
	}
	glEnable(GL_DEPTH_TEST);

	skyDome = new Model("assets/dome.obj","shaders/domeFragmentShader.txt","shaders/domeVertexShader.txt");
	house = new Model("assets/houseA_obj.obj","shaders/lightingFragmentShader.txt","shaders/lightingVertexShader.txt");
	plane = new Model("assets/plane.obj","shaders/lightingFragmentShader.txt","shaders/planeVertexShader.txt");
	shelter = new Model("assets/Old_Shelter.obj","shaders/lightingFragmentShader.txt","shaders/lightingVertexShader.txt");
	car = new Model("assets/pickup_truck.obj","shaders/lightingFragmentShader.txt","shaders/lightingVertexShader.txt");
	rain = new Rain();
	lightning = new Lightning();
	skyDome->loadTexture("assets/Sky.bmp");
	skyDome->loadTexture("assets/SkyNormal.bmp");
	house->loadTexture("assets/houseA.bmp");
	house->loadTexture("assets/houseANormal.bmp");
	plane->loadTexture("assets/grass.bmp");
	plane->loadTexture("assets/grassNormal.bmp");
	shelter->loadTexture("assets/Curve.bmp");
	shelter->loadTexture("assets/Metal line bump.bmp");
	car->loadTexture("assets/pickup_blue.bmp");
	car->loadTexture("assets/pickup_blue_n.bmp");
	//bloomFBuffer = new FrameBuffer("shaders/BloomFragmentShader.txt", "shaders/BloomVertexShader.txt");


	//NORMAL FB//////////////////////////////
	ShaderBloomFinal = new ProgramLoader();
	ShaderBloomFinal->loadProgram("shaders/BloomFinalVertexShader.txt", "shaders/BloomFinalFragmentShader.txt");
	ShaderBlurFB = new ProgramLoader();
	ShaderBlurFB->loadProgram("shaders/BlurVertexShader.txt", "shaders/BlurFragmentShader.txt");
	//fBuffer = new FrameBuffer("shaders/frameBuffFragmentShader.txt", "shaders/frameBuffVertexShader.txt");
	//fBuffer->init(winWidth,winHeight);

	//
	glGenFramebuffers(1, &HDRFBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, HDRFBuffer);

	glGenTextures(2, rendTexture);
	for(GLuint i = 0; i < 2; i++){
		glBindTexture(GL_TEXTURE_2D, rendTexture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, winWidth, winHeight,0, GL_RGB, GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + i,GL_TEXTURE_2D, rendTexture[i],0);
	}

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, winWidth, winHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	GLuint DrawBuffers[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer not complete!" << std::endl;
	}else{
		std::cout << "\n Framebuffer complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER,0);


	glUseProgram(ShaderBlurFB->getProgram());
	glGenFramebuffers(2, PingPongFBuffer);
	glGenTextures(2,PingPongRendTextures);

	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, PingPongFBuffer[i]);
		glBindTexture(GL_TEXTURE_2D, PingPongRendTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, winWidth, winHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PingPongRendTextures[i], 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "Framebuffer not complete!" << std::endl;
		}else{
			std::cout << "Framebuffer complete!" << std::endl;
		}
	}
	//NORMAL SHADOWS
	glGenFramebuffers(1, &depthMapFbuffer);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//POINT LIGHT SHADOWS
	for (int x=0; x< 5; x++)
	{
		glGenFramebuffers(1, &cubeMapFrameBuffer[x]);

		glGenTextures(1, &depthCubeMap[x]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap[x]);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		for(GLuint i = 0; i<6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_DEPTH_COMPONENT32F, 1024,1024,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);


		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

		glBindFramebuffer(GL_FRAMEBUFFER, cubeMapFrameBuffer[x]);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap[x],0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "Point Light Shadowws Framebuffer not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_TEXTURE_CUBE_MAP);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	quadVAO = 0;
	//glGenVertexArrays(1, &quad_VertexArrayID);
	//glBindVertexArray(quad_VertexArrayID);
	//GLfloat g_quad_vertex_buffer_data[] =
	// {
	//-1, -1,
	//1, -1,
	//-1, 1,
	//1, 1,
	// };
	//glGenBuffers(1, &quad_vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	//
	//
	//quad_programID = ShaderBloomFinal->getProgram();
	//quad_attrib = glGetAttribLocation(quad_programID, "vs_position");
	//
	//glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	//glEnableVertexAttribArray(quad_attrib);
	//glVertexAttribPointer(quad_attrib, 2, GL_FLOAT, false, 0, 0);
	//glBindVertexArray(0);

	//!NORMAL FB//////////////////////////////



	camera = new Camera(winHeight,winWidth);

	lightningTimer = 10.0f;

	lightHandler = new Lights();

	//POINT LIGHTS
	lightHandler->newPointLight(glm::vec4(-0.6f,0.3f,0.05f,1.0f),RADIUS7,glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.1f,0.1f,0.2f,1.0f));
	lightHandler->newPointLight(glm::vec4(0.6f,0.3f,0.05f,1.0f),RADIUS7,glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.1f,0.1f,0.2f,1.0f));
	lightHandler->newPointLight(glm::vec4(-0.8,-0.4f,0.05f,1.0f),RADIUS20,glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.1f,0.1f,0.2f,1.0f));	//RETURN TO RADIUS 50
	lightHandler->newPointLight(glm::vec4(0.8f,-0.4f,0.05f,1.0f),RADIUS20,glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.1f,0.1f,0.2f,1.0f));	//RETURN TO RADIUS 50
	lightHandler->newPointLight(glm::vec4(-0.8f,0.4f,0.05f,1.0f),RADIUS20,glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(0.1f,0.1f,0.2f,1.0f));	//RETURN TO RADIUS 50
	lightHandler->initPointLights();
	lightHandler->bindUniformBlockPointLights(plane->getProgram());
	lightHandler->bindUniformBlockPointLights(car->getProgram());
	lightHandler->bindUniformBlockPointLights(house->getProgram());
	lightHandler->bindUniformBlockPointLights(shelter->getProgram());
	lightHandler->bindDataPointLights();
	//DIRECTIONAL LIGHT vec3(-2.0f, 4.0f, -1.0f)
	lightHandler->newDirectionalLight(glm::vec4(-2.0f, 4.0f, -1.0f,1.0f), glm::vec4(0.3f,0.3f,0.3f,1.0f), glm::vec4(0.1f,0.1f,0.1f,1.0f));
	lightHandler->initDirectionLights();
	lightHandler->bindUniformBlockDirectionalLights(plane->getProgram());
	lightHandler->bindUniformBlockDirectionalLights(car->getProgram());
	lightHandler->bindUniformBlockDirectionalLights(house->getProgram());
	lightHandler->bindUniformBlockDirectionalLights(shelter->getProgram());
	lightHandler->bindDataDirectionalLights();
}
bool Application::run(){

	update();
	draw();
	return exit;
}
void Application::draw(){
	glEnable(GL_DEPTH_TEST);
	if(!shadowRendered)
	{
		//NORMAL SHADOWS.
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER,depthMapFbuffer);


		glClear(GL_DEPTH_BUFFER_BIT);
		car		->drawDepth(lightHandler->getLightSpaceMatrix());
		house	->drawDepth(lightHandler->getLightSpaceMatrix());
		shelter	->drawDepth(lightHandler->getLightSpaceMatrix());
		skyDome	->drawDepth(lightHandler->getLightSpaceMatrix());
		plane	->drawDepth(lightHandler->getLightSpaceMatrix());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//!NORMAL SHADOWS
		car			->setDepthMap(depthMap);
		house		->setDepthMap(depthMap);
		skyDome		->setDepthMap(depthMap);
		plane		->setDepthMap(depthMap);
		shelter		->setDepthMap(depthMap);
		shadowRendered = true;
	}

	//PL SHADOWS.
	for(int i = 0; i<5; i++){
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER,cubeMapFrameBuffer[i]);


		glClear(GL_DEPTH_BUFFER_BIT);
		car		->drawDepth(lightHandler->getLightSpaceMatrix(), i);
		house	->drawDepth(lightHandler->getLightSpaceMatrix(), i);
		skyDome	->drawDepth(lightHandler->getLightSpaceMatrix(), i);
		plane	->drawDepth(lightHandler->getLightSpaceMatrix(), i);
		shelter	->drawDepth(lightHandler->getLightSpaceMatrix(), i);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		car			->setCubeDepthMap(depthCubeMap[i],i);
		house		->setCubeDepthMap(depthCubeMap[i],i);
		skyDome		->setCubeDepthMap(depthCubeMap[i],i);
		plane		->setCubeDepthMap(depthCubeMap[i],i);
		shelter		->setCubeDepthMap(depthCubeMap[i],i);
		//!PL SHADOWS
	}





	//NORMAL SCENE
	glBindFramebuffer(GL_FRAMEBUFFER, HDRFBuffer);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, winWidth, winHeight);
	//bind 
	lightHandler->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
	car			->draw(camera->getViewMatrix(), camera->getProjectionMatrix(),lightHandler->getLightSpaceMatrix(),camera->getCameraPosition());
	house		->draw(camera->getViewMatrix(), camera->getProjectionMatrix(),lightHandler->getLightSpaceMatrix(),camera->getCameraPosition());
	skyDome		->draw(camera->getViewMatrix(), camera->getProjectionMatrix(),lightHandler->getLightSpaceMatrix(),camera->getCameraPosition());
	plane		->draw(camera->getViewMatrix(), camera->getProjectionMatrix(),lightHandler->getLightSpaceMatrix(),camera->getCameraPosition());
	shelter		->draw(camera->getViewMatrix(), camera->getProjectionMatrix(),lightHandler->getLightSpaceMatrix(),camera->getCameraPosition());
	if (rainAndLightningOn){
		if(finishedForking){
			lightning->Draw(camera->getViewMatrix(), camera->getProjectionMatrix());
		}
		rain->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
	}
	//unbind here
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0,0,winWidth,winHeight);

	glUseProgram(ShaderBlurFB->getProgram());
	///Blur bright fragments with gausian blur
	GLboolean horizontal = true, first_iteration = true;
	GLuint amount = 10;

	for (GLuint i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, PingPongFBuffer[horizontal]); 
		glUniform1i(glGetUniformLocation(ShaderBlurFB->getProgram(), "horizontal"), horizontal);
		glUniform1i(glGetUniformLocation(ShaderBlurFB->getProgram(), "image"), 0);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if( first_iteration )
		{
			glBindTexture(GL_TEXTURE_2D,rendTexture[1]);
			//std::cout<<"********************** rendTexture[1] = "<<rendTexture[1]<<std::endl;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D,PingPongRendTextures[!horizontal]);
		}
		RenderQuad(ShaderBlurFB->getProgram());
		horizontal = !horizontal;

		if (first_iteration)
			first_iteration = false;

	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D,0);
	//glUseProgram(ShaderBlurFB->getProgram());
	glUseProgram(ShaderBloomFinal->getProgram());

	//glUniform1i(glGetUniformLocation(ShaderBloomFinal->getProgram(), "bloomBlur"),1);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rendTexture[0]);
	GLuint TL = glGetUniformLocation(ShaderBloomFinal->getProgram(), "scene");
	glUniform1i(TL, 0);
	//GLint tL  = glGetUniformLocation( ShaderBloomFinal->getProgram(), "scene");
	//glUniform1i(tL, 0);
	//glUseProgram(ShaderBlurFB->getProgram());
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, PingPongRendTextures[!horizontal]);
	TL = glGetUniformLocation(ShaderBloomFinal->getProgram(), "bloomBlur");
	glUniform1i(TL, 1);
	//tL  = glGetUniformLocation( ShaderBloomFinal->getProgram(), "bloomBlur");
	//glUniform1i(tL,1);

	glUniform1i(glGetUniformLocation(ShaderBloomFinal->getProgram(), "bloom"), bloom);
	glUniform1f(glGetUniformLocation(ShaderBloomFinal->getProgram(), "exposure"), exposure);
	RenderQuad(ShaderBloomFinal->getProgram());
	////draw here
	//glUseProgram(quad_programID);
	//glBindVertexArray(quad_VertexArrayID);
	//glEnableVertexAttribArray(quad_attrib);
	//
	//glActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, rendTexture[0]);
	//glUniform1i(glGetUniformLocation(quad_programID, "input_texture"), 0);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//// cleanup here
	//glDisableVertexAttribArray(quad_attrib);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	//glFinish();

	SDL_GL_SwapWindow( window );
	//glUseProgram(0);
}
void Application::update()
{

	current = SDL_GetTicks();
	delta_Time = (float) (current - lastTime) / 1000.0f;
	lastTime = current;
	if( delta_Time < (1.0f/50.0f) ){
		SDL_Delay((unsigned int) (((1.0f/50.0f) - delta_Time)*1000.0f) );
	}
	//lightningTimer -= 10.0f * delta_Time;
	//if(lightningTimer <= 0.0f)
	//{
	//	lightning->Init();
	//	lightningTimer = util::randF(3.0f,10.0f);
	//}
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skyDome->update(delta_Time);
	house->update(delta_Time);
	plane->update(delta_Time);
	shelter->update(delta_Time);
	car->update(delta_Time);
	skyDome->rotateY(0.04f,delta_Time);
	rain->update(delta_Time,camera);
	camera->update(delta_Time);
	storm(delta_Time);
	lightning->get3MajorPoints(lightningLightA,lightningLightB,lightningLightC);
	lightHandler->changeSpecificLightPosition(2,lightningLightA);
	lightHandler->changeSpecificLightPosition(3,lightningLightB);
	lightHandler->changeSpecificLightPosition(4,lightningLightC);
	lightHandler->update(delta_Time);

	SDL_Event incomingEvent;
	while( SDL_PollEvent( &incomingEvent))
	{
		if(incomingEvent.type == SDL_QUIT)
		{
			exit = true;
		}
		camera->cameraMovement(delta_Time,incomingEvent);
		lightHandler->lightInput(incomingEvent,delta_Time);
		rain->rainInput(delta_Time,incomingEvent);
		lightning->LightningInput(delta_Time,incomingEvent);
		if(incomingEvent.type ==SDL_KEYDOWN)
		{
			if(incomingEvent.key.keysym.sym == SDLK_b)
			{
				bloom = !bloom;
			}
			if (incomingEvent.key.keysym.sym == SDLK_t)
			{
				exposure += 0.1f;
			}
			if(incomingEvent.key.keysym.sym == SDLK_r)
			{
				exposure -= 0.1f;
			}
			if (incomingEvent.key.keysym.sym == SDLK_k)
			{
				rainAndLightningOn = !rainAndLightningOn;
			}
		}
	}

}

float Application::getDeltaTime(){
	return delta_Time;
}
int Application::getWidth(){
	return winWidth;
}
int Application::getHeight(){
	return winHeight;
}

void Application::RenderQuad(GLint program)
{
	glUseProgram(program);
	if(quadVAO == 0){
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Application::RenderSmallQuad(GLint program)
{
	glUseProgram(program);
	if(quadSmallVAO == 0){
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-0.3f, 0.3f, 0.0f, 0.0f, 1.0f,
			-0.3f, -0.3f, 0.0f, 0.0f, 0.0f,
			0.3f, 0.3f, 0.0f, 1.0f, 1.0f,
			0.3f, -0.3f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadSmallVAO);
		glGenBuffers(1, &quadSmallVBO);
		glBindVertexArray(quadSmallVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadSmallVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadSmallVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
}



void Application::storm(float deltaTime)
{
	timerStorm-= 10.0f *deltaTime;

	if(timerStorm <= 0.0f)
	{
		//if(!flashed && lightning->getBranchesToDraw() == 0 && lightning->getCoreToDraw() == 0 ){
		if(!flashed ){
			lightning->Init();
			finishedForking = true;
			flashed = true;
		}
		timerFlash-= 100.0f *deltaTime;
		if(timerFlash <= 0.0f  && flashed == true ){
			if(flashCount >= 0)
			{
				lightHandler->toggleActive(2);
				lightHandler->toggleActive(3);
				lightHandler->toggleActive(4);
				flashCount -= 1.0f;
				timerFlash = rand() % 5 + 5;

			}
			if(flashCount == 0 ){
				flashCount = 5;
				timerFlash = rand() % 5 + 5;
				timerStorm = rand() % 20 + 2;
				flashed = false;
				finishedForking = false;
				lightHandler->toggleActive(2);
				lightHandler->toggleActive(3);
				lightHandler->toggleActive(4);
				
			}
			
			
		}
	}
	
};
