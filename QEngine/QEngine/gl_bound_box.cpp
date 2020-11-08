#include "gl_bound_box.h"


BoundBox::BoundBox(float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ)
	:min(minX, minY, minZ), max(maxX, maxY, maxZ) {

}

glm::vec3 BoundBox::Size() {
	return max - min;
}

glm::vec3 BoundBox::Center() {
	return (min + max)*0.5f;
}

void BoundBox::Expand(float x) {
	glm::vec3 dir = glm::normalize(max - min);
	max = max + dir * x;
	min = min - dir * x;
}

