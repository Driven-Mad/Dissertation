#ifndef UTIL_H
#define UTIL_H
//------------------------------------------------------------------
/// \file    Utilities.h
/// \author  Lloyd Phillips
/// \brief   This is the utilities class
///          THIS CLASS WAS CREATED IN ASSIGNMENT 2 OF PROGRAMMING FOR GRAPHICS AND GAMES. 
///          SLIGHTLY MODIFIED FOR IMPOROVED FUNCTIONALITY.
//------------------------------------------------------------------
#include<iostream>
#include<vector>
#include <glm.hpp>
#include <SDL.h>

///Namespace util has multiple random functions to use.
namespace util{
	/// \brief creates tangents, taking in a normal, and applying the tangent directly the the specified container.
	void createTangents(glm::vec3 normal, glm::vec3 &tangent);
	/// \brief creates bitangents, taking in a normal and a tangent, and applying the bitangent directly the the specified container.
	void createBiTangents(glm::vec3 normal, glm::vec3 tangent, glm::vec3 &biTangent);
	/// \brief finds the magnitude of a vec3, 
	/// \returns float
	float findMagnitude(glm::vec3 x);
	/// \brief normalises a vec3, 
	/// \returns vec3
	glm::vec3 normalize(glm::vec3 x);
	/// \brief finds the cross product of 2 vec3's, 
	/// \returns vec3
	glm::vec3 crossProduct(glm::vec3 a, glm::vec3 b);
	/// \brief finds the dot product of 2 vec3's, 
	/// \returns float
	float dotProduct(glm::vec3 a, glm::vec3 b);

	glm::vec3 rotateZaxisByPoint(glm::vec3 origin, float Angle, glm::vec3 rotatePoint);
	glm::vec3 rotateXaxisByPoint(glm::vec3 origin, float Angle, glm::vec3 rotatePoint);
	float randF(float lowerBound, float higherBound);
}
#endif ///!UTIL_H