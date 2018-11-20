#pragma once

#include "Pocket.h"

class Ball : public Pocket {
public:
	enum Color {
		RED,
		YELLOW,
		BLACK,
		WHITE
	};

	Ball (glm::vec3 position, float radius, Color color);

	Color getColor () const {return color;}

	glm::vec3 getRenderColor () const ;

	glm::vec3 getVelocity () const {return velocity;}
	void setPosition (glm::vec3 pos) {position = pos;}
	void setVelocity (glm::vec3 vel) {velocity = vel;}

protected:
	Color					color;
	glm::vec3				velocity;
};