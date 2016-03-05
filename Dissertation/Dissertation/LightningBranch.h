#pragma once
#include <glm.hpp>
class LightningBranch
{
public:
	LightningBranch();
	LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, bool chance);
	~LightningBranch(void);

	void Draw();

	glm::vec3 getBeginning(){return beginning;};
	glm::vec3 getEnd(){return end;};
	float getBranchLength(){return branchLength;};
	bool getBranchChance(){return branchChance;};
	bool getIsBranchOff(){return isBranchOff;};
	int getAmountOfBranches(){return branchOffAmountOfBranches;};

	void setBeginning(glm::vec3 t_v){beginning = t_v;};
	void setEnd(glm::vec3 t_v){end = t_v;};
	void setBranchLength(float t_f){branchLength = t_f;};
	void setBranchChance(bool t_i){branchChance = t_i;};
	void setIsBranchOff(bool t_i){isBranchOff = t_i;};
	void setAmountOfBranches(int t_i){branchOffAmountOfBranches = t_i;};
private:
	glm::vec3 beginning;
	glm::vec3 end;
	float branchLength;
	bool branchChance;
	bool isBranchOff;
	int branchOffAmountOfBranches;
};

