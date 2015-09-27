#include "Partical_System.h"

Partical_System::Partical_System(char *objFilepath){
	//initalise variables
	VertsBuffer = 0;
	posBuffer = 0;
	maxParticles = 2000;
	particleCount = 0;
	gravity = 0.98f;
	area_size = 5.0f;
	_VAO = 0;
	_program = 0;
	particle_position_data = new GLfloat[maxParticles * 3];
	//create a program laoder
	ProgramLoader *PL = new ProgramLoader;
	//load a program with chosen shaders.
	PL->loadProgram("PsVertexShader.txt", "PsFragmentShader.txt");
	_program = PL->getProgram();
	//get locations of the view and projectiob matrixs
	_shaderViewMatLocation = glGetUniformLocation( _program, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _program, "projMat" );
	//create the partical system
	createParticleSystem(objFilepath);
}


Partical_System::~Partical_System(void){
}

void Partical_System::createParticleSystem(char *filepath){
	//load a new object, need all 5 containers to use, but only interested in the verts.
	obj = new ObjectLoader;
	obj->loadOBJ(filepath, out_Verts, out_UVs, out_Normals, out_Tangents, out_BiTangents);
	//generate a vertex array object
	glGenVertexArrays(1, &_VAO );
	glBindVertexArray(_VAO);
	//Variable for storing a VBO, generate a buffer, 
	//then "bind"/actiate it to the VBO, apply the data, 
	//then link to the shader via pointer & enable the array.
	//Vertex Buffer
	glGenBuffers(1, &VertsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertsBuffer);
	glBufferData(GL_ARRAY_BUFFER, out_Verts.size() * sizeof(glm::vec3), &out_Verts[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,VertsBuffer);
	

	//Poisition Buffer
	//changed to stream draw to enable the data to be updated constantly. this allows me to use instancing
	//for the partical system
	glGenBuffers(1, &posBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat),NULL,GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,posBuffer);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0, (void*)0);
	
	//create the particals
	for(unsigned int i =0; i<maxParticles; i++){
		glm::vec4 pos = glm::linearRand(glm::vec4(-area_size, -area_size, -area_size, 0.2f), glm::vec4(area_size, area_size, area_size, 0.2f));
		createParticle(pos);
		int temp = 3*particleCount;
		particle_position_data[temp +0] = pos.x;
		particle_position_data[temp +1] = pos.y;
		particle_position_data[temp +2] = pos.z;
		particleCount++;
	}
	//clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
}

void Partical_System::updateSnow(float dt){
	
	//constantly apply the position of the individual particals to the position data array
	for(unsigned int i = 0; i< maxParticles ; i++){
		float r = util::randF(0.0f,0.5f);
		float speed = dt * r;

		//do the calculation once rather than 3 times.
		int index = 3 * i;
		particle_position_data[index] = parts[i].position.x;
		particle_position_data[index + 1] = parts[i].position.y;
		particle_position_data[index + 2] = parts[i].position.z;
		//applys velocity to the position, also keeps them within a certain area,
		//rather than doing a life span, deleting/creating
		parts[i].position.x += parts[i].velocity.x * speed;
		parts[i].position.y -= parts[i].velocity.y * speed;
		parts[i].position.z += parts[i].velocity.z * speed;
		if(parts[i].position.x <= -area_size){
			parts[i].position.x = area_size - 1;
		}
		if( parts[i].position.x >= area_size){
			parts[i].position.x = -area_size + 1;
		}
		if(parts[i].position.y < -area_size){
			parts[i].position.y *= -1;
		}
		if(parts[i].position.z <= -area_size){
			parts[i].position.z = area_size - 1;
		}								
		if( parts[i].position.z >= area_size){
			parts[i].position.z = -area_size + 1;
		}
	}

	//stream the position data to the shaders.
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat),NULL,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) *3, particle_position_data);	
}


void Partical_System::draw( glm::mat4 pM, glm::mat4 vM){
	///ATTEMPT
	//glm::mat4 temp;
	//float flakeRot = 0;
	//flakeRot += dt * 0.5f;
	//for(int i= 0; i<maxParticles; i++){
	//	
	//	while( flakeRot > (3.14159265358979323846 * 2.0) )
	//	{
	//		flakeRot -= (3.14159265358979323846 * 2.0);
	//	}
	//
	//	temp = glm::translate(glm::mat4(1.0f), parts[i].position );
	//	temp = glm::rotate(temp, flakeRot, glm::vec3(0,1,0) );
	//	flakesModMat.push_back(temp);
	//
	//}
	//use the VAO
	glUseProgram(_program);
	glBindVertexArray( _VAO );
	//send matrixes to the shader
	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(vM) );
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(pM) );
	
	//draw the instanced partical system
	glDrawArraysInstanced(GL_TRIANGLES, 0, out_Verts.size(), particleCount);
	//using the first buffer as the instance
	//and the second as the multiple instances
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	//clean up
	glBindVertexArray( 0 );
	glUseProgram( 0 );	
}

void Partical_System::createParticle(glm::vec4 position){
	//simple partical creation.
	partical temp;
	float r = util::randF(-5.0f,5.0f);
	temp.isAlive = true;
	temp.life = 10.0f;
	temp.position = glm::vec3(position.x, position.y, position.z);
	temp.velocity = glm::vec3(r,gravity,r);
	parts.push_back(temp);
}