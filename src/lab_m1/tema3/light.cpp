#include "light.h"
#include "object.h"

Light::Light(const glm::vec3& pos, const glm::vec3& col) {
	this->pos = pos;
	this->color = col;
	this->lightType = 0;

	this->lightDirection = glm::vec3{ 0,0,0 };
	this->cutOffAngle = 0;
	this->dirOx = 0;
	this->dirOz = 0;
	this->newColor = color;
}