#ifndef MODEL_H
#define MODEL_H
//------------------------------------------------------------------
/// \file    Model.h
/// \author  Lloyd Phillips
/// \brief   This is the Model class
//------------------------------------------------------------------
#include <glm.hpp>
#include "glew.h"
class Model
{
public:
	/// \brief Constructor calls InitialiseVAO and InitialiseShaders
	/// takes in the object filepath fragment shader filepath, vertex shader filepath.
	Model(char *objFP, char *fragShaderFP, char vertShaderFP);

	/// \brief Deconstructor
	~Model(void);
	/// \brief initialises vertex array objects with openGL and model
	void initaliseVAO();
	/// \brief intialises shaders
	void initaliseShaders();
	/// \brief updates the model
	void update(float DT);
	/// \brief draws model given camera view and projection matrix
	void draw(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	/// \brief sets position
	void setPosition(glm::vec3);
	/// \brief sets rotation
	void setRotation(glm::vec3);
	/// \brief gets position
	glm::vec3 getPosition();
	/// \brief gets rotation
	glm::vec3 getRotation();
	/// \brief gets program used.
	GLint getProgram();
private:
	glm::vec3 position; ///< Position of model
	glm::vec3 rotation; ///< Rotation of model
	GLuint VAO; ///< vertex Array object for model in openGL
	GLuint program;///< shader program
	GLint shaderModelMatLocation, shaderViewMatLocation,shaderProjMatLocation, shader3X3Location; ///<uniformlocations for matrix
	glm::mat4 modelMatrix; ///< model matrix
	glm::mat3 modelViewMatrix3x3; ///<model matrix in a 3x3 needed for normals
	unsigned int numVerts;///<number of verts in model
	char *objFP, *fragFP, *vertFP; ///< filepath storage

};
#endif //!MODEL_H
