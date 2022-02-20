#include "transform.h"

Transform::~Transform() {

}

glm::vec3 Transform::getPosition() const {
	return position;
}

void Transform::setPosition(const glm::vec3& pos) {
	position = pos;
}

void Transform::setPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

glm::vec3 Transform::getRotation() const {
	return rotation;
}

void Transform::setRotation(const glm::vec3& rot) {
	rotation = rot;
}
void Transform::setRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

glm::vec3 Transform::getScale() const {
	return scale;
}
void Transform::setScale(const glm::vec3& sca) {
	scale = sca;
}
void Transform::setScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
}