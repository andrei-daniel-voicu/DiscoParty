#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
	polygonMode = GL_FILL;

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	// Initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// Initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;

	// Initialize angular steps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	// bonus
	// Initialise sphere mesh
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	currPlanet = 0;
	angularStep1 = 0;
	angularStep2 = 0;
	angularStep3 = 0;

	angularStepRot1 = 0;
	angularStepRot2 = 0;

	d1 = 2.5f;
	d2 = 2.5f;

	v1 = 40.f;
	v2 = 40.f;

	w1 = v1 / d1;
	w2 = v2 / d2;
}


void Lab4::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sets the screen area where to draw
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
	angularStep1 = angularStep1 >= 360 ? angularStep1 - 360 : angularStep1 + deltaTimeSeconds * 40;
	angularStep2 = angularStep2 >= 360 ? angularStep2 - 360 : angularStep2 + deltaTimeSeconds * 60;
	angularStep3 = angularStep3 >= 360 ? angularStep3 - 360 : angularStep3 + deltaTimeSeconds * 20;

	angularStepRot1 = angularStepRot1 >= 360 ? angularStepRot1 - 360 : angularStepRot1 + deltaTimeSeconds * w1;
	angularStepRot2 = angularStepRot2 >= 360 ? angularStepRot2 - 360 : angularStepRot2 + deltaTimeSeconds * w2;

	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
	modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= transform3D::RotateOX(angularStepOX * AI_MATH_PI / 180);
	modelMatrix *= transform3D::RotateOY(angularStepOY * AI_MATH_PI / 180);
	modelMatrix *= transform3D::RotateOZ(angularStepOZ * AI_MATH_PI / 180);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


	// bonus
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(2, 0, 0);
	modelMatrix *= transform3D::RotateOY(angularStep3 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::Scale(2, 2, 2);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

	modelMatrix *= transform3D::RotateOY(-angularStep3 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::RotateOY(angularStepRot1 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::Translate(d1, 0.0f, 0);
	modelMatrix *= transform3D::RotateOY(angularStep1 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

	modelMatrix *= transform3D::RotateOY(-angularStep1 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::RotateOY(angularStepRot2 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::Translate(d2, 0.0f, 0);
	modelMatrix *= transform3D::RotateOY(angularStep2 * AI_MATH_PI / 180);
	modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

}


void Lab4::FrameEnd()
{
	DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
	// TODO(student): Add transformation logic
	if (window->KeyHold(GLFW_KEY_W))
	{
		translateY += 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S))
	{
		translateY -= 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A))
	{
		translateX -= 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D))
	{
		translateX += 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_R))
	{
		translateZ -= 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_F))
	{
		translateZ += 10 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_1))
	{
		scaleX += 10 * deltaTime;
		scaleY += 10 * deltaTime;
		scaleZ += 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_2))
	{
		if (scaleX - 10 * deltaTime >= 0)
			scaleX -= 10 * deltaTime;
		if (scaleY - 10 * deltaTime >= 0)
			scaleY -= 10 * deltaTime;
		if (scaleZ - 10 * deltaTime >= 0)
			scaleZ -= 10 * deltaTime;


	}
	if (window->KeyHold(GLFW_KEY_3))
	{
		angularStepOX = angularStepOX >= 360 ? angularStepOX - 360 : angularStepOX + deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_4))
	{
		angularStepOX = angularStepOX <= -360 ? angularStepOX + 360 : angularStepOX - deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_5))
	{
		angularStepOY = angularStepOY >= 360 ? angularStepOY - 360 : angularStepOY + deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_6))
	{
		angularStepOY = angularStepOY <= -360 ? angularStepOY + 360 : angularStepOY - deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_7))
	{
		angularStepOZ = angularStepOZ >= 360 ? angularStepOZ - 360 : angularStepOZ + deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_8))
	{
		angularStepOZ = angularStepOZ <= -360 ? angularStepOZ + 360 : angularStepOZ - deltaTime * 40;
	}
	if (window->KeyHold(GLFW_KEY_O))
	{
		if (!currPlanet)
		{
			v1 -= 60 * deltaTime;
			w1 = v1 / d1;
		}
		else
		{
			v2 -= 60 * deltaTime;
			w2 = v2 / d2;
		}
	}
	if (window->KeyHold(GLFW_KEY_P))
	{
		if (!currPlanet)
		{
			v1 += 60 * deltaTime;
			w1 = v1 / d1;
		}
		else
		{
			v2 += 60 * deltaTime;
			w2 = v2 / d2;

		}
	}
	if (window->KeyHold(GLFW_KEY_K))
	{
		if (!currPlanet)
		{
			d1 -= 5.f * deltaTime;
			w1 = v1 / d1;
		}
		else
		{
			d2 -= 5.f * deltaTime;
			w2 = v2 / d2;
		}
	}
	if (window->KeyHold(GLFW_KEY_L))
	{
		if (!currPlanet)
		{
			d1 += 5.f * deltaTime;
			w1 = v1 / d1;
		}
		else
		{
			d2 += 5.f * deltaTime;
			w2 = v2 / d2;
		}
	}
}


void Lab4::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
	else if (key == GLFW_KEY_0)
	{
		currPlanet = !currPlanet;
	}
}


void Lab4::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
