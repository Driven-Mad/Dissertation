#ifndef LIGHTNING_BRANCH_H
#define LIGHTNING_BRANCH_H
//------------------------------------------------------------------
/// \file    LightningBranch.h
/// \author  Created by Lloyd Phillips
/// \brief   This is the Lightning Branch Class which mainly acts as a storage for the branches. 
//------------------------------------------------------------------
#include <glm.hpp>
#include "Utilities.h"
class LightningBranch
{
public:
	///\brief Default constructor for the lightning branch. 
	LightningBranch();
	///\brief overloaded constructor for the lighning branch, takes in a beginning, end, length of branch and the chance of breaking off into a fork. 
	LightningBranch(glm::vec3 begin, glm::vec3 en, float branchLen, bool chance);
	///\brief default destructor. 
	~LightningBranch(void);

	//GETTERS
	///\breif Returns the beginning vec3 of the branch.
	glm::vec3 getBeginning(){return beginning;};
	///\breif Returns the end vec3 of the branch.
	glm::vec3 getEnd(){return end;};
	///\breif Returns the direction vec3 of the branch.
	glm::vec3 getDirection(){return direction;};
	///\breif Returns the branchLength float of the branch.
	float getBranchLength(){return branchLength;};
	///\breif Returns the branchChance bool of the branch.
	bool getBranchChance(){return branchChance;};
	///\breif Returns the isBranchOff bool of the branch.
	bool getIsBranchOff(){return isBranchOff;};
	///\breif Returns the branchOffAmountOfBranches int of the branch.
	int getAmountOfBranches(){return branchOffAmountOfBranches;};
	///\breif Returns the parentAngle float of the branch.
	float getParentAngle(){return parentAngle;};
	///\breif Returns the Angle float of the branch.
	float getAngle(){return Angle;};

	//SETTERS
	///\brief sets the beginning value of the branch (vec3)
	void setBeginning(glm::vec3 t_v){beginning = t_v;};
	///\brief sets the end value of the branch (vec3)
	void setEnd(glm::vec3 t_v){end = t_v;};
	///\brief sets the direction value of the branch (vec3)
	void setDirections(glm::vec3 t_v){direction = t_v;};
	///\brief sets the branchLength value of the branch (float)
	void setBranchLength(float t_f){branchLength = t_f;};
	///\brief sets the branchChance value of the branch (bool)
	void setBranchChance(bool t_i){branchChance = t_i;};
	///\brief sets the isBranchOff value of the branch (bool)
	void setIsBranchOff(bool t_i){isBranchOff = t_i;};
	///\brief sets the branchOffAmountOfBranches value of the branch (int)
	void setAmountOfBranches(int t_i){branchOffAmountOfBranches = t_i;};
	///\brief sets the parentAngle value of the branch (float)
	void setParentAngle(float t_f){parentAngle = t_f;};
	///\brief sets the Angle value of the branch (float)
	void setAngle(float t_f){Angle = t_f;};

private:
	glm::vec3 beginning; ///<Starting point of the branch
	glm::vec3 end;///<End point of the branch
	glm::vec3 direction; ///<storage for the direction
	float branchLength;///<Length of the branch
	bool branchChance; ///<chance of the branch breaking off
	bool isBranchOff; ///<Is it a branch off the core branch
	float Angle; ///<storage for the current angle of rotation. 
	float parentAngle; ///<storage for the parent angle, to continue the branches in the general direction it was going
	int branchOffAmountOfBranches; ///< if a branch off how many branches are we giving it. 
};
#endif //!LIGHTNING_BRANCH_H
