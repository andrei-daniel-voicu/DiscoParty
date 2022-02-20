#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "object.h"
#include "mymaterial.h"

namespace m1
{
	class Tema3 : public gfxc::SimpleScene {
	public:
		Tema3();
		~Tema3();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Object* obj, const glm::mat4& modelMatrix, int isReflector, int isGlobe);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		Mesh* GenerateCone(const std::string&, const glm::vec3&, float, int, float, bool);
		bool CheckConeCollision(const glm::vec3& pos, const glm::vec3& sc, int i);
		void ResetLights();

		void UpdateGroundLights(float);
		void UpdateReflectorLights(float);
		void UpdateGlobeLights(float);
		void UpdateReflectorRotation(float);
		void UpdateDancers(float);
		Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

		std::vector<Object> grid;
		std::vector<Object> walls;
		std::vector<Object> dancers;

		std::vector<glm::vec3> targets;
		float timeChangeTarget = 2.5f;
		float timePassedTargetChanged = 0.f;

		std::vector<Light> groundLights;

		std::vector<Object> reflectors;
		std::vector<Light> reflectorLights;

		Object globe;
		Light globeLight;
		Light globeShine;

		glm::vec3 lightPos[14];
		glm::vec3 lightColor[14];
		int lightType[14];
		glm::vec3 lightDir[14];
		float lightCutOff[14];
		glm::vec3 lightIntensity[14];

		bool showGlobe = true;
		bool showGround = true;
		bool showReflectors = true;

		std::unordered_map<std::string, Texture2D*> mapTextures;
	};
}   // namespace m1
