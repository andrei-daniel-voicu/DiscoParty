#include "mymaterial.h"


MyMaterial::MyMaterial(Shader* shader, Mesh* mesh, const glm::vec3& materialKd, const glm::vec3& materialKe, const glm::vec3& materialKs, unsigned int materialShininess) {
	this->materialKd = materialKd;
	this->materialKe = materialKe;
	this->materialKs = materialKs;
	this->materialShininess = materialShininess;

	this->shader = shader;
	this->mesh = mesh;

	this->texture = nullptr;
	this->texture_2 = nullptr;
}

MyMaterial::~MyMaterial() {
}
