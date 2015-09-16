#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Application.h"


int main(int argc, char *argv[]){
	//create a new application.
	Application *app = new Application;
	//Initialise all our variables in our application.
	app->init();
	bool go = true;
	while( go ){
		//input handler for our app
		app->inputHandler();
		//run our application
		app->run(app->getDeltaTime());
	}
	//cleaan up
	app->~Application();
	return 0;
}