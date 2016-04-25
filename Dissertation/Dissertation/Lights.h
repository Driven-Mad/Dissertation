#ifndef LIGHTS_H
#define LIGHTS_H
//------------------------------------------------------------------
/// \file    Light.h
/// \author  Created by Lloyd Phillips
/// \brief   This is the Light Class which handles all the multiple lights of the program
//------------------------------------------------------------------
#include <glm.hpp>
#include "Model.h"
enum Radius
{
RADIUS7,RADIUS13,RADIUS20,RADIUS32,RADIUS50,RADIUS100,RADIUS600, RADIUS3250
};

class Lights
{
public:
	///STRUCTURE for POINTLIGHT 
	///\brief contains position, ambient color, light color for calculating the lighting and color 
	/// constant, quadratic and liniar for fall off and finally if it's active or not
	struct PointLight
	{
		glm::vec4 PLposition;
		glm::vec4 PLambient;
		glm::vec4 PLlightColour;
		float PLconstant;
		float PLliniear;
		float PLquadratic;
		bool PLactive;
		glm::mat4 ShadowTransforms[6];
	};

	///STRUCTURE for DIRECTIONAL LIGHT 
	///\brief contains position, ambient color, light color for calculating the lighting and color */
	struct DirectionalLight
	{
		glm::vec4 direction;
		glm::vec4 lightColour;
		glm::vec4 ambient;
	};
	///STRUCTURE for SPOT LIGHT 
	///\briefcurrently empty
	struct SpotLight
	{
	};
	///\brief default constructor
	Lights(void);
	///\brief default destructor
	~Lights(void);
	///\brief Instantiate a new Point Light.
	void newPointLight(glm::vec4 position,Radius r, glm::vec4 lightColour, glm::vec4 ambientColour);
	///\brief Instantiate a new Directional Light (although only one is required)
	void newDirectionalLight(glm::vec4 direction, glm::vec4 lightColour, glm::vec4 ambientColour);
	///\brief Initalise all instatiated point lights
	void initPointLights();		//use block 0
	///\brief initalise all instatiated directional lights
	void initDirectionLights(); //use block 1
	///\brief initialise all instatiated spot lights !*CURRENTLY DOES NOTHING*!
	void initSpotLights();		//use block 2

	///\brief Bind the uniform block for specified amount of offset etc. for all the point lights to a Uniformblock. 
	void bindUniformBlockPointLights(GLuint program);		//use block 0
	///\brief Bind the uniform block for specified amount of offset etc. for all the Directional lights to a Uniformblock. 
	void bindUniformBlockDirectionalLights(GLuint program);	//use block 1
	///\brief Bind the uniform block for specified amount of offset etc. for all the spotlights to a Uniformblock. 
	void bindUniformBlockSpotLights(GLuint program);		//use block 2

	///\brief Change a specific light position, taking in the ID of the light and a new position
	void changeSpecificLightPosition(int lightID, glm::vec4 newPosition);
	///\brief Bind all the data of the point lights to it's UBO
	void bindDataPointLights();
	///\brief Bind all the data of the directional lights to it's UBO
	void bindDataDirectionalLights();
	///\brief Bind all the data of the spot lights to it's UBO
	void bindDataSpotLights();
	///\brief Returns the points lights. 
	std::vector<PointLight> getPointLights(){return pointLights;};

	glm::mat4 getLightSpaceMatrix(){return DLlightSpaceMatrix;};

	///\brief Toggle the which is the currently active light. 
	void toggleActive();
	void toggleActive(int Light);
	///\brief handle the lightInput such as the toggle active, and movement
	void lightInput(SDL_Event incomingEvent, float delta_Time);
	///\brief draw the visual appearance of the lights
	void draw(glm::mat4 viewMat, glm::mat4 projMat);
	///\Update the movements, the positions, send positions and bools off to shaders etc.
	void update(float dt);

private:
	GLuint uniformBlockPLs, uniformBlockDLs, uniformBlockSLs; ///<Different Uniform Blocks for the different Lights
	std::vector<PointLight> pointLights; ///<Storage container for all of the point lights in our scene
	DirectionalLight directionalLight; ///<Storage for our directional light values
	glm::vec3 activeLightModelPos; ///<Get and update the current active light model position to be assigned to the active light.
	unsigned int offsetPL, booloffsetsPL, shadowOffSetsPL; ///<Data offset
	bool leftShiftPressed; ///< boolean if the Left shift is pressed or not.
	std::vector<Model*> PointLightsModels; ///<Storage for our light model representation
	int size;/// <variable to store the size that the UNIFORM BUFFER OBJECT needs
	unsigned int activeLight; ///<Index for which the current active light the user has selected.

	glm::mat4 DLlightProjection, DLlightView, DLlightSpaceMatrix;
	glm::mat4 PLShadowProjection;
};

#endif //!LIGHTS_H