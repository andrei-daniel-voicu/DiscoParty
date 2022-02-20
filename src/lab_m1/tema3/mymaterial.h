#pragma once
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "components/simple_scene.h"

class MyMaterial {

public:
	MyMaterial(Shader* shader, Mesh* mesh, const glm::vec3& materialKd, const glm::vec3& materialKe, const glm::vec3& materialKs, unsigned int materialShininess);
	~MyMaterial();

	glm::vec3 materialKd;
	glm::vec3 materialKe;
	glm::vec3 materialKs;
	unsigned int materialShininess;
	Shader* shader;
	Mesh* mesh;
	Texture2D* texture;
	Texture2D* texture_2;
};