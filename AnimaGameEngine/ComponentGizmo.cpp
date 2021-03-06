#include "ComponentGizmo.h"
#include "Globals.h"
#include "Shader.h"
#include <glew_2.0.0/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"

ComponentGizmo::ComponentGizmo(
	ComponentType type,
	const std::string& vertexPath,
	const std::string &fragmentPath,
	GameObject *ownerGO) : Component(type, ownerGO)
{
	GenerateVertexData();
	ComputeNumVerticesToDraw();
	SetVertexBuffers();
	ClearVertexBuffers();

	CreateShader(vertexPath, fragmentPath);
	Disable();
}

ComponentGizmo::~ComponentGizmo()
{
	RELEASE(shader);
}

void ComponentGizmo::Update(float dt)
{
	std::string name = GetOwnerGO()->GetName();
	//Draw
	if (IsEnabled())
	{
		glm::mat4 model;

		const glm::vec3 translation = GetOwnerGO()->GetTransform()->GetWorldPosition();
		model = glm::translate(model, translation);

		const glm::quat rotQuat = GetOwnerGO()->GetTransform()->GetWorldRotation();
		const glm::mat4 rotMatrix = glm::mat4_cast(rotQuat);

		model = model * rotMatrix;

		//Set shader uniforms
		
		shader->Use();
		shader->SetMat4("model", model);
		
		ComponentCamera *camera = (ComponentCamera*)App->scene->activeCameraComponent;

		shader->SetMat4("view", camera->GetViewMatrix());
		shader->SetMat4("projection", camera->GetProjectionMatrix());
		
		glBindVertexArray(VAO);

		glDrawArrays(GL_LINES, 0, numVertices);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			MYLOG(" Error despu�s glDrawArrays(): %s\n", gluErrorString(error));
		}

		glBindVertexArray(0);		
	}
}

void ComponentGizmo::CreateShader(const std::string& vertexPath, const std::string &fragmentPath)
{
	shader = new Shader(vertexPath, fragmentPath);
}

void ComponentGizmo::GenerateVertexData()
{
	vertices = {
		//axis X										//red
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f,				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		//arrow X								
		0.75f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.75f,-0.25f,0.0f, 1.0f, 0.0f, 0.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		//label X
		1.0f, 0.25f, 0.0f, 1.5f, -0.25f, 0.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -0.25f, 0.0f, 1.5f, 0.25f, 0.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		//axis Y										//green
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,				0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		//arrow Y
		0.0f, 1.0f, 0.0f, 0.25f, 0.75, 0.0f,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, -0.25f, 0.75f, 0.0f,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		//label Y
		0.0f, 1.30f, 0.0f, 0.25f, 1.55f, 0.0f,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.30f, 0.0f, -0.25f, 1.55f, 0.0f,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.30f, 0.0f, 0.0f, 1.05f, 0.0f,			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		//axis Z										//blue
		0.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,				0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		//arrow Z
		0.0f, 0.0f, 1.0f, 0.25f, 0.0f, 0.75f,			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, -0.25f, 0.0f, 0.75f,			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		//label Z
		-0.25f, 0.25f, 1.25f, 0.25f, 0.25f, 1.25f,		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.25f, 0.25f, 1.25f, -0.25f, -0.25f, 1.25f,		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.25f, -0.25f, 1.25f, 0.25f, -0.25f, 1.25f,	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
}

void ComponentGizmo::SetVertexBuffers()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//vertices location
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//color location
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));

}

void ComponentGizmo::ClearVertexBuffers()
{
	vertices.clear();
}

void ComponentGizmo::ComputeNumVerticesToDraw()
{
	numVertices = vertices.size() / 3;
}
