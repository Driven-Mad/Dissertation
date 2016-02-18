#pragma once
#include <glm.hpp>
class LightningBranch
{
public:
	LightningBranch();
	LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, int chance);
	~LightningBranch(void);

	void Draw();

	glm::vec3 getBeginning(){return beginning;};
	glm::vec3 getEnd(){return end;};
	float getBranchLength(){return branchLength;};
	int getBranchChance(){return branchChance;};

	void setBeginning(glm::vec3 t_v){beginning = t_v;};
	void setEnd(glm::vec3 t_v){end = t_v;};
	void setBranchLength(float t_f){branchLength = t_f;};
	void setBranchChance(int t_i){branchChance = t_i;};

private:
	glm::vec3 beginning;
	glm::vec3 end;
	float branchLength;
	int branchChance;
};

