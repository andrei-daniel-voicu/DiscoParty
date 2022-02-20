#pragma once
#include "glm/glm.hpp"

class Transform {

public:
	Transform() : position{ 0,0,0 }, rotation{ 0,0,0 }, scale{ 1,1,1 }{};
	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc) :position(pos), rotation(rot), scale(sc) {};
	~Transform();

private:
	glm::vec3 position, rotation, scale;

public:
	glm::vec3 getPosition() const;
	virtual void setPosition(const glm::vec3&);
	virtual void setPosition(float, float, float);

	glm::vec3 getRotation() const;
	virtual void setRotation(const glm::vec3&);
	virtual void setRotation(float, float, float);

	glm::vec3 getScale() const;
	virtual void setScale(const glm::vec3&);
	virtual void setScale(float, float, float);
};