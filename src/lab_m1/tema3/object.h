#pragma once
#include "transform.h";
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "components/simple_scene.h"
#include "mymaterial.h"
#include "light.h"

class Object : public Transform {

public:
	Object();
	Object(MyMaterial* material);
	Object(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc, MyMaterial* mat) : Transform(pos, rot, sc) {
		toDestroy = false;
		this->material = mat;
	}
	~Object();

	virtual void Update(float);
	void Destroy();
	bool isDestroy() const;
	MyMaterial* getMaterial() const;
	void setMaterial(MyMaterial* mat);
private:
	bool toDestroy;
	MyMaterial* material;
};