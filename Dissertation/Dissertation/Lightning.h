#ifndef LIGHTNING_H
#define LIGHTNING_H
//------------------------------------------------------------------
/// \file    Lightning.h
/// \author  Created by Lloyd Phillips
/// \brief   This is the Lightning Class which handles spawning and lightning behaviour
//------------------------------------------------------------------
#include <vector>
#include "Utilities.h"
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include "ProgramLoader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "LightningBranch.h"

class Lightning
{
	
public:
	///\brief default constructor
	Lightning(void);
	///\brief default destructor
	~Lightning(void);
	///\brief returns if the branch should branch off depending on the  branch percentage past in. 
	bool BranchChance(int branchPercentageChance);
	///\brief renturns a branch length
	float BranchLength();
	///\brief main function for causing a bolt of lightning
	void Strike();
	///\brief Initialise all variables/call strike
	void Init();


	///\brief draw function to call draw calls. 
	void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	///\brief handler for input for lightning (mainly for debugging)
	void LightningInput(float DT,SDL_Event &incomingEvent);
	///\brief creating a new branch has never been easier, takes in a begin point, 
	///and what the chance of it branching off is, if it is a branch or not, and how many branches it can have off it
	void newBranch(glm::vec3 begin, int percentChance, bool isBranchOff, int amountOfBranches);
	///\brief initialises vertex array objects with openGL and model
	void initaliseVAO();
	///\brief intialises shaders
	void initaliseShaders();

	///\breif branch off the main core
	void branchOff();
	///\brief finish off drawing the branched off branches
	void finishOffBranches();
	///\brief finds the core branch
	void FindCoreBranch();
	///\brief finds 3 major points within the lightning to find the best place to put the spot lights
	void get3MajorPoints(glm::vec4 &pointA, glm::vec4& pointB, glm::vec4 &pointC);

	int getBranchesToDraw(){return branchesToDraw;};
	int getCoreToDraw(){return coreToDraw;};
	int getCoreBranchSize(){return m_vCoreVerts.size();};
	int getBranchSize(){return m_vBranchedVerts.size();};
private:
	///\brief draw the core branch as a thicker line than the rest
	void DrawCore(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	///\brief draw the branches as a smaller line than the core
	void DrawBranch(glm::mat4 viewMatrix, glm::mat4 projMatrix);

	bool firstBranch;///<check if it is the first branch being drawm
	glm::vec3 startingPoint; ///<Starting point of the strike
	std::vector<LightningBranch> lightningStrike; ///<Storage container for our lightning strike
	std::vector<glm::vec3> m_vCoreVerts; ///<storage container for our core verticies
	std::vector<glm::vec3> m_vBranchedVerts;///<storage container for our branched verticies
	int predessingIndex; ///<find out what the parent is of the core branch
	GLuint corePositionBuffer, branchPositionBuffer; ///<Position buffers
	glm::mat4 modelMatrix; ///< model matrix
	GLuint VAOcore, VAObranch; ///< vertex Array object for model in openGL
	GLuint program;///< shader program
	unsigned int numCoreVerts, numBranchVerts;///<number of verts in model
	GLint shaderModelMatLocation, shaderViewMatLocation,shaderProjMatLocation; ///<Shader Locations
	glm::vec3 position; ///< Position of model
	int strikeBranchPercentage; ///<storage for strike branch percentage to be used throughout
	bool hitGround;///<Bool to check if the strike has hit the ground
	int lightningCounter; ///<Counter to go into Storm Functoion !*NOT YET IMPLEMENTED*!
	bool finishedBranches; ///<Check if we've finished off finishing off the branches

	int coreToDraw;
	int branchesToDraw;
	int branchedStart;
	bool drawBranch;

};

#endif