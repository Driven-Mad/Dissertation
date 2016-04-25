#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Application.h"
#include <time.h>


int main(int argc, char *argv[]){
	//create a new application.
	Application *app = new Application;
	//Initialise all our variables in our application.
	srand(time(NULL));
	app->init();
	bool quit = false;
	while( !quit ){
		//input handler for our app
		//app->inputHandler();
		//run our application
		quit = app->run();
	}
	//cleaan up
	app->~Application();
	return 0;
}