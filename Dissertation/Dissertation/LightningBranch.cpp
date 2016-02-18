#include "LightningBranch.h"

LightningBranch::LightningBranch()
{
}
LightningBranch::LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, int chance)
{
	beginning = begin;
	end = en;
	branchLength = branchLen;
	branchChance = chance;
}


LightningBranch::~LightningBranch(void)
{
}
