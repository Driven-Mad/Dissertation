#include "Camera.h"


Camera::Camera(float screen_h, float screen_w)
{
	pers_val = screen_w/screen_h;
	cameraPosition = glm::vec3(0.0f, 0.5f,0.0f);
	cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
	cameraUp = glm::vec3(0.0f,1.0f,0.0f);
	field_of_view = 45.0f;
	leftAltPressed = false;
	sensitivity = 0.2f;
}


Camera::~Camera(void)
{
}

void Camera::update(float DT){
	
	projectionMatrix = glm::perspective(glm::radians(field_of_view),pers_val , 0.1f, 100.0f);
	viewMatrix = glm::lookAt(cameraPosition,cameraPosition+cameraFront,cameraUp);
	if(field_of_view <=1.0f){
		field_of_view = 1.0f;
	}
	if(field_of_view >=45.0f){
		field_of_view = 45.0f;
	}
}

void Camera::cameraMovement(float DT, SDL_Event &incomingEvent){
	
	float cameraSpeed = 30.0f * DT ;
	switch(incomingEvent.type){
	case SDL_QUIT:
		return;
		break;
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym ){
			case SDLK_w:
				if(leftAltPressed){
					cameraPosition += cameraSpeed * cameraFront;
				}
				break;
			case SDLK_a:
				if(leftAltPressed){
					cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
				}
				break;
			case SDLK_s:
				if(leftAltPressed){
					cameraPosition -= cameraSpeed * cameraFront;
				}
				break;
			case SDLK_d:
				if(leftAltPressed){
					cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
				}
				break;
			case SDLK_LALT:
				leftAltPressed = true;
				break;
		}
		break;
	case SDL_KEYUP:
		switch(incomingEvent.key.keysym.sym){
			case SDLK_LALT:
				leftAltPressed = false;
				break;
		}
		break;
	case SDL_MOUSEMOTION:
		mouse_X = incomingEvent.motion.x;
		mouse_Y = -incomingEvent.motion.y;
		mouse_X_offset = mouse_X - old_mouse_X;
		mouse_Y_offset = mouse_Y - old_mouse_Y;
		old_mouse_X = mouse_X;
		old_mouse_Y = mouse_Y;

		
		mouse_X_offset *= sensitivity;
		mouse_Y_offset *= sensitivity;

		yaw += mouse_X_offset;
		pitch += mouse_Y_offset;

		if(pitch> 89.0f){
			pitch = 89.0f;
		}
		if(pitch < -89.0f){
			pitch = -89.0f;
		}

		
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
		break;
	case SDL_MOUSEWHEEL:
		if(incomingEvent.wheel.y > 0){
			field_of_view -= cameraSpeed;
		}
		if(incomingEvent.wheel.y < 0){
			field_of_view += cameraSpeed;
		}
		break;
	}
		
}