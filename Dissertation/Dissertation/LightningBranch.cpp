#include "LightningBranch.h"
#include <iostream>

LightningBranch::LightningBranch()
{
}
LightningBranch::LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, bool chance)
{
	///Set all the default values. 
	beginning = begin;
	end = en;
	branchLength = branchLen;
	branchChance = chance;
	isBranchOff = false;
	branchOffAmountOfBranches = rand() % 5 + 1;
	direction = glm::normalize(en- begin );
}


LightningBranch::~LightningBranch(void)
{
}
