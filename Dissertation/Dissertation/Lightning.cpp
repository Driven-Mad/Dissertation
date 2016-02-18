#include "Lightning.h"


Lightning::Lightning(void){
	strikeLength = 0;
	startingPoint = glm::vec3(util::randF(-16.0f,16.0f),22.0f,util::randF(-16.0f,16.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	firstBranch = true;
	VAO = 0;
	program = 0;
	strikeBranchPercentage = 40;
	shaderModelMatLocation = shaderViewMatLocation = shaderProjMatLocation = 0;
	coreStrike = false;
	Strike();
	// Create the model
	initaliseVAO();
	// Create the shaders
	initaliseShaders();
	
}


Lightning::~Lightning(void){
}

void Lightning::Init(){
	m_vVerts.clear();
	lightningStrike.clear();
	strikeLength = 0;
	startingPoint = glm::vec3(util::randF(-16.0f,16.0f),22.0f,util::randF(-16.0f,16.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	firstBranch = true;
	strikeBranchPercentage = 40;
	coreStrike = false;
	Strike();
	// Create the model
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vVerts.size() * sizeof(glm::vec3), &m_vVerts[0] ,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	glEnableVertexAttribArray(0);
	glBindVertexArray( 0 );
}
void Lightning::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix){
		// Activate the shader program
	glUseProgram( program );

		// Activate the VAO
		glBindVertexArray( VAO );
			modelMatrix = glm::translate(glm::mat4(1.0f), position);
			//glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glFrontFace(GL_CCW);
			// Send matrices to the shader as uniforms like this:
			glUniformMatrix4fv(shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix) );
			glUniformMatrix4fv(shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix) );
			glUniformMatrix4fv(shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix) );

			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			glLineWidth(5.0f);
			glDrawArrays(GL_LINES, 0,numVerts);
		// Unbind VAO
		glBindVertexArray( 0 );
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}

void Lightning::initaliseVAO(){
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray(VAO );
		positionBuffer = 0;
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vVerts.size() * sizeof(glm::vec3), &m_vVerts[0] ,GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
		glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
}
void Lightning::initaliseShaders(){
	ProgramLoader *pgLoad = new ProgramLoader();
	//make a program for the specified frag and vertex shaders
	pgLoad->loadProgram("shaders/lightningVertexShader.txt", "shaders/lightningFragmentShader.txt");
	program = pgLoad->getProgram(); 
	//get all the locations in the program to link the application to the shaders.
	shaderModelMatLocation = glGetUniformLocation( program, "modelMat" );
	shaderViewMatLocation = glGetUniformLocation( program, "viewMat" );
	shaderProjMatLocation = glGetUniformLocation( program, "projMat" );
}

void Lightning::Strike(){
	int branches = 0;
	while(strikeLength < 22.0f){
		if(!firstBranch){
			glm::vec3 begin = lightningStrike[predessingIndex].getEnd();
			newBranch(begin,strikeBranchPercentage);
			predessingIndex +=1;
		}else{
			newBranch(startingPoint,strikeBranchPercentage);
			firstBranch = false;
			predessingIndex = 0;
		}
	}
	coreStrike = true;
	while(strikeBranchPercentage != 0){
		branchOff();
	}
	numVerts = m_vVerts.size();
	printf("\n No of branches would be: %i", branches);
}
void Lightning::newBranch(glm::vec3 begin, int percentChance){
	
	float len = BranchLength();
	glm::vec3 en = begin + glm::vec3(0,len,0);
	float Angle = util::randF(90.0f,270.0f);
	float randVal = rand() % 2;
	if(randVal == 0){
		en = util::rotateZaxisByPoint(begin,Angle,en);
	}else{
		en = util::rotateXaxisByPoint(begin,Angle,en);
	}
	if(!coreStrike){
		strikeLength += (begin.y - en.y);
	}
	int branchC = BranchChance(percentChance);
	LightningBranch *t_bBranch = new LightningBranch(begin,en,len,branchC);
	lightningStrike.push_back(*t_bBranch);
	m_vVerts.push_back(begin);
	m_vVerts.push_back(en);
}
void Lightning::branchOff(){
	strikeBranchPercentage -= 2;
	for(int i= 0; i<lightningStrike.size(); i++){
		if(lightningStrike[i].getBranchChance() == 1){
			float randVal = rand() % 2;
			if(randVal == 0){
				newBranch(lightningStrike[i].getEnd(),strikeBranchPercentage);
			}else{
				newBranch(lightningStrike[i].getBeginning(),strikeBranchPercentage);
			}
			lightningStrike[i].setBranchChance(0);
		}
	}
}
int Lightning::BranchChance(int branchPercentageChance){
	int branchChance;
	int t_iRandom = rand() % 100;
	if(t_iRandom <= branchPercentageChance){
		branchChance = 1;
	}else{
		branchChance = 0;
	}
	return branchChance;
}

float Lightning::BranchLength(){
	float branchLength;
	branchLength = util::randF(0.3f,1.5f);
	return branchLength;
}

void Lightning::LightningInput(float DT,SDL_Event &incomingEvent){
	switch(incomingEvent.type){
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym ){
			case SDLK_l:
				Init();
				break;
		}
		break;
}
}