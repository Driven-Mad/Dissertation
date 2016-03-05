#include "LightningBranch.h"
#include <iostream>

LightningBranch::LightningBranch()
{
}
LightningBranch::LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, bool chance)
{
	beginning = begin;
	end = en;
	branchLength = branchLen;
	branchChance = chance;
	isBranchOff = false;
	branchOffAmountOfBranches = rand() % 5 + 1;
}


LightningBranch::~LightningBranch(void)
{
}
