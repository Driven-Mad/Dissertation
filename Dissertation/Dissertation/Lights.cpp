#include "Lights.h"


Lights::Lights(void)
{
	offsetPL = 0.0f;
	
	leftShiftPressed = false;
}


Lights::~Lights(void)
{
}
void Lights::newPointLight(glm::vec4 position, Radius Radius, glm::vec4 lightColour, glm::vec4 ambientColour){
	lightModelPos = glm::vec3(position.x,position.y,position.z);
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
	for(int i = 0; i<pointLights.size(); i++)
	pointLights[i].light->draw(viewMat, projMat);
}
void Lights::update(float dt){
	for(int i = 0; i<pointLights.size(); i++)
	pointLights[i].light->update(dt);
}
void Lights::initPointLights(){
	glGenBuffers(1,&uniformBlockPLs);

	glBindBuffer(GL_UNIFORM_BUFFER,uniformBlockPLs);
	glBufferData(GL_UNIFORM_BUFFER, (6 * sizeof(glm::vec4))*pointLights.size(), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,0,uniformBlockPLs,0, (6 * sizeof(glm::vec4))*pointLights.size());
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
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , &pointLights[x].PLconstant);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , &pointLights[x].PLliniear);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	for(int x = 0; x<pointLights.size(); x++){
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockPLs);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetPL,sizeof(glm::vec4) , &pointLights[x].PLquadratic);
		offsetPL += sizeof(glm::vec4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
void Lights::changePLPosition( int lightI){
	pointLights[0].light->setPosition(lightModelPos);
	pointLights[lightI].PLposition = glm::vec4(lightModelPos.x,lightModelPos.y,lightModelPos.z,1.0f);
}
void Lights::updatePLposition(){
	offsetPL = 0;
	for(int x = 0; x<pointLights.size(); x++){
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
					lightModelPos.z += 10.0f* delta_Time;
				}
				break;
			case SDLK_e:
				if(leftShiftPressed){
					lightModelPos.z -=10.0f* delta_Time;
				}
				break;
			case SDLK_w:
				if(leftShiftPressed){
					lightModelPos.y += 10.0f* delta_Time;
				}
				break;
			case SDLK_a:
				if(leftShiftPressed){
					lightModelPos.x -= 10.0f* delta_Time;
				}
				break;
			case SDLK_s:
				if(leftShiftPressed){
					lightModelPos.y -= 10.0f* delta_Time;
				}
				break;
			case SDLK_d:
				if(leftShiftPressed){
					lightModelPos.x += 10.0f* delta_Time;
				}
				break;
			case SDLK_LSHIFT:
				leftShiftPressed = true;
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
