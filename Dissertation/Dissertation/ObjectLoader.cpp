#include "ObjectLoader.h"


ObjectLoader::ObjectLoader(void){
}


ObjectLoader::~ObjectLoader(void){
}

bool ObjectLoader::loadOBJ(char * filepath, std::vector<glm::vec3> &outVerts,
	std::vector<glm::vec2> &outUVs,
	std::vector<glm::vec3> &outNormals,
	std::vector<glm::vec3> &outTangents, 
	std::vector<glm::vec3> &outBiTangents){
	
	//create temp containers as they have to be organised.
	std::vector<glm::vec3> tempVerts;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	//set a file to read the file only.
	FILE * file = fopen(filepath, "r");
	if(file != NULL){
		printf("\n file %s read successfully", filepath);
	}else{
		printf("\n Couldn't load file %s, I do apolagise", filepath);
		return false;
	}
	while(1){
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF){
			break;
		}
		//checks for the start of the line to be a V = Vertex
		if( strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n",&vertex.x, &vertex.y, &vertex.z);
			tempVerts.push_back(vertex);
		}
		//checks for the start of the line to be a VT = UV
		else if (strcmp(lineHeader,"vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f \n", &uv.x , & uv.y);
			tempUVs.push_back(uv);
		}
		//checks for the start of the line to be a VN = Vertex Normal
		else if(strcmp(lineHeader, "vn")==0){
			glm::vec3 norm;
			fscanf(file, " %f %f %f \n", &norm.x, &norm.y, &norm.z);
			tempNormals.push_back(norm);
		}
		//Checks if the start of the line is an F which is indicies.
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read as too many options for this loader\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	//sorts through our temp containers, and pushes out to the "used" containers.
	for(unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 vertex = tempVerts[vertexIndex - 1];
			//For some reason the UVs have rotated
			glm::vec2 uv = tempUVs[uvIndex - 1];
			//this fixed everything.
			uv.y = 1.0f - uv.y;
			glm::vec3 normal = tempNormals[normalIndex- 1];

			outVerts.push_back(vertex);
			outUVs.push_back(uv);
			outNormals.push_back(normal);
		}
	//Creates the tangents and bitangents.
	for (unsigned int i = 0; i < outNormals.size(); i++){
		glm::vec3 temp;
		util::createTangents(outNormals[i], temp);
		outTangents.push_back(temp);
		util::createBiTangents(outNormals[i], outTangents[i], temp);
		outBiTangents.push_back(temp);
	}
		return true;
}