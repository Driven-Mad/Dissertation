#pragma once
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
	Lightning(void);
	~Lightning(void);
	int BranchChance(int branchPercentageChance);
	float BranchLength();
	void Strike();
	void Init();
	void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	void LightningInput(float DT,SDL_Event &incomingEvent);
	void newBranch(glm::vec3 begin, int percentChance);
	/// \brief initialises vertex array objects with openGL and model
	void initaliseVAO();
	/// \brief intialises shaders
	void initaliseShaders();
	void setLoaded(bool load){alreadyLoaded = load;};
	void branchOff();
private:
	bool firstBranch;
	float strikeLength;
	glm::vec3 startingPoint;
	std::vector<LightningBranch> lightningStrike;
	std::vector<glm::vec3> m_vVerts;
	int predessingIndex;
	GLuint positionBuffer;
	glm::mat4 modelMatrix; ///< model matrix
	GLuint VAO; ///< vertex Array object for model in openGL
	GLuint program;///< shader program
	unsigned int numVerts;///<number of verts in model
	GLint shaderModelMatLocation, shaderViewMatLocation,shaderProjMatLocation;
	glm::vec3 position; ///< Position of model
	bool alreadyLoaded;
	int strikeBranchPercentage;
	bool coreStrike;
	
};

