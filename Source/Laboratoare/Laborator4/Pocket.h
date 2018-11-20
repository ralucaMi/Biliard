#pragma once
#include <include/glm.h>


class Pocket {
public:
	Pocket (glm::vec3 position, float radius);

	glm::vec3				getPosition () const {return position;}
	float					getRadius () const {return radius;}

protected:
	glm::vec3				position;
	float					radius;
};

