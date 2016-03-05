#pragma once
#include <glm.hpp>
#include "Model.h"
enum Radius{
RADIUS7,RADIUS13,RADIUS20,RADIUS32,RADIUS50,RADIUS100
};

class Lights
{
public:
	struct PointLight{
		glm::vec4 PLposition;
		glm::vec4 PLambient;
		glm::vec4 PLlightColour;
		float PLconstant;
		float PLliniear;
		float PLquadratic;
		Model *light;
	};
	struct DirectionalLight{
		glm::vec4 direction;
		glm::vec4 lightColour;
		glm::vec4 ambient;
	};
	struct SpotLight{
	};
	Lights(void);
	~Lights(void);
	void newPointLight(glm::vec4 position,Radius r, glm::vec4 lightColour, glm::vec4 ambientColour);
	void newDirectionalLight(glm::vec4 direction, glm::vec4 lightColour, glm::vec4 ambientColour);
	void initPointLights();		//use block 0
	void initDirectionLights(); //use block 1
	void initSpotLights();		//use block 2
	void bindUniformBlockPointLights(GLuint program);		//use block 0
	void bindUniformBlockDirectionalLights(GLuint program);	//use block 1
	void bindUniformBlockSpotLights(GLuint program);		//use block 2
	void bindDataPointLights();
	void bindDataDirectionalLights();
	void bindDataSpotLights();
	void changePLPosition( int light);
	void updatePLposition();
	void lightMovement(SDL_Event incomingEvent, float delta_Time);
	void draw(glm::mat4 viewMat, glm::mat4 projMat);
	void update(float dt);
private:
	GLuint uniformBlockPLs, uniformBlockDLs, uniformBlockSLs;
	std::vector<PointLight> pointLights;
	DirectionalLight directionalLight;
	glm::vec3 lightModelPos;
	glm::vec3 activeLightModelPos;
	unsigned int offsetPL;
	bool leftShiftPressed;

	glm::vec4 positions	[5];
	glm::vec4 ambient	[5];
	glm::vec4 lightcolor[5];
	float constant		[5];
	float linear		[5];
	float quadratic		[5];

	int activeLight;
	bool printed;
};

