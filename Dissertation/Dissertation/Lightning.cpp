#include "Lightning.h"


Lightning::Lightning(void)
{
	//Random point between 16, 22, 16 && -16, 22, -16
	startingPoint = glm::vec3(util::randF(-26.0f,26.0f),30.0f,util::randF(-26.0f,26.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	//is this the first branch of lightning
	firstBranch = true;
	//set all default values
	VAOcore, VAObranch = 0;
	program = 0;
	predessingIndex = 0;
	strikeBranchPercentage = 30;
	branchedStart = -1;
	shaderModelMatLocation = shaderViewMatLocation = shaderProjMatLocation = 0;
	hitGround = false;
	drawBranch = false;
	finishedBranches = false;
	Strike();
	coreToDraw = m_vCoreVerts.size();
	branchesToDraw = m_vBranchedVerts.size();
	// Create the model
	initaliseVAO();
	// Create the shaders
	initaliseShaders();
	
}


Lightning::~Lightning(void)
{
}

void Lightning::Init()
{
	m_vCoreVerts.clear();
	m_vBranchedVerts.clear();
	lightningStrike.clear();
	branchedStart = -1;
	predessingIndex = 0;
	//* Random point between 16, 22, 16 && -16, 22, -16
	startingPoint = glm::vec3(util::randF(-26.0f,26.0f),30.0f,util::randF(-26.0f,26.0f));
	position = glm::vec3(0.0f,0.0f,0.0f);
	//is this the first branch of lightning
	firstBranch = true;
	strikeBranchPercentage = 30;
	drawBranch = false;
	hitGround = false;
	finishedBranches = false;
	Strike();
	coreToDraw = m_vCoreVerts.size();
	branchesToDraw = m_vBranchedVerts.size();
	//resend the data for positions. 
	//for (int i = m_vCoreVerts.size(); i >= 0; i--){
	//	for (int x = 0; x < 60; x++){
	//		if (x != 59){
	//			continue;
	//		};
	glBindBuffer(GL_ARRAY_BUFFER, corePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, (m_vCoreVerts.size()) * sizeof(glm::vec3), &m_vCoreVerts[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
			
	//	}
	//}

	glBindBuffer(GL_ARRAY_BUFFER, branchPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vBranchedVerts.size() * sizeof(glm::vec3), &m_vBranchedVerts[0] ,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
	glEnableVertexAttribArray(0);
	glBindVertexArray( 0 );
}

void Lightning::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	if (coreToDraw > 0)
	{
		if (coreToDraw < branchedStart)
		{
			drawBranch = true;
		}
		coreToDraw -= 4;
		if (coreToDraw < 0)
		{
			coreToDraw = 0;
		}
	}
	if (drawBranch)
	{
		if (branchesToDraw > 0)
		{
			branchesToDraw -= 4;
			if (branchesToDraw < 0)
			{
				branchesToDraw = 0;
			}
		}
	}
	DrawCore(viewMatrix,projMatrix);
	DrawBranch(viewMatrix,projMatrix);
}
void Lightning::DrawCore(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
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
			glEnable(GL_LINE_SMOOTH);
			// Tell OpenGL to draw it
			// Must specify the type of geometry to draw and the number of vertices
			//specifying the line width
			glLineWidth(6.0f);
			glDrawArrays(GL_LINES, 0, numCoreVerts - coreToDraw);
		// Unbind VAO
		glBindVertexArray( 0 );
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}
void Lightning::DrawBranch(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
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
			glDrawArrays(GL_LINES, 0, numBranchVerts - branchesToDraw);
		// Unbind VAO
		glBindVertexArray( 0 );
	// Technically we can do this, but it makes no real sense because we must always have a valid shader program to draw geometry
	glUseProgram( 0 );
}
void Lightning::initaliseVAO()
{
	//Initalise all the VAOS
	
	glGenVertexArrays( 1, &VAOcore );
	glBindVertexArray(VAOcore);
		corePositionBuffer = 0;
		glGenBuffers(1, &corePositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, corePositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, (m_vCoreVerts.size()) * sizeof(glm::vec3), &m_vCoreVerts[0] ,GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
		glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );

	glGenVertexArrays( 1, &VAObranch );
	glBindVertexArray(VAObranch);
		branchPositionBuffer = 0;
		glGenBuffers(1, &branchPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, branchPositionBuffer);
		glBufferData(GL_ARRAY_BUFFER, (m_vBranchedVerts.size()) * sizeof(glm::vec3), &m_vBranchedVerts[0] ,GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0 );
		glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray( 0 );
}
void Lightning::initaliseShaders()
{
	ProgramLoader *pgLoad = new ProgramLoader();
	//make a program for the specified frag and vertex shaders
	pgLoad->loadProgram("shaders/lightningVertexShader.txt", "shaders/lightningFragmentShader.txt");
	program = pgLoad->getProgram(); 
	//get all the locations in the program to link the application to the shaders.
	shaderModelMatLocation = glGetUniformLocation( program, "modelMat" );
	shaderViewMatLocation = glGetUniformLocation( program, "viewMat" );
	shaderProjMatLocation = glGetUniformLocation( program, "projMat" );
}

void Lightning::Strike()
{
	//While its not hit the ground
	while(!hitGround)
	{
		//If not first branch(to save on the else statment)
		if(!firstBranch)
		{
			//create branch from parent branch
			glm::vec3 begin = lightningStrike[predessingIndex].getEnd();
			newBranch(begin,strikeBranchPercentage, false, 0);
		}
		else
		{
			//Create a brand new branch.
			newBranch(startingPoint,strikeBranchPercentage , false, 0);
			predessingIndex = 0;
			firstBranch = false;
		}
		branchOff();
	}
	//Finish branches until the amount of branches it has gets to 0 
	while(!finishedBranches)
	{
		finishOffBranches();
		branchOff();
	}
	
	//Find the core branch to seperate from branch offs
	FindCoreBranch();
	/*!DEBUG ONLY!*/
	//for(int i = 0; i<m_vCoreVerts.size();i++){
	//	printf("\n CORE BRANCH %i = X:%f, Y:%f, Z%f", i, m_vCoreVerts[i].x, m_vCoreVerts[i].y, m_vCoreVerts[i].z);
	//}
	for (int i = 0; i < m_vCoreVerts.size(); i+=2)
	{
		if (m_vCoreVerts[i] == m_vBranchedVerts[0])
		{
			branchedStart = m_vCoreVerts.size() - i;
			break;
		}
	}

}

void Lightning::FindCoreBranch()
{
	//Make sure the arrays are cleared
	m_vCoreVerts.clear();
	m_vBranchedVerts.clear();
	for(int i = 0; i < lightningStrike.size(); i++)
	{
		//If it isn't a branch  off add to core branch else add to branchoff verts.
		if(lightningStrike[i].getIsBranchOff() == false)
		{
			m_vCoreVerts.push_back(lightningStrike[i].getBeginning());
			m_vCoreVerts.push_back(lightningStrike[i].getEnd());
		}
		else
		{
			m_vBranchedVerts.push_back(lightningStrike[i].getBeginning());
			m_vBranchedVerts.push_back(lightningStrike[i].getEnd());
		}
	}
	//clear out the array to save memory.
	lightningStrike.clear();
	//Set sizes for drawing. 
	numCoreVerts = m_vCoreVerts.size();
	numBranchVerts = m_vBranchedVerts.size();
}

void Lightning::newBranch(glm::vec3 begin, int percentChance, bool isBranchOff,int amountOfBranches)
{
	//get branch length, add it to the up value, then rotate it between 90 and 270 degrees in either x or Z axis.
	float len = BranchLength();
	glm::vec3 en = begin + glm::vec3(0,len,0);
	float Angle;
	int randVal = rand() % 2;
	if(!isBranchOff){
		Angle = util::randF(90.0f,270.0f);
		if(randVal == 0)
		{
			en = util::rotateZaxisByPoint(begin,Angle,en);
		}
		else
		{
			en = util::rotateXaxisByPoint(begin,Angle,en);
		}
	}
	else
	{
		//en += lightningStrike[lightningStrike.size()].getDirection();
		int randVal2 = rand() % 2;
		//Angle = util::randF(90.0f,270.0f);
		if(randVal2 == 1){
			Angle = util::randF(90.0f,140.0f);
		}else{
			Angle = util::randF(220.0f,270.0f);
		}
		if(randVal == 0)
		{
			en = util::rotateZaxisByPoint(begin,Angle,en);
		}
		else
		{
			en = util::rotateXaxisByPoint(begin,Angle,en);
		}
	}
	bool branchC = BranchChance(percentChance);

	//Add our new branch. 
	LightningBranch *t_bBranch = new LightningBranch(begin,en,len,branchC);
	t_bBranch->setIsBranchOff(isBranchOff);
	t_bBranch->setAmountOfBranches(amountOfBranches);
	t_bBranch->setAngle(Angle);
	if(!isBranchOff)
	{
		predessingIndex = lightningStrike.size();
	}/*else{
		t_bBranch->setEnd(t_bBranch->getEnd() + lightningStrike[lightningStrike.size()].getDirection());
	}*/
	lightningStrike.push_back(*t_bBranch);
	//if the end y is ground level then it's hit the ground. 
	if(en.y <= 0.0f)
	{
		hitGround = true;
	}
	
}
void Lightning::branchOff()
{
	//Check for any current branch offs, if so branch off them.
	for(unsigned int i= 0; i<lightningStrike.size(); i++)
	{
		if(lightningStrike[i].getBranchChance() == true)
		{
			int randVal = rand() % 2;
			int branchAmountSize = rand() % 5;
			if(randVal == 0)
			{
				newBranch(lightningStrike[i].getEnd(),strikeBranchPercentage, true,branchAmountSize);
			}else
			{
				newBranch(lightningStrike[i].getBeginning(),strikeBranchPercentage,true,branchAmountSize);
			}
			lightningStrike[i].setBranchChance(false);
		}
	}
}

void Lightning::finishOffBranches()
{
	//finish off the branches as if continously branch off, it can go forever. 
	for(unsigned int i= 0; i<lightningStrike.size(); i++)
	{
		if(lightningStrike[i].getAmountOfBranches() == 0){
			finishedBranches = true;
		}else if(lightningStrike[i].getIsBranchOff() == true )
		{
			newBranch(lightningStrike[i].getEnd(),strikeBranchPercentage, true, lightningStrike[i].getAmountOfBranches() - 1);
			lightningStrike[i].setAmountOfBranches(0);
			finishedBranches = false;
		}
	}
}
bool Lightning::BranchChance(int branchPercentageChance)
{
	//get a branch change based out of 100
	bool branchChance;
	int t_iRandom = rand() % 100 + 1;
	if(t_iRandom <= branchPercentageChance)
	{
		branchChance = true;
	}
	else
	{
		branchChance = false;
	}
	return branchChance;
}

float Lightning::BranchLength()
{
	//returns a random length
	float branchLength;
	branchLength = util::randF(0.5f,1.5f);
	return branchLength;
}

void Lightning::LightningInput(float DT,SDL_Event &incomingEvent)
{
	//Restart the current strike
	switch(incomingEvent.type)
	{
	case SDL_KEYDOWN:
		switch( incomingEvent.key.keysym.sym )
		{
			case SDLK_l:
				Init();
				break;
		}
		break;
	}
}

void  Lightning::get3MajorPoints(glm::vec4 &pointA, glm::vec4& pointB, glm::vec4 &pointC)
{
	//returns 3 major points in the array
	int mid = m_vCoreVerts.size()-1;
	int midBegin = mid -10;
	int midend = mid - 20;
	pointA = glm::vec4(m_vCoreVerts[midBegin].x,m_vCoreVerts[midBegin].y,m_vCoreVerts[midBegin].z,1.0f);
	pointB =  glm::vec4(m_vCoreVerts[mid].x,m_vCoreVerts[mid].y,m_vCoreVerts[mid].z,1.0f);
	pointC =  glm::vec4(m_vCoreVerts[midend].x,m_vCoreVerts[midend].y,m_vCoreVerts[midend].z,1.0f);
}