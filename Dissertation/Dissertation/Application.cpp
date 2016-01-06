#include "Application.h"


Application::Application(void)
{
	winPosX = 100;
	winPosY = 100;
	winWidth = 1280;
	winHeight = 960;
	lastTime = SDL_GetTicks();
	lightPosition = glm::vec4(1.0f,1.0f,0.2f,1.0f);
	leftShiftPressed = false;
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
	window = SDL_CreateWindow("Dissertation", winPosX,winPosY,winWidth,winHeight,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer (window, -1,0);
	glContext = SDL_GL_CreateContext(window);
	if(!InitGL() ){
		return;
	}
	glEnable(GL_DEPTH_TEST);

	skyDome = new Model("assets/dome.obj","shaders/domeFragmentShader.txt","shaders/domeVertexShader.txt");
	house = new Model("assets/houseA_obj.obj","shaders/houseFragmentShader.txt","shaders/houseVertexShader.txt");
	plane = new Model("assets/plane.obj","shaders/planeFragmentShader.txt","shaders/planeVertexShader.txt");
	shelter = new Model("assets/Old_Shelter.obj","shaders/houseFragmentShader.txt","shaders/houseVertexShader.txt");
	car = new Model("assets/pickup_truck.obj","shaders/carFragmentShader.txt","shaders/carVertexShader.txt");
	light = new Model("assets/light.obj","shaders/lightFragmentShader.txt","shaders/lightVertexShader.txt");
	rain = new Model("assets/DECone.obj","shaders/rainFragmentShader.txt","shaders/rainVertexShader.txt");
	
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
	rain->loadTexture("assets/rain.bmp");
	rain->loadTexture("assets/rain2.bmp");
	rain->loadTexture("assets/rain3.bmp");
	fBuffer = new FrameBuffer("shaders/frameBuffFragmentShader.txt", "shaders/frameBuffVertexShader.txt");
	fBuffer->init(winWidth,winHeight);
	camera = new Camera(winHeight,winWidth);
}
void Application::run(){

	update();
	draw();
}
void Application::draw(){
	fBuffer->bind(winWidth,winHeight);
			//bind here
			car->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			house->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			skyDome->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			plane->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			shelter->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
			light->draw(camera->getViewMatrix(), camera->getProjectionMatrix());
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
	light->setLightPosition(lightPosition);
	glClearColor(1.0f,0.5f,0.3f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skyDome->update(delta_Time,lightPosition);
	house->update(delta_Time,lightPosition);
	plane->update(delta_Time,lightPosition);
	shelter->update(delta_Time,lightPosition);
	car->update(delta_Time,lightPosition);
	skyDome->rotateY(0.01,delta_Time);
	light->update(delta_Time,lightPosition);
	rain->setPosition(camera->getCameraPosition());
	rain->update(delta_Time,lightPosition);
	rain->updateUVS(delta_Time);
	light->setPosition(glm::vec3(lightPosition.x,lightPosition.y,lightPosition.z));
	camera->update(delta_Time);
	SDL_Event incomingEvent;
	while( SDL_PollEvent( &incomingEvent)){
		camera->cameraMovement(delta_Time,incomingEvent);
		lightMovement(incomingEvent);
	}

}
void Application::lightMovement(SDL_Event incomingEvent){
	
	switch(incomingEvent.type){
	case SDL_QUIT:
		return;
		break;
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym ){
			case SDLK_q:
				if(leftShiftPressed){
					lightPosition.z += 10.0f* delta_Time;
				}
				break;
			case SDLK_e:
				if(leftShiftPressed){
					lightPosition.z -=10.0f* delta_Time;
				}
				break;
			case SDLK_w:
				if(leftShiftPressed){
					lightPosition.y += 10.0f* delta_Time;
				}
				break;
			case SDLK_a:
				if(leftShiftPressed){
					lightPosition.x -= 10.0f* delta_Time;
				}
				break;
			case SDLK_s:
				if(leftShiftPressed){
					lightPosition.y -= 10.0f* delta_Time;
				}
				break;
			case SDLK_d:
				if(leftShiftPressed){
					lightPosition.x += 10.0f* delta_Time;
				}
				break;
			case SDLK_LSHIFT:
				leftShiftPressed = true;
				break;
		}
		break;
	case SDL_KEYUP:
		switch(incomingEvent.key.keysym.sym){
			case SDLK_LSHIFT:
				leftShiftPressed = false;
				break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		SDL_GetMouseState(&mouse_X, &mouse_Y);
		break;
	case SDL_MOUSEWHEEL:
		break;
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