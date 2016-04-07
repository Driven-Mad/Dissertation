#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H
//------------------------------------------------------------------
/// \file    ProgramLoader.h
/// \author  Lloyd Phillips
/// \brief   This is the program loader class
///          THIS CLASS WAS CREATED IN ASSIGNMENT 2 OF PROGRAMMING FOR GRAPHICS AND GAMES. 
///          SLIGHTLY MODIFIED FOR IMPOROVED FUNCTIONALITY.
//------------------------------------------------------------------
#include <string>
#include <sstream>
#include <glm.hpp>
#include "glew.h"
#include <fstream>
#include <iostream>
class ProgramLoader{
public:
	/// \brief constructor for programloader
	ProgramLoader(void);
	/// \brief destructor for programloader
	~ProgramLoader(void);
	/// \brief loads vertex Shader, taking in filepath
	void loadVShader(char *filepath);
	/// \brief loads fragment Shader, taking in filepath
	void loadFShader(char *filepath);
	/// \brief loads Geometry Shader, taking in filepath
	void loadGShader(char *filepath);
	/// \brief loads program, taking in vertex shader and Fragment shader filepaths.
	void loadProgram(char *vShaderFP, char *fShaderFP);
	/// \brief loads program, taking in vertex shader and Fragment shader filepaths.
	void loadProgram(char *vShaderFP, char *fShaderFP, char *gShaderFP);
	/// \brief error checking to see if the shader compiled ok
	bool CheckShaderCompiled( GLint shader );
	/// \brief gets the program
	/// \returns GLint program
	GLint getProgram();

private:
	std::string Vdata, Fdata, Gdata; ///<storage for the shader
	GLuint program; ///<the actual program
	GLuint vShader, fShader,gShader; ///<the shader
};

#endif ///!PROGRAMLOADER_H