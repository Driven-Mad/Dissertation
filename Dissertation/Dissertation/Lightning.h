#pragma once
#include <vector>
#include "Utilities.h"
#include <SDL.h>
#include "glew.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
class Lightning
{
	struct LightningBranch{
		glm::vec3 beginning;
		glm::vec3 end;
		float branchLength;
		int branchChance;
	};
public:
	Lightning(void);
	~Lightning(void);
	void BranchChance(int &branchChance);
	void BranchLength(float &branchLength);
	void Strike();
	void Init();
	void Draw();
private:
	bool firstBranch;
	float strikeLength;
	glm::vec3 startingPoint;
	std::vector<LightningBranch> lightingStrike;
	int predessingIndex;
};

