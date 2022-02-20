#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3() : globeLight(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), globeShine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)) {
}


Tema3::~Tema3() {
}


void Tema3::Init() {
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("couple");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters"), "couple.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("LightShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	glm::vec3 pos = glm::vec3{ 0,0,0 };
	glm::vec3 sc = glm::vec3(1, 0, 1);

	// generate ground/grid
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			glm::vec3 emmisive = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
			MyMaterial* mat = new MyMaterial(shaders["LightShader"], meshes["box"], glm::vec3(1.f, 1.f, 1.f), emmisive, glm::vec3(1.f, 1.f, 1.f), 40);
			Light light = Light(glm::vec3{ j,0.f,i }, emmisive);
			light.intensity = glm::vec3{ 3,3,3 };
			groundLights.push_back(light);
			grid.push_back(Object(glm::vec3{ j,0,i }, glm::vec3(0, 0, 0), glm::vec3(1, 0.1f, 1), mat));
		}
	}

	// generate dancers
	{
		MyMaterial* dancerMat = new MyMaterial(shaders["LightShader"], meshes["couple"], glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 0);
		for (int i = 0; i < 6; i++) {
			int x = 2 + rand() % 4;
			int z = 2 + rand() % 4;
			dancers.push_back(Object(glm::vec3{ x, 0,z }, glm::vec3{ -RADIANS(90),0,0 }, glm::vec3{ 0.25f, 0.25f, 0.25f }, dancerMat));
			glm::vec3 target;
			target.x = 2 + rand() % 4;
			target.y = 0;
			target.z = 2 + rand() % 4;
			targets.push_back(target);
		}
	}

	// generate walls
	MyMaterial* mat = new MyMaterial(shaders["LightShader"], meshes["box"], glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 0);
	walls.push_back(Object(glm::vec3{ 3.5f, 3.f, -0.5f }, glm::vec3{ 0,0,0 }, glm::vec3{ 8,6,0.1f }, mat));
	walls.push_back(Object(glm::vec3{ 3.5f, 3.f, 8 * sc.z - 0.5f }, glm::vec3{ 0,0,0 }, glm::vec3{ 8,6,0.1f }, mat));
	walls.push_back(Object(glm::vec3{ -sc.z / 2, 3.f, 3.5f }, glm::vec3{ 0,0,0 }, glm::vec3{ 0.1f,6,8 }, mat));
	walls.push_back(Object(glm::vec3{ 8 * sc.z - 0.5f, 3.f, 3.5f }, glm::vec3{ 0,0,0 }, glm::vec3{ 0.1f,6,8 }, mat));
	walls.push_back(Object(glm::vec3{ 3.5, 6.f,3.5 }, glm::vec3{ 0,0,0 }, glm::vec3{ 8,0.1f,8 }, mat));

	// generate reflectors
	AddMeshToList(GenerateCone("Reflector", glm::vec3(1, 1, 1), 1, 30, 1, true));

	float height = 7;
	int posY = 1;
	{
		glm::vec3 emmisive = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		Light light = Light(glm::vec3(1, height * posY - 1, 1), emmisive);
		light.lightDirection = glm::vec3(0, -1, 0);
		light.cutOffAngle = RADIANS(15);
		light.lightType = 1;
		light.intensity = glm::vec3{ 2,2,2 };
		MyMaterial* reflectorMat = new MyMaterial(shaders["LightShader"], meshes["Reflector"], glm::vec3(1, 1, 1), emmisive, glm::vec3(1, 1, 1), 30);

		reflectors.push_back(Object(glm::vec3(1, -1, 1), glm::vec3(0, 0, 0), glm::vec3(tan(light.cutOffAngle), 1, tan(light.cutOffAngle)) * height, reflectorMat));
		reflectorLights.push_back(light);
	}
	{
		glm::vec3 emmisive = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		Light light = Light(glm::vec3(6, height * posY - 1, 3), emmisive);
		light.lightDirection = glm::vec3(0, -1, 0);
		light.cutOffAngle = RADIANS(15);
		light.lightType = 1;
		light.intensity = glm::vec3{ 2,2,2 };
		MyMaterial* reflectorMat = new MyMaterial(shaders["LightShader"], meshes["Reflector"], glm::vec3(1, 1, 1), emmisive, glm::vec3(1, 1, 1), 30);

		reflectors.push_back(Object(glm::vec3(6, -1, 3), glm::vec3(0, 0, 0), glm::vec3(tan(light.cutOffAngle), 1, tan(light.cutOffAngle)) * height, reflectorMat));
		reflectorLights.push_back(light);
	}
	{
		glm::vec3 emmisive = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		Light light = Light(glm::vec3(3, height * posY - 1, 6), emmisive);
		light.lightDirection = glm::vec3(0, -1, 0);
		light.cutOffAngle = RADIANS(15);
		light.lightType = 1;
		light.intensity = glm::vec3{ 2,2,2 };
		MyMaterial* reflectorMat = new MyMaterial(shaders["LightShader"], meshes["Reflector"], glm::vec3(1, 1, 1), emmisive, glm::vec3(1, 1, 1), 30);

		reflectors.push_back(Object(glm::vec3(3, -1, 6), glm::vec3(0, 0, 0), glm::vec3(tan(light.cutOffAngle), 1, tan(light.cutOffAngle)) * height, reflectorMat));
		reflectorLights.push_back(light);
	}
	{
		glm::vec3 emmisive = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		Light light = Light(glm::vec3(6, height * posY - 1, 6), emmisive);
		light.lightDirection = glm::vec3(0, -1, 0);
		light.cutOffAngle = RADIANS(15);
		light.lightType = 1;
		light.intensity = glm::vec3{ 2,2,2 };
		MyMaterial* reflectorMat = new MyMaterial(shaders["LightShader"], meshes["Reflector"], glm::vec3(1, 1, 1), emmisive, glm::vec3(1, 1, 1), 30);

		reflectors.push_back(Object(glm::vec3(6, -1, 6), glm::vec3(0, 0, 0), glm::vec3(tan(light.cutOffAngle), 1, tan(light.cutOffAngle)) * height, reflectorMat));
		reflectorLights.push_back(light);
	}

	// generate globe
	{
		mapTextures["globe"] = CreateRandomTexture(16, 16);
		MyMaterial* globeMat = new MyMaterial(shaders["LightShader"], meshes["sphere"], glm::vec3(0.5f, 1, 1), glm::vec3{ 0,0,0 }, glm::vec3(1, 1, 1), 30);
		globeMat->texture = mapTextures["globe"];
		globeMat->texture_2 = mapTextures["globe"];
		globe = Object(glm::vec3(4, 5, 4), glm::vec3{ 0,0,0 }, glm::vec3{ 2,2,2 }, globeMat);
		Light light = Light(glm::vec3(4, 5, 4), glm::vec3{ 1,1,1 });
		light.lightType = 2;
		light.intensity = glm::vec3{ 0.3f,0.3f,0.3f };
		light.timeColorChange = 3.f;
		light.timeSinceColorChanged = 0;
		globe.getMaterial()->texture_2 = CreateRandomTexture(16, 16);
		globeLight = light;

		Light shine = Light(glm::vec3(4, 5, 4), glm::vec3{ 1.f,1.f,1.f });
		shine.lightType = 3;
		shine.intensity = glm::vec3{ 0.004,0.004,0.004 };
		globeShine = shine;
	}
}


void Tema3::FrameStart() {
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds) {

	// change dancer position
	UpdateDancers(deltaTimeSeconds);

	// change reflector direction
	UpdateReflectorRotation(deltaTimeSeconds);

	// change colors at random intervals
	if (showReflectors)
		UpdateReflectorLights(deltaTimeSeconds);
	if (showGround)
		UpdateGroundLights(deltaTimeSeconds);
	if (showGlobe)
		UpdateGlobeLights(deltaTimeSeconds);

	// Render Ground
	{
		{
			for (Object& obj : grid) {
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, obj.getPosition());
				modelMatrix = glm::scale(modelMatrix, obj.getScale());
				int k = 0;
				if (showReflectors) {
					for (int i = 0; i < reflectorLights.size(); i++) {
						if (k == 9)
							break;
						if (CheckConeCollision(obj.getPosition(), obj.getScale(), i)) {
							{
								lightPos[k] = reflectorLights[i].pos;
								lightColor[k] = reflectorLights[i].color;
								lightDir[k] = reflectorLights[i].lightDirection;
								lightType[k] = reflectorLights[i].lightType;
								lightCutOff[k] = reflectorLights[i].cutOffAngle;
								lightIntensity[k] = reflectorLights[i].intensity;
								k++;
							}
						}
					}
				}
				if (showGlobe) {
					lightPos[k] = globeLight.pos;
					lightColor[k] = globeLight.color;
					lightType[k] = globeLight.lightType;
					lightIntensity[k] = globeLight.intensity;
					k++;
					lightPos[k] = globeShine.pos;
					lightColor[k] = globeShine.color;
					lightType[k] = globeShine.lightType;
					lightIntensity[k] = globeShine.intensity;
				}
				RenderSimpleMesh(&obj, modelMatrix, 0, 0);
			}
		}
	}
	// Render Walls
	{
		for (int i = 0; i < walls.size(); i++) {
			int k = 0;
			if (showGround) {
				for (int j = 0; j < 8; j++) {
					switch (i) {
					case 0: {
						lightPos[j] = groundLights[j].pos;
						lightColor[j] = groundLights[j].color;
						lightIntensity[j] = groundLights[j].intensity;
						k++;
						break;
					}
					case 1: {
						lightPos[j] = groundLights[56 + j].pos;
						lightColor[j] = groundLights[56 + j].color;
						lightIntensity[j] = groundLights[j].intensity;
						k++;
						break;
					}
					case 2: {
						lightPos[j] = groundLights[j * 8].pos;
						lightColor[j] = groundLights[j * 8].color;
						lightIntensity[j] = groundLights[j].intensity;
						k++;
						break;
					}
					case 3: {
						lightPos[j] = groundLights[7 + j * 8].pos;
						lightColor[j] = groundLights[7 + j * 8].color;
						lightIntensity[j] = groundLights[j].intensity;
						k++;
						break;
					}
					}
				}
			}
			if (showReflectors) {
				for (int i = 0; i < reflectorLights.size(); i++) {
					//if (CheckConeCollision(walls[i].getPosition(), walls[i].getScale(), i)) {
						//{
					lightPos[k] = reflectorLights[i].pos;
					lightColor[k] = reflectorLights[i].color;
					lightDir[k] = reflectorLights[i].lightDirection;
					lightType[k] = reflectorLights[i].lightType;
					lightCutOff[k] = reflectorLights[i].cutOffAngle;
					lightIntensity[k] = reflectorLights[i].intensity;
					k++;
					//}
				//}
				}
			}
			if (showGlobe) {
				lightPos[k] = globeLight.pos;
				lightColor[k] = globeLight.color;
				lightType[k] = globeLight.lightType;
				lightIntensity[k] = globeLight.intensity;
				k++;
				lightPos[k] = globeShine.pos;
				lightColor[k] = globeShine.color;
				lightType[k] = globeShine.lightType;
				lightIntensity[k] = globeShine.intensity;
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, walls[i].getPosition());
			modelMatrix = glm::scale(modelMatrix, walls[i].getScale());
			RenderSimpleMesh(&walls[i], modelMatrix, 0, 0);
		}

	}
	// Render Dancers
	{
		for (Object& dancer : dancers) {
			int k = 0;
			if (showReflectors) {
				for (int i = 0; i < reflectorLights.size(); i++) {
					if (CheckConeCollision(dancer.getPosition(), dancer.getScale(), i)) {
						lightPos[k] = reflectorLights[i].pos;
						lightColor[k] = reflectorLights[i].color;
						lightDir[k] = reflectorLights[i].lightDirection;
						lightType[k] = reflectorLights[i].lightType;
						lightCutOff[k] = reflectorLights[i].cutOffAngle;
						lightIntensity[k] = reflectorLights[i].intensity;
						k++;
					}
				}
			}
			if (showGround) {
				for (int i = 0; i < groundLights.size(); i++) {
					if (glm::distance(groundLights[i].pos, dancer.getPosition()) <= 1.f) {
						lightPos[k] = groundLights[i].pos;
						lightColor[k] = groundLights[i].color;
						lightType[k] = groundLights[i].lightType;
						lightIntensity[k] = groundLights[i].intensity;
						k++;
					}
				}
			}
			if (showGlobe) {
				lightPos[k] = globeLight.pos;
				lightColor[k] = globeLight.color;
				lightType[k] = globeLight.lightType;
				lightIntensity[k] = globeLight.intensity;
				k++;
				lightPos[k] = globeShine.pos;
				lightColor[k] = globeShine.color;
				lightType[k] = globeShine.lightType;
				lightIntensity[k] = globeShine.intensity;
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, dancer.getPosition());
			modelMatrix = glm::rotate(modelMatrix, dancer.getRotation().x, glm::vec3{ 1,0,0 });
			modelMatrix = glm::rotate(modelMatrix, dancer.getRotation().z, glm::vec3{ 0,0,1 });

			modelMatrix = glm::scale(modelMatrix, dancer.getScale());
			RenderSimpleMesh(&dancer, modelMatrix, 0, 0);
		}
	}

	// Render Reflectors
	if (showReflectors) {
		for (int i = 0; i < reflectors.size(); i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, reflectors[i].getPosition());
			modelMatrix = glm::rotate(modelMatrix, RADIANS(180), glm::vec3(1, 0, 0));

			modelMatrix = glm::translate(modelMatrix, glm::vec3{ 0,-7,0 });
			modelMatrix = glm::rotate(modelMatrix, reflectors[i].getRotation().x, glm::vec3_right);
			modelMatrix = glm::rotate(modelMatrix, reflectors[i].getRotation().z, glm::vec3_backward);
			modelMatrix = glm::translate(modelMatrix, glm::vec3{ 0,7,0 });

			modelMatrix = glm::scale(modelMatrix, reflectors[i].getScale());

			lightPos[0] = reflectorLights[i].pos;
			lightColor[0] = reflectorLights[i].color;
			lightDir[0] = reflectorLights[i].lightDirection;
			lightType[0] = reflectorLights[i].lightType;
			lightCutOff[0] = reflectorLights[i].cutOffAngle;
			RenderSimpleMesh(&reflectors[i], modelMatrix, 1, 0);
		}
	}

	// Render Globe
	{
		if (showGlobe) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, globe.getPosition());
			modelMatrix = glm::scale(modelMatrix, globe.getScale());

			lightPos[0] = globeLight.pos;
			lightColor[0] = globeLight.color;
			lightType[0] = globeLight.lightType;
			lightIntensity[0] = globeLight.intensity;

			lightPos[1] = globeShine.pos;
			lightColor[1] = globeShine.color;
			lightType[1] = globeShine.lightType;
			lightIntensity[1] = globeShine.intensity;
			RenderSimpleMesh(&globe, modelMatrix, 0, 1);
		}
	}
}


void Tema3::FrameEnd() {
}


void Tema3::RenderSimpleMesh(Object* obj, const glm::mat4& modelMatrix, int isReflector, int isGlobe) {
	if (!obj->getMaterial()->mesh || !obj->getMaterial()->shader || !obj->getMaterial()->shader->GetProgramID())
		return;

	Shader* shader = obj->getMaterial()->shader;
	Mesh* mesh = obj->getMaterial()->mesh;
	MyMaterial* mat = obj->getMaterial();

	if (isReflector) {
		// se vor desena doar fatetele fata
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// aceasta directiva este folosita pentru nu se scrie in depth buffer
		glDepthMask(GL_FALSE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light properties
	GLuint loc_lightPos = glGetUniformLocation(shader->program, "lightPos");
	glUniform3fv(loc_lightPos, 14, glm::value_ptr(lightPos[0]));
	GLuint loc_lightCol = glGetUniformLocation(shader->program, "lightColor");
	glUniform3fv(loc_lightCol, 14, glm::value_ptr(lightColor[0]));
	GLuint loc_lightType = glGetUniformLocation(shader->program, "lightType");
	glUniform1iv(loc_lightType, 14, lightType);
	GLuint loc_lightDir = glGetUniformLocation(shader->program, "lightDir");
	glUniform3fv(loc_lightDir, 14, glm::value_ptr(lightDir[0]));
	GLuint loc_cutOffAngle = glGetUniformLocation(shader->program, "lightCutOff");
	glUniform1fv(loc_cutOffAngle, 14, lightCutOff);
	GLuint loc_lightIntensity = glGetUniformLocation(shader->program, "lightIntensity");
	glUniform3fv(loc_lightIntensity, 14, glm::value_ptr(lightIntensity[0]));

	GLuint loc_isReflector = glGetUniformLocation(shader->program, "isReflector");
	glUniform1i(loc_isReflector, isReflector);
	GLuint loc_isGlobe = glGetUniformLocation(shader->program, "isGlobe");
	glUniform1i(loc_isGlobe, isGlobe);

	// Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
	GLuint eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	GLuint loc_globe_position = glGetUniformLocation(shader->program, "globe_position");
	glUniform3f(loc_globe_position, globe.getPosition().x, globe.getPosition().y, globe.getPosition().z);

	// Set material property uniforms (shininess, kd, ks, ka, ke) 
	GLuint material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform3f(material_kd, mat->materialKd.x, mat->materialKd.y, mat->materialKd.z);
	GLuint material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform3f(material_ks, mat->materialKs.x, mat->materialKs.y, mat->materialKs.z);
	GLuint material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, mat->materialShininess);
	GLuint material_ke = glGetUniformLocation(shader->program, "material_ke");
	glUniform3f(material_ke, mat->materialKe.x, mat->materialKe.y, mat->materialKe.z);

	if (isGlobe) {
		glActiveTexture(GL_TEXTURE0);
		// - bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, mat->texture->GetTextureID());
		// - send the uniform value
		GLuint loc = glGetUniformLocation(shader->program, "texture_1");
		glUniform1i(loc, 0);

		glActiveTexture(GL_TEXTURE1);
		// - bind the texture2 ID
		glBindTexture(GL_TEXTURE_2D, mat->texture_2->GetTextureID());
		// - send the uniform value
		GLuint loc_2 = glGetUniformLocation(shader->program, "texture_2");
		glUniform1i(loc_2, 1);
	}
	glUniform1f(glGetUniformLocation(shader->program, "t"), globeLight.timeSinceColorChanged / globeLight.timeColorChange);
	glUniform1f(glGetUniformLocation(shader->program, "time"), static_cast<float>(Engine::GetElapsedTime()));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	GLuint loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	GLuint loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

	if (isReflector) {
		// se dezactiveaza actiunile tuturor directivelor apelate anterior
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}
	ResetLights();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods) {
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
		// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_W)) {
			reflectorLights[0].pos -= forward * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() - forward * deltaTime * 2.f);
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			reflectorLights[0].pos -= right * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() - right * deltaTime * 2.f);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			reflectorLights[0].pos += forward * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() + forward * deltaTime * 2.f);

		}
		if (window->KeyHold(GLFW_KEY_D)) {
			reflectorLights[0].pos += right * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() + right * deltaTime * 2.f);

		}
		if (window->KeyHold(GLFW_KEY_E)) {
			reflectorLights[0].pos += up * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() + up * deltaTime * 2.f);
		}
		if (window->KeyHold(GLFW_KEY_Q)) {
			reflectorLights[0].pos -= up * deltaTime * 2.f;
			reflectors[0].setPosition(reflectors[0].getPosition() - up * deltaTime * 2.f);
		}
	}
}


void Tema3::OnKeyPress(int key, int mods) {
	if (key == GLFW_KEY_1) {
		showGlobe = !showGlobe;
	}
	if (key == GLFW_KEY_2) {
		showReflectors = !showReflectors;
	}
	if (key == GLFW_KEY_3) {
		showGround = !showGround;
		if (!showGround) {
			for (int i = 0; i < grid.size(); i++) {
				grid[i].getMaterial()->materialKe = glm::vec3(0, 0, 0);
			}
		}
	}
}


void Tema3::OnKeyRelease(int key, int mods) {
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}


void Tema3::OnWindowResize(int width, int height) {
}

Mesh* Tema3::GenerateCone(const std::string& name, const glm::vec3& color, float r, int no_sides, float height, bool fill) {
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;

	vertices.push_back({ glm::vec3(0, -height, 0), color, glm::vec3(1, 1, 1) });

	for (int i = 1; i < no_sides + 2; i++) {
		float x, z;
		x = r * cos(i * 2 * M_PI / no_sides + M_PI / 4);
		z = r * sin(i * 2 * M_PI / no_sides + M_PI / 4);

		vertices.push_back({ glm::vec3(x, 0, z), color, glm::vec3(1, 1, 0.6) });
	}
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}
	Mesh* circle = new Mesh(name);

	if (!fill) {
		vertices.erase(vertices.begin());
		indices.erase(indices.begin());
		vertices.push_back(*vertices.begin());
		indices.push_back(*indices.begin());
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else
		circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(vertices, indices);

	return circle;
}

bool m1::Tema3::CheckConeCollision(const glm::vec3& pos, const glm::vec3& sc, int i) {
	glm::vec3 scale = sc;
	if (scale.x < 1)
		scale = glm::vec3{ 2.f,2.f,2.f };
	glm::vec3 leftDown = glm::vec3{ pos.x - scale.x / 2,pos.y - scale.y / 2,pos.z + scale.z / 2 };
	glm::vec3 rightDown = glm::vec3{ pos.x + scale.x / 2,pos.y - scale.y / 2,pos.z + scale.z / 2 };
	glm::vec3 leftTop = glm::vec3{ pos.x - scale.x / 2,pos.y - scale.y / 2,pos.z - scale.z / 2 };
	glm::vec3 rightTop = glm::vec3{ pos.x + scale.x / 2,pos.y - scale.y / 2,pos.z - scale.z / 2 };

	glm::vec3 currPos = leftDown;
	while (currPos.x <= rightDown.x) {
		float cone_dist = glm::dot(currPos - reflectorLights[i].pos, reflectorLights[i].lightDirection);
		float cone_radius = (cone_dist / reflectors[i].getScale().y) * reflectors[i].getScale().x;
		float orth_distance = glm::length((currPos - reflectorLights[i].pos) - cone_dist * reflectorLights[i].lightDirection);
		if (orth_distance < cone_radius)
			return true;
		currPos.x += 0.1f;
	}
	currPos = leftTop;
	while (currPos.z <= rightTop.z) {
		glm::vec3 pos = leftTop;
		float cone_dist = glm::dot(currPos - reflectorLights[i].pos, reflectorLights[i].lightDirection);
		float cone_radius = (cone_dist / reflectors[i].getScale().y) * reflectors[i].getScale().x;
		float orth_distance = glm::length((currPos - reflectorLights[i].pos) - cone_dist * reflectorLights[i].lightDirection);
		if (orth_distance < cone_radius)
			return true;
		currPos.z += 0.1f;
	}
	return false;
}

void m1::Tema3::ResetLights() {
	for (int i = 0; i < 14; i++) {
		lightPos[i] = glm::vec3{ 0,0,0 };
		lightColor[i] = glm::vec3{ 0,0,0 };
		lightDir[i] = glm::vec3{ 0,0,0 };
		lightType[i] = 0;
		lightCutOff[i] = 0;
	}
}

void m1::Tema3::UpdateGroundLights(float deltaTime) {
	for (int i = 0; i < groundLights.size(); i++) {
		if (groundLights[i].timeSinceColorChanged >= groundLights[i].timeColorChange) {
			groundLights[i].timeSinceColorChanged = 0;
			groundLights[i].color = groundLights[i].newColor;
			grid[i].getMaterial()->materialKe = groundLights[i].newColor;
			groundLights[i].timeColorChange = rand() % 4 + 1;
			groundLights[i].newColor = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		}
		else {
			groundLights[i].timeSinceColorChanged += deltaTime;

			glm::vec3 newColor = glm::mix(groundLights[i].color, groundLights[i].newColor, groundLights[i].timeSinceColorChanged / groundLights[i].timeColorChange);
			groundLights[i].color = newColor;
			grid[i].getMaterial()->materialKe = newColor;
		}
	}
}

void m1::Tema3::UpdateReflectorLights(float deltaTime) {
	for (int i = 0; i < reflectorLights.size(); i++) {
		if (reflectorLights[i].timeSinceColorChanged >= reflectorLights[i].timeColorChange) {
			reflectorLights[i].timeSinceColorChanged = 0;
			reflectorLights[i].color = reflectorLights[i].newColor;
			reflectors[i].getMaterial()->materialKe = reflectorLights[i].newColor;
			reflectorLights[i].timeColorChange = rand() % 7 + 1;
			reflectorLights[i].newColor = glm::vec3{ (float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX };
		}
		else {
			reflectorLights[i].timeSinceColorChanged += deltaTime;

			glm::vec3 newColor = glm::mix(reflectorLights[i].color, reflectorLights[i].newColor, reflectorLights[i].timeSinceColorChanged / reflectorLights[i].timeColorChange);
			reflectorLights[i].color = newColor;
			reflectors[i].getMaterial()->materialKe = newColor;
		}
	}
}

void m1::Tema3::UpdateGlobeLights(float deltaTime) {
	if (globeLight.timeSinceColorChanged >= globeLight.timeColorChange) {
		delete globe.getMaterial()->texture;
		globe.getMaterial()->texture = globe.getMaterial()->texture_2;
		globe.getMaterial()->texture_2 = CreateRandomTexture(16, 16);
		globeLight.timeColorChange = rand() % 3 + 1;
		globeLight.timeSinceColorChanged = 0;
	}
	else {
		globeLight.timeSinceColorChanged += deltaTime;
	}
}

void m1::Tema3::UpdateReflectorRotation(float deltaTime) {
	// change reflector direction
	for (int i = 0; i < reflectorLights.size(); i++) {
		if (reflectorLights[i].timeSinceDirChanged >= reflectorLights[i].timeDirChange) {
			reflectorLights[i].timeSinceDirChanged = 0;
			reflectorLights[i].dirOz = rand() % 2;
			if (reflectorLights[i].dirOz == 0)
				reflectorLights[i].dirOx = 1;
			if (rand() % 2 == 1)
				reflectorLights[i].dirOx *= -1;
			if (rand() % 2 == 1)
				reflectorLights[i].dirOz *= -1;

			reflectorLights[i].timeDirChange = rand() % 2 + 1.f;
		}
		else {
			reflectorLights[i].timeSinceDirChanged += deltaTime;
		}
	}

	// apply reflector rotation
	for (int i = 0; i < reflectorLights.size(); i++) {
		bool changedOz = false;
		bool changedOx = false;
		float angleOz = reflectors[i].getRotation().z;
		float angleOx = reflectors[i].getRotation().x;
		if (reflectorLights[i].dirOz) {
			angleOz += deltaTime * 0.2f * reflectorLights[i].dirOz;
			changedOz = true;
		}
		if (reflectorLights[i].dirOx) {
			angleOx += deltaTime * 0.2f * reflectorLights[i].dirOx;
			changedOx = true;
		}
		glm::mat4 rot = glm::mat4(1);
		if (changedOz) {
			rot = glm::rotate(rot, angleOz, glm::vec3_forward);
		}
		if (changedOx) {
			rot = glm::rotate(rot, angleOx, glm::vec3_right);
		}
		if (changedOz || changedOx) {
			reflectorLights[i].lightDirection = glm::vec3(rot * glm::vec4(glm::vec3(0, -1, 0), 0));
			reflectorLights[i].lightDirection = glm::normalize(reflectorLights[i].lightDirection);
			glm::vec3 oldRot = reflectors[i].getRotation();
			reflectors[i].setRotation(glm::vec3{ angleOx, oldRot.y, angleOz });
		}
		if (reflectorLights[i].dirOz == -1) {
			if (angleOz < -RADIANS(20)) {
				angleOz = -RADIANS(20);
				reflectorLights[i].dirOz = 1;
			}
		}
		if (reflectorLights[i].dirOz == 1) {
			if (angleOz > RADIANS(20)) {
				angleOz = RADIANS(20);
				reflectorLights[i].dirOz = -1;
			}
		}
		if (reflectorLights[i].dirOx == -1) {
			if (angleOx < -RADIANS(20)) {
				angleOx = -RADIANS(20);
				reflectorLights[i].dirOx = 1;
			}
		}
		if (reflectorLights[i].dirOx == 1) {
			if (angleOx > RADIANS(20)) {
				angleOx = RADIANS(20);
				reflectorLights[i].dirOx = -1;
			}
		}
	}
}

void m1::Tema3::UpdateDancers(float deltaTime) {
	for (int i = 0; i < dancers.size(); i++) {
		glm::vec3 rot = dancers[i].getRotation();
		rot.z += 0.5f * deltaTime;
		dancers[i].setRotation(rot);
		if (timePassedTargetChanged >= timeChangeTarget) {
			targets[i].x = rand() % 8;
			targets[i].z = rand() % 8;
			timePassedTargetChanged = 0;
		}
		else {
			glm::vec3 dir = glm::normalize(targets[i] - dancers[i].getPosition());
			glm::vec3 pos = dancers[i].getPosition();
			if (glm::distance(pos, targets[i]) <= 0.6f) {
				targets[i].x = rand() % 8;
				targets[i].z = rand() % 8;
			}
			pos += dir * 0.6f * deltaTime;
			dancers[i].setPosition(pos);
			timePassedTargetChanged += deltaTime;
		}
	}
}

Texture2D* m1::Tema3::CreateRandomTexture(unsigned int width, unsigned int height) {
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	for (int i = 0; i < size; i++)
		data[i] = rand() % 256;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (GLEW_EXT_texture_filter_anisotropic) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	CheckOpenGLError();

	Texture2D* texture = new Texture2D();
	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	return texture;
}

