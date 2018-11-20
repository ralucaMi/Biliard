#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object3D
{
	Mesh* CreateBox (std::string name, glm::vec3 min, glm::vec3 max, glm::vec3 xPosColor, glm::vec3 xNegColor, glm::vec3 yPosColor, glm::vec3 yNegColor, glm::vec3 zPosColor, glm::vec3 zNegColor);
}

