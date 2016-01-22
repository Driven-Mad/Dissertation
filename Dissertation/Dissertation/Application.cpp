#include "Application.h"


Application::Application(void)
{
	winPosX = 0;
	winPosY = 0;
	winWidth = 1920;
	winHeight = 1080;
	lastTime = SDL_GetTicks();
	exit = false;
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

	fBuffer = new FrameBuffer("shaders/frameBuffFragmentShader.txt", "shaders/frameBuffVertexShader.txt");
	fBuffer->init(winWidth,winHeight);

	camera = new Camera(winHeight,winWidth);

	lightHandler= new Lights();

	//POINT LIGHTS
	lightHandler->newPointLight(glm::vec4(0.6f,6.0f,0.05f,1.0f),RADIUS13,glm::vec4(1,1,1,1),glm::vec4(0.1f,0.1f,0.2f,1.0f));
	lightHandler->initPointLights();
	lightHandler->bindUniformBlockPointLights(plane->getProgram());
	lightHandler->bindUniformBlockPointLights(car->getProgram());
	lightHandler->bindUniformBlockPointLights(house->getProgram());
	lightHandler->bindUniformBlockPointLights(shelter->getProgram());
	lightHandler->bindDataPointLights();
	//DIRECTIONAL LIGHT
	lightHandler->newDirectionalLight(glm::vec4(-0.2f,-1.0f,-0.3f,1.0f), glm::vec4(0.3f,0.3f,0.3f,1.0f), glm::vec4(0.2f,0.2f,0.2f,1.0f));
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
	fBuffer->bind(winWidth,winHeight);
			//bind 
			lightHandler->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			car->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			house->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			skyDome->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			plane->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			shelter->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			rain->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			
			
	//unbind here
	fBuffer->unbind(winWidth,winHeight);
	//draw here
	fBuffer->draw(0);
	// cleanup here
	fBuffer->cleanUp();
	SDL_GL_SwapWindow( window );
}
void Application::update(){
	
	current = SDL_GetTicks();
	delta_Time = (float) (current - lastTime) / 1000.0f;
	lastTime = current;
	if( delta_Time < (1.0f/50.0f) ){
			SDL_Delay((unsigned int) (((1.0f/50.0f) - delta_Time)*1000.0f) );
	}
	glClearColor(1.0f,0.5f,0.3f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skyDome->update(delta_Time);
	house->update(delta_Time);
	plane->update(delta_Time);
	shelter->update(delta_Time);
	car->update(delta_Time);
	skyDome->rotateY(0.04,delta_Time);
	rain->update(delta_Time,camera);
	camera->update(delta_Time);
	lightHandler->changePLPosition(0);
	lightHandler->updatePLposition();
	lightHandler->update(delta_Time);

	SDL_Event incomingEvent;
	while( SDL_PollEvent( &incomingEvent)){
		if(incomingEvent.type == SDL_QUIT){
			exit = true;
		}
		camera->cameraMovement(delta_Time,incomingEvent);
		lightHandler->lightMovement(incomingEvent,delta_Time);
		rain->rainInput(delta_Time,incomingEvent);
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