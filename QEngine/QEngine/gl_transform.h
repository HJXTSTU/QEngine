#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
class Transform {
protected:
	Transform *parent;
	std::vector<Transform*> children;
	glm::vec3 position;
	glm::mat4 rotation;
	glm::vec3 scale;
	glm::mat4 matrixWorld;
	bool Static;
public:
	void SetStatic(bool isStatic);

	int GetChildCount();

	Transform *GetChild(int index);

	Transform();

	void Rotate(glm::mat4 rot);

	void Move(glm::vec3 off);

	void Scale(glm::vec3 sc, bool childrenFlow = true);

	void SetLocalRotation(const glm::mat4 &rot);

	void SetLocalPosition(const glm::vec3 &pos);

	void SetWorldRotation(glm::mat4 rot);

	glm::mat4 GetWorldRotation();

	glm::mat4 MatrixWorld();

	void ApplyMatrix(glm::mat4 mat);

	void Remove(Transform *child);

	void Add(Transform *child);

	void UpdateMatrixWorld(bool updateChildren);


	glm::vec3 GetWorldPosition();

	void SetWorldPosition(glm::vec3);
};