#include "Model.h"


Model::Model(char *objFP, char *fragShaderFP, char vertShaderFP)
{
}


Model::~Model(void)
{
}

void Model::initaliseVAO(){
}

void Model::initaliseShaders(){
}

void Model::update(float DT){
}

void Model::draw(glm::mat4 viewMatrix, glm::mat4 projMatrix){
}

void Model::setPosition(glm::vec3){
}

void Model::setRotation(glm::vec3){
}

glm::vec3 Model::getPosition(){
}

glm::vec3 Model::getRotation(){
}

GLint Model::getProgram(){
}