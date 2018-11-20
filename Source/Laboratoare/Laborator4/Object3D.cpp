#include "Object3D.h"

#include <Core/Engine.h>

Mesh* Object3D::CreateBox (std::string name, glm::vec3 min, glm::vec3 max, glm::vec3 xPosColor, glm::vec3 xNegColor, glm::vec3 yPosColor, glm::vec3 yNegColor, glm::vec3 zPosColor, glm::vec3 zNegColor)
{
	std::vector<VertexFormat> vertices =
	{
		// Face yPos
		VertexFormat(glm::vec3 (min.x, max.y, min.z), yPosColor, glm::vec3 (0, 1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, max.y, min.z), yPosColor, glm::vec3 (0, 1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, max.y, max.z), yPosColor, glm::vec3 (0, 1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, max.y, max.z), yPosColor, glm::vec3 (0, 1, 0), glm::vec2 (0, 0)),
		// Face yNeg
		VertexFormat(glm::vec3 (min.x, min.y, min.z), yNegColor, glm::vec3 (0, -1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, min.z), yNegColor, glm::vec3 (0, -1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, max.z), yNegColor, glm::vec3 (0, -1, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, min.y, max.z), yNegColor, glm::vec3 (0, -1, 0), glm::vec2 (0, 0)),
		// Face xNeg
		VertexFormat(glm::vec3 (min.x, max.y, max.z), xNegColor, glm::vec3 (-1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, min.y, max.z), xNegColor, glm::vec3 (-1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, min.y, min.z), xNegColor, glm::vec3 (-1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, max.y, min.z), xNegColor, glm::vec3 (-1, 0, 0), glm::vec2 (0, 0)),
		// Face xPos
		VertexFormat(glm::vec3 (max.x, max.y, max.z), xPosColor, glm::vec3 (1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, max.z), xPosColor, glm::vec3 (1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, min.z), xPosColor, glm::vec3 (1, 0, 0), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, max.y, min.z), xPosColor, glm::vec3 (1, 0, 0), glm::vec2 (0, 0)),
		// Face zNeg
		VertexFormat(glm::vec3 (min.x, max.y, min.z), zNegColor, glm::vec3 (0, 0, -1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, min.y, min.z), zNegColor, glm::vec3 (0, 0, -1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, min.z), zNegColor, glm::vec3 (0, 0, -1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, max.y, min.z), zNegColor, glm::vec3 (0, 0, -1), glm::vec2 (0, 0)),
		// Face zPos
		VertexFormat(glm::vec3 (min.x, max.y, max.z), zPosColor, glm::vec3 (0, 0, 1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (min.x, min.y, max.z), zPosColor, glm::vec3 (0, 0, 1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, min.y, max.z), zPosColor, glm::vec3 (0, 0, 1), glm::vec2 (0, 0)),
		VertexFormat(glm::vec3 (max.x, max.y, max.z), zPosColor, glm::vec3 (0, 0, 1), glm::vec2 (0, 0))
	};

	Mesh* box = new Mesh(name);
	std::vector<unsigned short> indices = { 
		// Face yPos
		0, 1, 3, 1, 2, 3, 
		// Face yNeg
		4 + 0, 4 + 1, 4 + 3, 4 + 1, 4 + 2, 4 + 3, 
		// Face xNeg
		8 + 0, 8 + 1, 8 + 3, 8 + 1, 8 + 2, 8 + 3, 
		// Face xNeg
		12 + 0, 12 + 1, 12 + 3, 12 + 1, 12 + 2, 12 + 3, 
		// Face zNeg
		16 + 0, 16 + 1, 16 + 3, 16 + 1, 16 + 2, 16 + 3, 
		// Face zPos
		20 + 0, 20 + 1, 20 + 3, 20 + 1, 20 + 2, 20 + 3, 
	};

	box->InitFromData(vertices, indices);
	return box;
}