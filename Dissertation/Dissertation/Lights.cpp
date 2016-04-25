#include "Lights.h"

Lights::Lights(void)
{
	//set default values
	offsetPL = 0;
	leftShiftPressed = false;
	activeLight = -1;
	PLShadowProjection = glm::perspective(glm::radians(90.0f),1.0f,0.1f,25.0f);
}

Lights::~Lights(void)
{
}

void Lights::newPointLight(glm::vec4 position, Radius Radius, glm::vec4 lightColour, glm::vec4 ambientColour)
{
	//Set it up so the last light we add is the active light at first.
	activeLightModelPos = glm::vec3(position.x,position.y,position.z);
	activeLight++;
	//Temporary pointLight & Set up variables.
	PointLight pl;
	pl.PLposition = position;
	pl.PLconstant = 1.0f;
	pl.PLlightColour = lightColour;
	pl.PLambient = ambientColour;
	pl.PLactive = true;
	//Handle the differnet Radius values. 
	switch(Radius)
	{
	case RADIUS7:
		pl.PLliniear = 0.7f;
		pl.PLquadratic = 1.8f;
		break;
	case RADIUS13:
		pl.PLliniear = 0.35f;
		pl.PLquadratic = 0.44f;
		break;
	case RADIUS20:
		pl.PLliniear = 0.22f;
		pl.PLquadratic = 0.2f;
		break;
	case RADIUS32:
		pl.PLliniear = 0.14f;
		pl.PLquadratic = 0.07f;
		break;
	case RADIUS50:
		pl.PLliniear = 0.09f;
		pl.PLquadratic = 0.032f;
		break;
	case RADIUS100:
		pl.PLliniear = 0.045f;
		pl.PLquadratic = 0.0075f;
		break;
	case RADIUS600:
		pl.PLliniear = 0.007f;
		pl.PLquadratic = 0.0002f;
		break;
	case RADIUS3250:
		pl.PLliniear = 0.0014f;
		pl.PLquadratic = 0.000007f;
		break;
	default:
		pl.PLliniear = 0.07f;
		pl.PLquadratic = 1.8f;
		break;
	}
	pl.ShadowTransforms[0] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(1.0f,0.0f,0.0f) ,glm::vec3(0.0f,-1.0f, 0.0f)));
	pl.ShadowTransforms[1] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f, 0.0f)));
	pl.ShadowTransforms[2] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(0.0f,1.0f,0.0f) ,glm::vec3(0.0f, 0.0f, 1.0f)));
	pl.ShadowTransforms[3] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f, 0.0f,-1.0f)));
	pl.ShadowTransforms[4] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(0.0f,0.0f,1.0f) ,glm::vec3(0.0f,-1.0f, 0.0f)));
	pl.ShadowTransforms[5] = (PLShadowProjection * glm::lookAt(glm::vec3(position),glm::vec3(position) + glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f, 0.0f)));
	pointLights.push_back(pl);
	//Create  anew model to represent our light.
	Model* t = new Model("assets/light.obj","shaders/lightFragmentShader.txt","shaders/lightVertexShader.txt");
	PointLightsModels.push_back(t);

	//The size of the memory required for point lights block (7 * 16 offsets)
	size = ((7 * sizeof(glm::vec4)) +(4 * sizeof(glm::vec4) * 6 ))* pointLights.size();
}

void Lights::draw(glm::mat4 viewMat, glm::mat4 projMat)
{
	for(unsigned int i = 0; i<PointLightsModels.size(); i++)
	{
		if(pointLights[i].PLactive)
		{
			//If the light is active, draw the model to represent it. 
			//PointLightsModels[i]->draw(viewMat, projMat, glm::mat4(0.0f),glm::vec3(0.0f));
		}
	}
}

void Lights::update(float dt)
{
	//Reset the offset
	offsetPL = 0;
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glm::vec3 temp = glm::vec3(pointLights[x].PLposition.x,pointLights[x].PLposition.y,pointLights[x].PLposition.z);
		//set position of the models.
		PointLightsModels[x]->setPosition(temp);
		if(pointLights[x].PLactive)
		{
			//if the light is active, update it's model.
			PointLightsModels[x]->update(dt);
		}
		//set poisition of the lights. 
		pointLights[x].PLposition = glm::vec4(temp.x,temp.y,temp.z,1.0f);
		//send the data for the positions of the lights to the shaders.
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLposition));
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	//Update the active light seperatly to be the active light positions. 
	PointLightsModels[activeLight]->setPosition(activeLightModelPos);
	pointLights[activeLight].PLposition = glm::vec4(activeLightModelPos.x,activeLightModelPos.y,activeLightModelPos.z,1.0f);

	//Set the offset to be where the offset of the bools start. 
	offsetPL= booloffsetsPL;
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		//send if the lights are active or not to the shaders.
		
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(bool) , &pointLights[x].PLactive);
		offsetPL += sizeof(glm::vec4);
	}
	offsetPL= shadowOffSetsPL;
	for(unsigned int i =0; i<6; i++){
		for(unsigned int x = 0; x<pointLights.size(); x++)
		{
			pointLights[x].ShadowTransforms[0] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(1.0f,0.0f,0.0f) ,glm::vec3(0.0f,-1.0f, 0.0f)));
			pointLights[x].ShadowTransforms[1] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f, 0.0f)));
			pointLights[x].ShadowTransforms[2] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(0.0f,1.0f,0.0f) ,glm::vec3(0.0f, 0.0f, 1.0f)));
			pointLights[x].ShadowTransforms[3] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f, 0.0f,-1.0f)));
			pointLights[x].ShadowTransforms[4] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(0.0f,0.0f,1.0f) ,glm::vec3(0.0f,-1.0f, 0.0f)));
			pointLights[x].ShadowTransforms[5] = (PLShadowProjection * glm::lookAt(glm::vec3(pointLights[x].PLposition),glm::vec3(pointLights[x].PLposition) + glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f, 0.0f)));

			glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4 , &pointLights[x].ShadowTransforms[i]);
			offsetPL += (sizeof(glm::vec4)*4);

			//glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[1]);
			//offsetPL += (sizeof(glm::vec4)*4);
			//glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[2]);
			//offsetPL += (sizeof(glm::vec4)*4); 
			//glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[3]);
			//offsetPL += (sizeof(glm::vec4)*4);
			//glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[4]);
			//offsetPL += (sizeof(glm::vec4)*4);
			//glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[5]);
			//offsetPL += (sizeof(glm::vec4)*4);

		}
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//for(unsigned int x = 0; x<pointLights.size(); x++)
	//{
	//	//send if the lights are active or not to the shaders.
	//	
	//	glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::mat4) *6, &pointLights[x].ShadowTransforms);
	//	offsetPL += (sizeof(glm::vec4)*4) *6;
	//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//}
}

void Lights::toggleActive()
{
	//Toggles if the active light is on or off. 
	pointLights[activeLight].PLactive = !pointLights[activeLight].PLactive;
}

void Lights::toggleActive(int Light){
	pointLights[Light].PLactive = !pointLights[Light].PLactive;
}

void Lights::initPointLights(){
	//Initalise the buffer for the pointlights 
	glGenBuffers(1,&uniformBlockPLs);

	glBindBuffer(GL_UNIFORM_BUFFER,uniformBlockPLs);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,0,uniformBlockPLs,0, size);
}

void Lights::bindUniformBlockPointLights(GLuint program){
	//Bind the buffer to an index. 
	GLuint uniformBlockIndex = glGetUniformBlockIndex(program,"PointLights");
	glUniformBlockBinding(program,uniformBlockIndex,0);
}

void Lights::bindDataPointLights(){
	// bind the data to it's correct place and send to the shaders.
	//POSITIONS
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{

		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLposition));
		offsetPL += sizeof(glm::vec4);

	}
	//AMBIENTS
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLambient));
		offsetPL += sizeof(glm::vec4);
	}
	//LIGHT COLOURS
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLlightColour));
		offsetPL += sizeof(glm::vec4);
	}
	//CONSTANTS
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLconstant);
		offsetPL += sizeof(glm::vec4);
	}
	//LINIEAR
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLliniear);
		offsetPL += sizeof(glm::vec4);
	}
	//QUADRATICS
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLquadratic);
		offsetPL += sizeof(glm::vec4);
	}
	booloffsetsPL = offsetPL; //Get the bool offset as this is the only one other than position that is being sent constantly
	//ACTIVE OR !ACTIVE
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(bool) , &pointLights[x].PLactive);
		offsetPL += sizeof(glm::vec4);
	}


	shadowOffSetsPL = offsetPL;
	for(unsigned int x = 0; x<pointLights.size(); x++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[0]);
		offsetPL += (sizeof(glm::vec4)*4);
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[1]);
		offsetPL += (sizeof(glm::vec4)*4);
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[2]);
		offsetPL += (sizeof(glm::vec4)*4);
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[3]);
		offsetPL += (sizeof(glm::vec4)*4);
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[4]);
		offsetPL += (sizeof(glm::vec4)*4);
		glBufferSubData(GL_UNIFORM_BUFFER,offsetPL,sizeof(glm::vec4)*4, &pointLights[x].ShadowTransforms[5]);
		offsetPL += (sizeof(glm::vec4)*4);

	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Lights::changeSpecificLightPosition(int lightID, glm::vec4 newPosition)
{
	//Change a specific lights (lightID) position and model position to be new position 
	PointLightsModels[lightID]->setPosition(glm::vec3(newPosition.x,newPosition.y,newPosition.z));
	pointLights[lightID].PLposition = newPosition;
}

void Lights::lightInput(SDL_Event incomingEvent, float delta_Time)
{	
	//Manages light input and differnet movement depending on keybinds. As well as toggling through the lights and toggle if active or not. 
	switch(incomingEvent.type)
	{
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym )
		{

		case SDLK_q:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.z += 10.0f* delta_Time;
				}
			}
			break;
		case SDLK_e:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.z -=10.0f* delta_Time;
				}
			}
			break;
		case SDLK_w:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.y += 10.0f* delta_Time;
				}
			}
			break;
		case SDLK_a:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.x -= 10.0f* delta_Time;
				}
			}
			break;
		case SDLK_s:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.y -= 10.0f* delta_Time;
				}
			}
			break;
		case SDLK_d:
			if(pointLights[activeLight].PLactive)
			{
				if(leftShiftPressed)
				{
					activeLightModelPos.x += 10.0f* delta_Time;
				}
			}
			break;

		case SDLK_LSHIFT:
			leftShiftPressed = true;
			break;
		case SDLK_TAB:
			activeLight ++;
			if( activeLight >= pointLights.size() )
			{
				activeLight = 0;
			}
			activeLightModelPos = glm::vec3(pointLights[activeLight].PLposition.x, pointLights[activeLight].PLposition.y, pointLights[activeLight].PLposition.z);
			printf("\n The Current Active light is: %i ", activeLight);
			break;
		case SDLK_c:
			toggleActive();
			break;
		}
		break;
	case SDL_KEYUP:
		switch(incomingEvent.key.keysym.sym)
		{
		case SDLK_LSHIFT:
			leftShiftPressed = false;
			break;
		}
		break;
	}

}

///////////////////////////////////////////
//////////DIRECTIONAL LIGHTS //////////////
///////////////////////////////////////////
void Lights::newDirectionalLight(glm::vec4 direction, glm::vec4 lightColour, glm::vec4 ambientColour)
{
	//Instantiate a new Directional lights with the passed in values. 
	directionalLight.direction = direction;
	directionalLight.lightColour = lightColour;
	directionalLight.ambient = ambientColour;
	DLlightProjection = glm::ortho(-5.0f,5.0f,5.0f,-5.0f,-6.0f,6.0f);
	DLlightView = glm::lookAt(glm::vec3(direction),glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));
	DLlightSpaceMatrix = DLlightProjection * DLlightView;
}


void Lights::initDirectionLights()
{
	//Initalise the buffer for the directional lights 
	glGenBuffers(1,&uniformBlockDLs);

	//Bind the data size to required block
	glBindBuffer(GL_UNIFORM_BUFFER,uniformBlockDLs);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,1,uniformBlockDLs, 0, 3 * sizeof(glm::vec4)	);
}

void Lights::bindUniformBlockDirectionalLights(GLuint program)
{
	//Bind the block to it's index.
	GLuint uniformBlockIndex = glGetUniformBlockIndex(program,"DirectionalLights");
	glUniformBlockBinding(program,uniformBlockIndex,1);
}

void Lights::bindDataDirectionalLights()
{
	//Bind and send all the data off to the shaders in a UBO
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockDLs);
	glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(glm::vec4) , glm::value_ptr(directionalLight.direction));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockDLs);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4),sizeof(glm::vec4) , glm::value_ptr(directionalLight.lightColour));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockDLs);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4)*2,sizeof(glm::vec4) , glm::value_ptr(directionalLight.ambient));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
