#include "object.h"

Object::Object() :Object(nullptr) {

};

Object::Object(MyMaterial* material) {
	this->material = material;
	toDestroy = false;
};

Object::~Object() {};

void Object::Update(float deltaTime) {


}

MyMaterial* Object::getMaterial() const {
	return material;
}
void Object::setMaterial(MyMaterial* mat) {
	this->material = mat;
}
void Object::Destroy() {
	toDestroy = true;
}

bool Object::isDestroy() const {
	return toDestroy;
}
