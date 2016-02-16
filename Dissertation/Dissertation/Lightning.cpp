#include "Lightning.h"


Lightning::Lightning(void){
	strikeLength = 0;
	startingPoint = glm::vec3(util::randF(-16.0f,16.0f),22.0f,util::randF(-16.0f,16.0f));
	firstBranch = true;
}


Lightning::~Lightning(void){
}

void Lightning::Strike(){
	while(strikeLength < 22){
		if(!firstBranch){
			LightningBranch t_bBranch;
			t_bBranch.beginning = lightingStrike[predessingIndex].end;
			BranchLength(t_bBranch.branchLength);
			t_bBranch.end = t_bBranch.beginning + glm::vec3(0,t_bBranch.branchLength,0);
			float Angle = util::randF(135.0f,225.0f);
			t_bBranch.end = util::rotateZaxisByPoint(t_bBranch.beginning,Angle,t_bBranch.end);
			strikeLength = strikeLength + (t_bBranch.beginning.y - t_bBranch.end.y);
			lightingStrike.push_back(t_bBranch);
			predessingIndex +=1;
		}else{
			LightningBranch t_bBranch;
			t_bBranch.beginning = startingPoint;
			BranchLength(t_bBranch.branchLength);
			t_bBranch.end = t_bBranch.beginning + glm::vec3(0,t_bBranch.branchLength,0);
			float Angle = util::randF(135.0f,225.0f);
			t_bBranch.end = util::rotateZaxisByPoint(t_bBranch.beginning,Angle,t_bBranch.end);
			strikeLength = strikeLength + (t_bBranch.beginning.y - t_bBranch.end.y);
			lightingStrike.push_back(t_bBranch);
			firstBranch = false;
			predessingIndex = 0;
		}
	}
}


void Lightning::BranchChance(int &branchChance){
	int t_iRandom = rand() % 100;
	if(t_iRandom <= 20){
		branchChance = 1;
	}else{
		branchChance = 0;
	}
}

void Lightning::BranchLength(float &branchLength){
	branchLength = util::randF(0.3f,4.0f);
}

