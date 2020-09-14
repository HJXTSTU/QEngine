#include "gl_transform.h"

Transform::Transform() {
	this->parent = NULL;
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::mat4(1);
	this->scale = glm::vec3(1, 1, 1);
	this->matrixWorld = glm::mat4(1);
}

int Transform::GetChildCount() {
	return this->children.size();
}

Transform *Transform::GetChild(int index) {
	return this->children[index];
}

void Transform::Rotate(glm::mat4 rot) {
	this->rotation *= rot;
}

void Transform::Move(glm::vec3 off) {
	this->position += off;
}

void Transform::Scale(glm::vec3 sc, bool childrenFlow)
{
	this->scale = sc;
	if (!childrenFlow) {
		for (int i = 0; i < this->children.size(); i++) {
			glm::vec3 newScale = this->children[i]->scale / sc;
			this->children[i]->Scale(newScale, childrenFlow);
		}
	}
}

void Transform::SetLocalRotation(const glm::mat4 &rot) {
	this->rotation = rot;
}

void Transform::SetLocalPosition(const glm::vec3 &pos) {
	this->position = pos;
}

void Transform::SetWorldRotation(glm::mat4 rot) {
	if (this->parent == NULL) {
		this->rotation = rot;
	}
	else {
		this->rotation = glm::inverse(this->parent->GetWorldRotation())*rot;
	}
}

glm::mat4 Transform::GetWorldRotation() {
	glm::mat4 res(1);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			res[i][j] = this->matrixWorld[i][j];
		}
	}
	return res;
}

glm::mat4 Transform::MatrixWorld() {
	return this->matrixWorld;
}


void Transform::ApplyMatrix(glm::mat4 mat) {
	glm::mat4 translate = glm::translate(glm::mat4(1), this->position);
	glm::mat4 matrix = translate * this->rotation;
	this->matrixWorld = mat * matrix;
}

void Transform::Remove(Transform *child) {
	std::vector<Transform*>::iterator it = this->children.begin();
	while (it != this->children.end()) {
		if ((*it) == child) {
			this->children.erase(it);
			child->position = child->parent->position + child->position;
			child->rotation = child->parent->rotation*child->rotation;
			child->UpdateMatrixWorld(false);
			child->parent = NULL;
			break;
		}
		it++;
	}
}

void Transform::Add(Transform *child) {
	if (child->parent != NULL) {
		child->parent->Remove(child);
	}
	child->parent = this;
	this->children.push_back(child);
	child->SetLocalPosition((child->GetWorldPosition() - this->GetWorldPosition()));
	glm::mat4 e = this->rotation*glm::inverse(this->rotation);
	child->SetLocalRotation(glm::inverse(this->GetWorldRotation())*child->rotation);
	child->scale /= this->scale;
	child->UpdateMatrixWorld(false);
}

void Transform::UpdateMatrixWorld(bool updateChildren) {
	if (this->parent == NULL) {
		glm::mat4 translate = glm::translate(glm::mat4(1), this->position);
		glm::mat4 scale_mat = glm::scale(glm::mat4(1), this->scale);
		glm::mat4 LocalMatrix = translate * this->rotation * scale_mat;
		this->matrixWorld = LocalMatrix;
	}
	else {
		glm::mat4 translate = glm::translate(glm::mat4(1), this->position + parent->GetWorldPosition());
		glm::mat4 scale_mat = glm::scale(glm::mat4(1), this->scale*this->parent->scale);
		glm::mat4 rot_mat = this->parent->GetWorldRotation()*this->rotation;
		this->matrixWorld = translate * rot_mat * scale_mat;
	}

	if (updateChildren) {
		std::vector<Transform*>::iterator it = this->children.begin();
		while (it != this->children.end()) {
			(*it)->UpdateMatrixWorld(updateChildren);
			it++;
		}
	}
}

glm::vec3 Transform::GetWorldPosition() {
	glm::vec3 pos;
	pos.x = this->matrixWorld[3][0];
	pos.y = this->matrixWorld[3][1];
	pos.z = this->matrixWorld[3][2];
	return pos;
}

void Transform::SetWorldPosition(glm::vec3 pos) {
	glm::vec3 parentPosition(0);
	if (this->parent != NULL) {
		parentPosition = this->parent->GetWorldPosition();
	}
	glm::vec3 localPos = pos - parentPosition;
	this->SetLocalPosition(localPos);
}