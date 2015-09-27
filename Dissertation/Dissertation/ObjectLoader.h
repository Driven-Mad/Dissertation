#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H
//------------------------------------------------------------------
/// \file    ObjectLoader.h
/// \author  Lloyd Phillips
/// \brief   This is the ObjectLoader class
/// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
/// this was used as reference only, the code itself was written by me 
/// taking the tutorial as reference (for function calls) and I have also adjusted the outcome
/// and modified to add tangents and bitangents.
/// THIS CLASS WAS CREATED IN ASSIGNMENT 2 OF PROGRAMMING FOR GRAPHICS AND GAMES. 
/// SLIGHTLY MODIFIED FOR IMPOROVED FUNCTIONALITY.
//------------------------------------------------------------------
#include "Utilities.h"
#include<iostream>
#include<vector>
#include <glm.hpp>
#include <SDL.h>
class ObjectLoader
{
public:
	ObjectLoader(void);
	~ObjectLoader(void);
	///\brief Loads an object taking in the filepath of the object,
	/// and 5 containers for the different attributes of the model.
	bool loadOBJ(char * filepath, std::vector<glm::vec3> &outVerts,
	std::vector<glm::vec2> &outUVs,
	std::vector<glm::vec3> &outNormals, 
	std::vector<glm::vec3> &outTangents, 
	std::vector<glm::vec3> &outBiTangents);

private:

};

#endif; ///!OBJECTLOADER_H