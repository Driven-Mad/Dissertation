#include "Application.h"


Application::Application(void)
{
	winPosX = 100;
	winPosY = 100;
	winWidth = 1280;
	winHeight = 960;
	camera_Zoom = -2.5f;
	camera_Left_Right = 0.0f;
	camera_Up_Down = 0.0f;
	lastTime = SDL_GetTicks();
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
}
void Application::run(float DT){
	current = SDL_GetTicks();
	delta_Time = (float) (current - lastTime) / 1000.0f;
	lastTime = current;
	if( delta_Time < (1.0f/50.0f) ){
			SDL_Delay((unsigned int) (((1.0f/50.0f) - delta_Time)*1000.0f) );
		}
	update(DT);
	draw();
}
void Application::draw(){
	SDL_GL_SwapWindow( window );
}
void Application::update(float DT){
	glClearColor(1.0f,0.5f,0.3f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(camera_Left_Right,camera_Up_Down,camera_Zoom) );
}
void Application::inputHandler(){
	SDL_Event incomingEvent;
		while( SDL_PollEvent( &incomingEvent)){
			switch(incomingEvent.type){
			case SDL_QUIT:
				return;
				break;
			case SDL_KEYDOWN:
				switch( incomingEvent.key.keysym.sym ){
					case SDLK_DOWN:
						camera_Up_Down += 0.1f;
						break;
					case SDLK_UP:
						camera_Up_Down -= 0.1f;
						break;
					case SDLK_LEFT:
						camera_Left_Right += 0.1f;
						break;
					case SDLK_RIGHT:
						camera_Left_Right -= 0.1f;
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouse_X, &mouse_Y);
				break;
			case SDL_MOUSEWHEEL:
				if(incomingEvent.wheel.y > 0){
					camera_Zoom += 0.1f;
				}
				if(incomingEvent.wheel.y < 0){
					camera_Zoom -= 0.1f;
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