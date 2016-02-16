

#include "Utilities.h"


namespace util{
	void createTangents(glm::vec3 normal, glm::vec3 &tangent){
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		//do the cross product between a normal and forward vector
		glm::vec3 tang1 = util::crossProduct(normal, forward);
		//do the cross product between a normal and up vector
		glm::vec3 tang2 = util::crossProduct(normal, up);
		//find the magnitude of them both
		float tang1mag = util::findMagnitude(tang1);
		float tang2mag = util::findMagnitude(tang2);
		//if the forward magnitude is greater than the up magnitude, the tangent is = tangforward
		//else we use the up tangent.
		if (tang1mag > tang2mag){
			tangent = tang1;
		}
		else{
			tangent = tang2;
		}
	}
	glm::vec3 rotateZaxisByPoint(glm::vec3 origin, float Angle, glm::vec3 rotatePoint){
		glm::vec3 t_vEnd;
		float t_fTranslatedX = rotatePoint.x - origin.x;
		float t_fTranslatedY = rotatePoint.y - origin.y;
		t_vEnd.x =((t_fTranslatedX * cosf(Angle)) - (t_fTranslatedY * sinf(Angle))) + origin.x;
		t_vEnd.y = ((t_fTranslatedX * sinf(Angle)) + (t_fTranslatedY * cosf(Angle))) + origin.y;
		t_vEnd.z = rotatePoint.z;
		return t_vEnd;
	}

	float randF(float lowerBound, float higherBound){
		float t = lowerBound + static_cast <float> (rand())/(static_cast <float> (RAND_MAX/(higherBound-lowerBound)));
		return t;
	}

	void createBiTangents(glm::vec3 normal, glm::vec3 tangent, glm::vec3 &biTangent){
		//normalise both the normal and tangent, do the cross product 
		//of the solutions and that gives you the bi-tangent
		glm::vec3 UN = util::normalize(normal);
		glm::vec3 UT = util::normalize(tangent);
		biTangent = crossProduct(UN, UT);
	}

	
	float findMagnitude(glm::vec3 x){
		//magnitude = sqrt of the dotproduct of two vectors.
		return sqrt(util::dotProduct(x,x));
	}

	glm::vec3 crossProduct(glm::vec3 a, glm::vec3 b){
		/* we have:
		  | I J K |
		V1| a b c | (or V1 X, Y, Z)
		V2| d e f | (or V2 X, Y, Z)

		|b c|I - |a c|J + |a b|K
		|e f|    |d f|	  |d e|

		= (bf - ce) I + (cd-af)J + (ae -bd) K
		*/
		float I, J, K;
		I = (a.y * b.z) - (a.z * b.y);
		J = (a.z * b.x) - (a.x * b.z);
		K = (a.x * b.y) - (a.y * b.x);
		return glm::vec3(I, J, K);
	}
	//Normalises a vec3
	glm::vec3 normalize(glm::vec3 x){
		float m = util::findMagnitude(x);
		if (abs(m) != 0){
			return glm::vec3(abs(x.x / m), abs(x.y / m), abs(x.z / m));
		}
		else{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	float dotProduct(glm::vec3 a, glm::vec3 b){
		//self explanitry 
		return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
	}
}
