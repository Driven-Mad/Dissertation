#include "Lightning.h"


Lightning::Lightning(void){
	//Random point between 16, 22, 16 && -16, 22, -16
	startingPoint = glm::vec3(util::randF(-16.0f,16.0f),22.0f,util::randF(-16.0f,16.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	//is this the first branch of lightning
	firstBranch = true;
	VAOcore, VAObranch = 0;
	lightningCounter = -1;
	program = 0;
	strikeBranchPercentage = 20;
	shaderModelMatLocation = shaderViewMatLocation = shaderProjMatLocation = 0;
	hitGround = false;
	finishedBranches = false;
	Strike();
	// Create the model
	initaliseVAO();
	// Create the shaders
	initaliseShaders();
	
}


Lightning::~Lightning(void){
}

void Lightning::Init(){
	m_vCoreVerts.clear();
	m_vBranchedVerts.clear();
	lightningStrike.clear();
	//* Random point between 16, 22, 16 && -16, 22, -16
	startingPoint = glm::vec3(util::randF(-16.0f,16.0f),22.0f,util::randF(-16.0f,16.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	//is this the first branch of lightning
	firstBranch = true;
	lightningCounter = -1;
	strikeBranchPercentage = 20;
	hitGround = false;
	finishedBranches = false;
	Strike();


	glBindBuffer(GL_ARRAY_BUFFER, corePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vCoreVerts.size() * sizeof(glm::vec3), &m_vCoreVerts[0] ,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	glEnableVertexAttribArray(0);
	glBindVertexArray( 0 );

	glBindBuffer(GL_ARRAY_BUFFER, branchPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vBranchedVerts.size() * sizeof(glm::vec3), &m_vBranchedVerts[0] ,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	glEnableVertexAttribArray(0);
	glBindVertexArray( 0 );
}
void Lightning::DrawCore(glm::mat4 viewMatrix, glm::mat4 projMatrix){
		// Activate the shader program
	glUseProgram( program );

		// Activate the VAO
		glBindVertexArray( VAOcore );
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
			//specifying the line width
			glLineWidth(5.0f);
			glDrawArrays(GL_LINES, 0,numCoreVerts);
		// Unbind VAO
		glBindVertexArray( 0 );
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}
void Lightning::DrawBranch(glm::mat4 viewMatrix, glm::mat4 projMatrix){
		// Activate the shader program
	glUseProgram( program );

		// Activate the VAO
		glBindVertexArray( VAObranch );
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
			glLineWidth(2.0f);
			glDrawArrays(GL_LINES, 0,numBranchVerts); 
		// Unbind VAO
		glBindVertexArray( 0 );
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}
void Lightning::initaliseVAO(){
	glGenVertexArrays( 1, &VAOcore );
	glBindVertexArray(VAOcore);
		corePositionBuffer = 0;
		glGenBuffers(1, &corePositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, corePositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vCoreVerts.size() * sizeof(glm::vec3), &m_vCoreVerts[0] ,GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
		glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

	glGenVertexArrays( 1, &VAObranch );
	glBindVertexArray(VAObranch);
		branchPositionBuffer = 0;
		glGenBuffers(1, &branchPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, branchPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vBranchedVerts.size() * sizeof(glm::vec3), &m_vBranchedVerts[0] ,GL_DYNAMIC_DRAW);
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

	while(!hitGround){
		if(!firstBranch){
			glm::vec3 begin = lightningStrike[predessingIndex].getEnd();
			newBranch(begin,strikeBranchPercentage, false,0);
			
		}else{
			newBranch(startingPoint,strikeBranchPercentage , false,0);
			firstBranch = false;
			predessingIndex = 0;
		}
		branchOff();
	}
	while(!finishedBranches){
		finishOffBranches();
	}
	FindCoreBranch();
	numCoreVerts = m_vCoreVerts.size();
	numBranchVerts = m_vBranchedVerts.size();
	printf("\n No of verts would be: %i", numCoreVerts);
}

void Lightning::FindCoreBranch(){
	m_vCoreVerts.clear();
	for(int i = lightningStrike.size(); i>= 0; i--)
	{
		if(lightningStrike[i].getIsBranchOff() == false)
		{
			m_vCoreVerts.push_back(lightningStrike[i].getBeginning());
			m_vCoreVerts.push_back(lightningStrike[i].getEnd());
			lightningStrike.erase(lightningStrike.begin() + i);
		}else{
			m_vBranchedVerts.push_back(lightningStrike[i].getBeginning());
			m_vBranchedVerts.push_back(lightningStrike[i].getEnd());
		}
	}
	lightningStrike.clear();

}
void Lightning::newBranch(glm::vec3 begin, int percentChance, bool isBranchOff,int amountOfBranches)
{
	float len = BranchLength();
	glm::vec3 en = begin + glm::vec3(0,len,0);
	float Angle = util::randF(90.0f,180.0f);
	int randVal = rand() % 2;
	if(randVal == 0)
	{
		en = util::rotateZaxisByPoint(begin,Angle,en);
	} else
	{
		en = util::rotateXaxisByPoint(begin,Angle,en);
	}
	bool branchC = BranchChance(percentChance);
	LightningBranch *t_bBranch = new LightningBranch(begin,en,len,branchC);
	t_bBranch->setIsBranchOff(isBranchOff);
	t_bBranch->setAmountOfBranches(amountOfBranches);
	
	lightningStrike.push_back(*t_bBranch);
	if(!isBranchOff)
	{
		predessingIndex = lightningStrike.size() -1;
	}
	if(en.y <= -2.0f)
	{
		lightningCounter = lightningStrike.size() -1;
		hitGround = true;
	}
}
void Lightning::branchOff(){
	for(unsigned int i= 0; i<lightningStrike.size(); i++){
		if(lightningStrike[i].getBranchChance() == true){
			printf("\n lightning branches %i ", lightningStrike[i].getAmountOfBranches());
			//if(lightningStrike[i].getAmountOfBranches() != 0){
				int randVal = rand() % 2;
				int temp = rand() % 5 + 5;
				if(randVal == 0)
				{
					newBranch(lightningStrike[i].getEnd(),strikeBranchPercentage, true,temp);
				}else
				{
					newBranch(lightningStrike[i].getBeginning(),strikeBranchPercentage,true,temp);
				}
				lightningStrike[i].setBranchChance(false);
		}
		
	}
}

void Lightning::finishOffBranches()
{
	for(unsigned int i= 0; i<lightningStrike.size(); i++)
	{
		if(lightningStrike[i].getAmountOfBranches() == 0){
			finishedBranches = true;
		}
		if(lightningStrike[i].getIsBranchOff() == true && lightningStrike[i].getAmountOfBranches() > 0)
		{
			newBranch(lightningStrike[i].getEnd(),strikeBranchPercentage, true, lightningStrike[i].getAmountOfBranches() - 1);
			lightningStrike[i].setAmountOfBranches(0);
			finishedBranches = false;
			
		}
	}
}
bool Lightning::BranchChance(int branchPercentageChance){
	bool branchChance;
	int t_iRandom = rand() % 100;
	if(t_iRandom <= branchPercentageChance){
		branchChance = true;
	}else{
		branchChance = false;
	}
	return branchChance;
}

float Lightning::BranchLength(){
	float branchLength;
	branchLength = util::randF(0.3f,1.0f);
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