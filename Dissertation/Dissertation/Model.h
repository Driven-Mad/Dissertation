#ifndef MODEL_H
#define MODEL_H
//------------------------------------------------------------------
/// \file    Model.h
/// \author  Lloyd Phillips
/// \brief   This is the Model class
//------------------------------------------------------------------
#include <glm.hpp>
#include "glew.h"
#include "ObjectLoader.h"
#include <vector>
#include "ProgramLoader.h"
#include <gtc\matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class Model
{
public:
	/// \brief Constructor calls InitialiseVAO and InitialiseShaders
	/// takes in the object filepath fragment shader filepath, vertex shader filepath.
	Model(char *objFP, char *fragShaderFP, char *vertShaderFP);

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
	void setPosition(glm::vec3 a);
	/// \brief sets rotation
	void setRotation(glm::vec3 a);
	/// \brief gets position
	glm::vec3 getPosition();
	/// \brief gets rotation
	glm::vec3 getRotation();
	/// \brief gets program used.
	GLint getProgram();

	glm::mat4 getModelMatrix();
	void setModelMatrix(glm::mat4 a);
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
	std::vector<glm::vec3> out_Verts; ///< verticies produced from object loader
	std::vector<glm::vec2> out_UVs;///< UVS produced from object loader
	std::vector<glm::vec3> out_Normals;///< Normals produced from object loader
	std::vector<glm::vec3> out_Tangents;///< Tangents produced from object loader
	std::vector<glm::vec3> out_BiTangents;///< BiTangents produced from object loader
};
#endif //!MODEL_H
