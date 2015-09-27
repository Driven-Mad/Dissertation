#ifndef PARTICALSYSTEM_H
#define PARTICALSYSTEM_H
//------------------------------------------------------------------
/// \file    Partical_System.h
/// \author  Lloyd Phillips
/// \brief   This is the Partical System
///          THIS CLASS WAS CREATED IN ASSIGNMENT 2 OF PROGRAMMING FOR GRAPHICS AND GAMES. 
///          SLIGHTLY MODIFIED FOR IMPOROVED FUNCTIONALITY.
//------------------------------------------------------------------
#include "ObjectLoader.h"
#include <iostream>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "ProgramLoader.h"
#include <string>
#include <gtc/random.hpp>
#include "ProgramLoader.h"
/// \brief struct for partical
struct partical{
	bool isAlive;///< is the partical alive
	float life; ///< current Life span of the partical
	glm::vec3 position; ///<position of the partical
	glm::vec3 velocity;///<velocity of the partical
};
class Partical_System{
public:
	/// \brief constructor, takes in the object file path that wants to be instanced.
	Partical_System(char *objFilepath);
	/// \brief destructor
	~Partical_System(void);
	/// \brief draws the instanced particals taking in the projection matrix, and vertex matrix
	/// as I have not done rotation, I've not used the model matrix as of yet as this will take an array of
	/// model matrixes for each individual flake. this will be done at a later date.
	void draw(glm::mat4 pM, glm::mat4 vM);
	/// \brief updates the particals taking in delta time, this is for movement.
	void updateSnow(float dt);

	private:
	/// \brief creates an individual partical taking in a position
	void createParticle(glm::vec4 position);
	/// \brief creates an entire partical system. needs the object file path
	void createParticleSystem(char *filepath);
	GLuint _VAO; ///<VAO to store buffers
	GLuint VertsBuffer,posBuffer; ///<Buffer
	GLuint _program; ///<Program for partical system
	GLuint maxParticles,particleCount; ///<Partical system attribute
	GLint _shaderViewMatLocation, _shaderProjMatLocation; ///<shader locations
	GLfloat gravity; ///<gravity variable
	GLfloat* particle_position_data; ///<position data to send to the shader.
	float area_size; ///<area size I felt easier to change a number of variables at the same time
	std::vector<partical> parts; ///<container to store all the particals.
	std::vector<glm::vec3> out_Verts; ///<VERTS for the partical
	std::vector<glm::vec2> out_UVs;///<UVS for the partical 
	std::vector<glm::vec3> out_Normals;///<NORMALS for the partical 
	std::vector<glm::vec3> out_Tangents;///<TANGENTS for the partical 
	std::vector<glm::vec3> out_BiTangents;///<BITANGENTS for the partical
	ObjectLoader *obj;
};
#endif ///!PARTICALSYSTEM_H

