#include "Ball.h"

Ball::Ball(glm::vec3 position, float radius, Color color) :
	Pocket (position, radius)
{
	this->color = color;
	velocity = glm::vec3 (0);
}

glm::vec3 Ball::getRenderColor() const {
	switch (color)
	{
		case WHITE: return glm::vec3 (1, 1, 1);
		case YELLOW: return glm::vec3 (1, 1, 0);
		case RED: return glm::vec3 (1, 0, 0);
		case BLACK: return glm::vec3 (0, 0, 0);
	}
}
