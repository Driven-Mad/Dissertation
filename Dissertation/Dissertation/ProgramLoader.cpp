#include "ProgramLoader.h"

ProgramLoader::ProgramLoader(void){
	// The 'program' stores the shaders
	program = glCreateProgram();
}


ProgramLoader::~ProgramLoader(void){
}
void ProgramLoader::loadVShader(char *filepath){
	//reads in the file into a buffer
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	Vdata = buffer.str();
	if(Vdata.size() == NULL){
		std::ofstream newFile;
		newFile.open(filepath);
		newFile <<"#version 330 \n \n layout(location = 0) in vec2 vs_position; \n \n out vec2 UV; \n \n void main() \n {\n UV = (vs_position + 1.0) / 2.0;\n gl_Position = vec4(vs_position, 0.0, 1.0); \n }";
		newFile.close();
		printf("\n NEW FILE WAS CREATED: %s", filepath);
	}
	// Create the vertex shader
	vShader = glCreateShader( GL_VERTEX_SHADER );
	//find the size of the Vdata
	int length = Vdata.length();
	//get our shader
	const GLchar* vStr = Vdata.c_str();
	// Give GL the source for it
	glShaderSource( vShader, 1, &vStr, NULL );
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( CheckShaderCompiled( vShader ) ){
		printf("\n %s successfully compiled", filepath);
	}
	else{
		printf("\n %s failed to compiled", filepath);
		return;
	}
	// This links the shader to the program
	glAttachShader( program, vShader );
	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( program );
}

void ProgramLoader::loadFShader(char *filepath){
	//reads in the file into a buffer
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	Fdata = buffer.str();
	if(Fdata.size() == NULL){
		std::ofstream newFile;
		newFile.open(filepath);
		newFile <<"#version 330 \n \n in vec2 UV; \n \n uniform sampler2D input_texture; \n \n void main() \n {\n gl_FragColor = texture(input_texture, UV);\n }";
		newFile.close();
		printf("\n NEW FILE WAS CREATED: %s", filepath);
	}
	// Create the fragment shader
	fShader = glCreateShader( GL_FRAGMENT_SHADER );
	//find the size of the fdata
	int length = Fdata.length();
	//get our shader
	const GLchar* fStr = Fdata.c_str();
	// Give GL the source for it
	glShaderSource( fShader, 1, &fStr, NULL );
	//compile the shader
	glCompileShader( fShader );
	// Check it compiled and give useful output if it didn't work!
	if( CheckShaderCompiled( fShader ) )
	{
		printf("\n %s successfully compiled", filepath);
	}else{
		printf("\n %s failed to compiled", filepath);
		return;
	}
	glAttachShader( program, fShader );
	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( program );
}

bool ProgramLoader::CheckShaderCompiled( GLint shader ){
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ){
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;
		return false;
	}
	return true;
}

void ProgramLoader::loadProgram(char *vShaderFP, char *fShaderFP){
	//loads the vertex shader
	loadVShader(vShaderFP);
	//loads the fragent shader
	loadFShader(fShaderFP);
	// Check this worked
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked );
	if ( linked ){
		printf("\n Program successfully linked");		
	}
	else{
		GLsizei len;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );
		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( program, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;
		return;
	}
}

GLint ProgramLoader::getProgram(){
	//returns the program from storage
	return program;
}




