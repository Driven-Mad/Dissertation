#include "Lights.h"


Lights::Lights(void)
{
	offsetPL = 0;
	printed = false;
	leftShiftPressed = false;
	activeLight = -1;
}


Lights::~Lights(void)
{
}
void Lights::newPointLight(glm::vec4 position, Radius Radius, glm::vec4 lightColour, glm::vec4 ambientColour){
	lightModelPos = glm::vec3(position.x,position.y,position.z);
	activeLightModelPos = glm::vec3(position.x,position.y,position.z);
	activeLight++;
	PointLight pl;
	pl.PLposition = position;
	pl.PLconstant = 1.0f;
	pl.PLlightColour = lightColour;
	pl.PLambient = ambientColour;
	pl.light = new Model("assets/light.obj","shaders/lightFragmentShader.txt","shaders/lightVertexShader.txt");
	switch(Radius){
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
		default:
			pl.PLliniear = 0.09f;
			pl.PLquadratic = 0.032f;
			break;
	}
	pointLights.push_back(pl);
}
void Lights::draw(glm::mat4 viewMat, glm::mat4 projMat){
	for(unsigned int i = 0; i<pointLights.size(); i++){
		pointLights[i].light->draw(viewMat, projMat);
	}
}
void Lights::update(float dt){
	for(unsigned int i = 0; i<pointLights.size(); i++)
	{
		pointLights[i].light->update(dt);
	}
}
void Lights::initPointLights(){
	glGenBuffers(1,&uniformBlockPLs);

	glBindBuffer(GL_UNIFORM_BUFFER,uniformBlockPLs);
	glBufferData(GL_UNIFORM_BUFFER, (6 * sizeof(glm::vec4) * pointLights.size()), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,0,uniformBlockPLs,0, 6 * sizeof(glm::vec4) *pointLights.size());
}
void Lights::bindUniformBlockPointLights(GLuint program){
	GLuint uniformBlockIndex = glGetUniformBlockIndex(program,"PointLights");
	glUniformBlockBinding(program,uniformBlockIndex,0);
}
void Lights::bindDataPointLights(){
	
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLposition));
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLambient));
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLlightColour));
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLconstant);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLliniear);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) , &pointLights[x].PLquadratic);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	////Position
	//for(int x = 0; x<pointLights.size(); x++)
	//{
	//	positions[x] = pointLights[x].PLposition;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(glm::vec4) * pointLights.size() , &positions[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
	//
	////AMBIENT
	//for(int x = 0; x<pointLights.size(); x++){
	//	ambient[x] = pointLights[x].PLambient;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4)* pointLights.size() , &ambient[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
	//
	////lightColor
	//for(int x = 0; x<pointLights.size(); x++){
	//	lightcolor[x] = pointLights[x].PLlightColour;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) * pointLights.size() , &lightcolor[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
	//
	////CONSTANT
	//for(int x = 0; x<pointLights.size(); x++){
	//	constant[x] = pointLights[x].PLconstant;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) * pointLights.size(), &constant[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
	//
	////LINIAR
	//for(int x = 0; x<pointLights.size(); x++){
	//	linear[x] = pointLights[x].PLliniear;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) * pointLights.size() , &linear[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
	//
	////QUADRATIC
	//for(int x = 0; x<pointLights.size(); x++){
	//	quadratic[x] = pointLights[x].PLquadratic;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(float) * pointLights.size(), &quadratic[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//offsetPL += (sizeof(glm::vec4) * pointLights.size());
}
void Lights::changePLPosition( int lightI){
	for(int x = 0; x<pointLights.size(); x++){
		glm::vec3 temp = glm::vec3(pointLights[x].PLposition.x,pointLights[x].PLposition.y,pointLights[x].PLposition.z);
		//glm::vec3 temp = glm::vec3(0.0f,0.0f,0.0f);
		//if(!printed){
		//printf("\n the model var is: %f, %f, %f...",lightModelPos.x,lightModelPos.y,lightModelPos.z);
		//printf("\n the temp var is: %f, %f, %f...",pointLights[0].PLposition.x,pointLights[0].PLposition.y,pointLights[0].PLposition.z);
		//printed = true;
		//}
		pointLights[x].light->setPosition(temp);
		//printf("\n the model var is: %f, %f, %f...",lightModelPos.x,lightModelPos.y,lightModelPos.z);
		pointLights[x].PLposition = glm::vec4(temp.x,temp.y,temp.z,1.0f);
	}
	pointLights[activeLight].light->setPosition(activeLightModelPos);
	pointLights[activeLight].PLposition = glm::vec4(activeLightModelPos.x,activeLightModelPos.y,activeLightModelPos.z,1.0f);
}
void Lights::updatePLposition(){
	//for(int x = 0; x<pointLights.size(); x++)
	//{
	//	positions[x] = pointLights[x].PLposition;
	//}
	//glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0,sizeof(glm::vec4) * pointLights.size() , &positions[0]);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	offsetPL = 0;
	for(unsigned int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , glm::value_ptr(pointLights[x].PLposition));
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void Lights::lightMovement(SDL_Event incomingEvent, float delta_Time){
	
	switch(incomingEvent.type){
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym ){
			case SDLK_q:
				if(leftShiftPressed){
					activeLightModelPos.z += 10.0f* delta_Time;
				}
				break;
			case SDLK_e:
				if(leftShiftPressed){
					activeLightModelPos.z -=10.0f* delta_Time;
				}
				break;
			case SDLK_w:
				if(leftShiftPressed){
					activeLightModelPos.y += 10.0f* delta_Time;
				}
				break;
			case SDLK_a:
				if(leftShiftPressed){
					activeLightModelPos.x -= 10.0f* delta_Time;
				}
				break;
			case SDLK_s:
				if(leftShiftPressed){
					activeLightModelPos.y -= 10.0f* delta_Time;
				}
				break;
			case SDLK_d:
				if(leftShiftPressed){
					activeLightModelPos.x += 10.0f* delta_Time;
				}
				break;
			case SDLK_LSHIFT:
				leftShiftPressed = true;
				break;
			case SDLK_TAB:
				activeLight ++;
				if(activeLight >= pointLights.size()){
					activeLight = 0;
				}
				activeLightModelPos = glm::vec3(pointLights[activeLight].PLposition.x,pointLights[activeLight].PLposition.y,pointLights[activeLight].PLposition.z);
				break;
		}
		break;
	case SDL_KEYUP:
		switch(incomingEvent.key.keysym.sym){
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
void Lights::newDirectionalLight(glm::vec4 direction, glm::vec4 lightColour, glm::vec4 ambientColour){
	directionalLight.direction = direction;
	directionalLight.lightColour = lightColour;
	directionalLight.ambient = ambientColour;
}
void Lights::initDirectionLights(){
	glGenBuffers(1,&uniformBlockDLs);

	glBindBuffer(GL_UNIFORM_BUFFER,uniformBlockDLs);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,1,uniformBlockDLs, 0, 3 * sizeof(glm::vec4)	);
}
void Lights::bindUniformBlockDirectionalLights(GLuint program){
	GLuint uniformBlockIndex = glGetUniformBlockIndex(program,"DirectionalLights");
	glUniformBlockBinding(program,uniformBlockIndex,1);
}
void Lights::bindDataDirectionalLights(){
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
