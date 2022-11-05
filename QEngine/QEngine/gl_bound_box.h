#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BoundBox {
public:
	glm::vec3 min;
	glm::vec3 max;
public:
	explicit BoundBox(float minX, float minY, float minZ,
		float maxX, float maxY, float maxZ);

	glm::vec3 Size();

	glm::vec3 Center();

	void Expand(float x);
};