#include "TextureLoader.h"

TextureLoader::TextureLoader(void){
}


TextureLoader::~TextureLoader(void){
}

GLuint TextureLoader::loadTEXT(char * filepath){
	//Load in an image like you would using SDL
	SDL_Surface *img;
	GLuint texture = 0;
	img = IMG_Load(filepath);
	//error checking/confirmation.
	if(img != NULL){
		printf("\n Texture %s successfully read", filepath);
	}
	else{
		printf(" \n Couldn't load file, I do apolagise");

	}
	//generate texture, bind it, add data, add any parameters which I have for texture wrapping, rebind.
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h,0, GL_BGR, GL_UNSIGNED_BYTE,img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	//confirmation
	printf("\n Texture %s has been loaded into slot NUM: %i",filepath, textures.size() );
	//Clean up
	SDL_FreeSurface(img);
	//add to our container for textures
	textures.push_back(texture);

	return texture;
}

void TextureLoader::loadTexture(char *fileName, GLint Program){
	glUseProgram(Program);
	int size = textures.size();
	std::string s2 = static_cast<std::ostringstream*>( &(std::ostringstream() << size) )->str();
	//Assign the texture
	glActiveTexture(GL_TEXTURE0 +size);
	loadTEXT(fileName);
	glEnable(GL_TEXTURE_2D);

	//Get location.
	std::string s1 = "textSampler";
	std::string s3 = s1 + s2;
	const char * f = s3.c_str();
	
	GLint tL  = glGetUniformLocation( Program, f);
	texturesLocation.push_back(tL);

	//Bind it
	glBindTexture(GL_TEXTURE_2D, getTextures()[size]);
	glUniform1i(texturesLocation[size], size);
}

std::vector<GLuint> TextureLoader::getTextures(){
	return textures;
}

void TextureLoader::enableTextures(){
	for(unsigned int i = 0; i< textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 +i);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, getTextures()[i]);
		glUniform1i(texturesLocation[i], i);
	}
}