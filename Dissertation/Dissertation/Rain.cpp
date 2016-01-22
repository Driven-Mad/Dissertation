#include "Rain.h"


Rain::Rain(void)
{
	
	doubleEndedCone = new Model("assets/DECone.obj","shaders/rainFragmentShader.txt","shaders/rainVertexShader.txt");
	doubleEndedCone->loadTexture("assets/rain.bmp");
	doubleEndedCone->loadTexture("assets/rain2.bmp");
	doubleEndedCone->loadTexture("assets/rain3.bmp");
	rotate = false;
	rotateNormal = true;
	angle = 20.0f;
}

Rain::~Rain(void)
{
}
void Rain::draw(glm::mat4 viewMat, glm::mat4 projMat){
	doubleEndedCone->draw(viewMat, projMat);
}
void Rain::update(float DT,Camera* c){
	doubleEndedCone->setPosition(c->getCameraPosition());
	doubleEndedCone->update(DT);
	doubleEndedCone->updateUVS(DT);
	if(rotate){
		doubleEndedCone->setModelMatrix(glm::translate(glm::mat4(1.0f), doubleEndedCone->getPosition()));
		cross = glm::cross(glm::vec3(0.0f,1.0f,0.0f),-c->getCameraDirection());
		doubleEndedCone->setModelMatrix(glm::rotate(doubleEndedCone->getModelMatrix(),glm::radians(angle),cross));
	}
}


void Rain::rainInput(float DT,SDL_Event &incomingEvent){
	switch(incomingEvent.type){
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym ){
			case SDLK_w:
				rotate = true;
				break;
			case SDLK_a:
				rotate = true;
				break;
			case SDLK_s:
				rotate = true;
				break;
			case SDLK_d:
				rotate = true;
				break;
			case SDLK_LALT:
				
				break;
		}
		break;
	case SDL_KEYUP:
		switch(incomingEvent.key.keysym.sym){
			case SDLK_LALT:
				break;
			case SDLK_w:
				rotate = false;
				break;
			case SDLK_a:
				rotate = false;
				break;
			case SDLK_s:
				rotate = false;
				break;
			case SDLK_d:
				rotate = false;
				break;
		}
		break;
	case SDL_MOUSEMOTION:
		
		break;
}
}