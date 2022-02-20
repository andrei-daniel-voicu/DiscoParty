#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
	struct Material {
		glm::vec3 materialKa;
		glm::vec3 materialKd;
		glm::vec3 materialKe;
		glm::vec3 materialKs;
		unsigned int materialShininess;

		Material(const glm::vec3& materialKa, const glm::vec3& materialKd, const glm::vec3& materialKe, const glm::vec3& materialKs, unsigned int materialShininess) {
			this->materialKa = materialKa;
			this->materialKd = materialKd;
			this->materialKe = materialKe;
			this->materialKs = materialKs;
			this->materialShininess = materialShininess;
		}
	};

	class Lab8 : public gfxc::SimpleScene {
	public:
		Lab8();
		~Lab8();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		//void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, m1::Material&);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		glm::vec3 lightPosition;
		glm::vec3 lightDirection;

		glm::vec3 sourceIntensity;
		glm::vec3 lightColor;
		glm::vec3 globalAmbientalIntensity;

		// TODO(student): If you need any other class variables, define them here.
		int lightType;
		float cutOffAngle;
		float angleOx, angleOy;
	};
}   // namespace m1
