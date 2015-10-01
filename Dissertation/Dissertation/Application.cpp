#include "Application.h"


Application::Application(void)
{
	winPosX = 100;
	winPosY = 100;
	winWidth = 1280;
	winHeight = 960;
	camera_Zoom = -15.0f;
	camera_Left_Right = -4.5f;
	camera_Up_Down = -3.0f;
	lastTime = SDL_GetTicks();
	camera_rotate = 0.0f;
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
	textLoad = new TextureLoader();
	textLoad2 = new TextureLoader();
	textLoad3 = new TextureLoader();
	textLoad4 = new TextureLoader();
	textLoad5 = new TextureLoader();
	textLoad->loadTexture("assets/Sky.bmp",skyDome->getProgram());
	textLoad->loadTexture("assets/SkyNormal.bmp",skyDome->getProgram());
	textLoad2->loadTexture("assets/houseA.bmp",house->getProgram());
	textLoad2->loadTexture("assets/houseANormal.bmp",house->getProgram());
	textLoad3->loadTexture("assets/grass.bmp",plane->getProgram());
	textLoad3->loadTexture("assets/grassNormal.bmp",plane->getProgram());
	textLoad5->loadTexture("assets/Curve.bmp", shelter->getProgram());
	textLoad5->loadTexture("assets/Metal line bump.bmp",shelter->getProgram());
	textLoad4->loadTexture("assets/pickup_blue.bmp",car->getProgram());
	textLoad4->loadTexture("assets/pickup_blue_n.bmp",car->getProgram());
	fBuffer = new FrameBuffer("shaders/frameBuffFragmentShader.txt", "shaders/frameBuffVertexShader.txt");
	fBuffer->init();
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
	fBuffer->bind(winWidth,winHeight);
			//bind here
			textLoad4->enableTextures();
			car->draw(viewMatrix, projectionMatrix);
			textLoad2->enableTextures();
			house->draw(viewMatrix,projectionMatrix);
			textLoad->enableTextures();
			skyDome->draw(viewMatrix, projectionMatrix);
			textLoad3->enableTextures();
			plane->draw(viewMatrix,projectionMatrix);
			textLoad5->enableTextures();
			shelter->draw(viewMatrix,projectionMatrix);
			
			
	//unbind here
	fBuffer->unbind(winWidth,winHeight);
	//draw here
	fBuffer->draw(0);
	// cleanup here
	fBuffer->cleanUp();
	SDL_GL_SwapWindow( window );
}
void Application::update(float DT){
	glClearColor(1.0f,0.5f,0.3f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skyDome->update(DT);
	house->update(DT);
	plane->update(DT);
	shelter->update(DT);
	car->update(DT);
	//////! ROTATING SKY DOME !/////////
	// update the rotation angle
	float rotation = skyDome->getRotation().y + DT * 0.01;
	skyDome->setRotation(glm::vec3(skyDome->getRotation().x, rotation ,skyDome->getRotation().z));
	while( rotation > (3.14159265358979323846 * 2.0) ){
		rotation -= (3.14159265358979323846 * 2.0);
	}
	//translate based on identity matrix, and position.
	skyDome->setModelMatrix(glm::translate(glm::mat4(1.0f), skyDome->getPosition()));
	// Next, we rotate this matrix in the y-axis by rotation:
	skyDome->setModelMatrix(glm::rotate(skyDome->getModelMatrix(), rotation, glm::vec3(0,1,0) ));
	//////! END ROTATING SKY DOME !/////////

	projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(camera_Left_Right,camera_Up_Down,camera_Zoom) );
	viewMatrix = glm::rotate(viewMatrix, camera_rotate, glm::vec3(0,1,0));
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
						
						break;
					case SDLK_UP:
						
						break;
					case SDLK_LEFT:
						
						break;
					case SDLK_RIGHT:
						
						break;
					case SDLK_q:
						camera_rotate += 0.5f;
						break;
					case SDLK_e:
						camera_rotate -= 0.5f;
						break;
					case SDLK_w:
						camera_Up_Down -= 0.5f;
						break;
					case SDLK_a:
						camera_Left_Right += 0.5f;
						break;
					case SDLK_s:
						camera_Up_Down += 0.5f;
						break;
					case SDLK_d:
						camera_Left_Right -= 0.5f;
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouse_X, &mouse_Y);
				break;
			case SDL_MOUSEWHEEL:
				if(incomingEvent.wheel.y > 0){
					camera_Zoom += 0.5f;
				}
				if(incomingEvent.wheel.y < 0){
					camera_Zoom -= 0.5f;
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