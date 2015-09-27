#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
//------------------------------------------------------------------
/// \file    TextureLoader.h
/// \author  Lloyd Phillips
/// \brief   This is the Texture Loader class
///          THIS CLASS WAS CREATED IN ASSIGNMENT 2 OF PROGRAMMING FOR GRAPHICS AND GAMES. 
///          SLIGHTLY MODIFIED FOR IMPOROVED FUNCTIONALITY.
//------------------------------------------------------------------
#include<iostream>
#include<vector>
#include <glm.hpp>
#include <SDL.h>
#include "glew.h"
#include <SDL_image.h>


class TextureLoader
{
public:
	/// \brief constructor for the texture loader
	TextureLoader(void);
	/// \brief deconstructor for the texture loader
	~TextureLoader(void);
	/// \brief loads a texture, taking in a filepath
	
	void loadTexture(char *fileName, GLint Program);
	std::vector<GLuint> getTextures();

	void enableTextures();
private:
	GLuint loadTEXT(char * filepath);
	std::vector<GLuint> textures; ///<container for our textures.
	std::vector<GLint> texturesLocation; ///<container for our textures.
};
#endif; ///!TEXTURELOADER_H
