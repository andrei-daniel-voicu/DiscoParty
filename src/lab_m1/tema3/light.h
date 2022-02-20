#pragma once
#include "transform.h"

class Light {

public:
	Light(const glm::vec3& pos, const glm::vec3& col);

	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 newColor;
	glm::vec3 lightDirection;
	int lightType;
	float cutOffAngle;
	glm::vec3 intensity;
	int dirOx;
	int dirOz;

	float timeColorChange = 4.f;
	float timeSinceColorChanged = 0;

	float timeDirChange = 2.f;
	float timeSinceDirChanged = 0;
};