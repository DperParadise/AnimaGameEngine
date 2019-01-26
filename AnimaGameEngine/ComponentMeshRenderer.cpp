#include "ComponentMeshRenderer.h"
#include <vector>
#include "GameObject.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "Globals.h"
#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtc/matrix_transform.hpp"
#include "libraries/glm/gtc/quaternion.hpp"
#include "ComponentTransform.h"
#include "Shader.h"
#include "ComponentCamera.h"
#include "Mesh.h"
#include "ModuleEditorCamera.h"


ComponentMeshRenderer::ComponentMeshRenderer(
	ComponentType type, 
	const Mesh *mesh,
	const Shader *shader,
	const ComponentCamera *compCamera,
	const ModuleEditorCamera *editorCam,
	const std::string &name, 
	bool act, 
	GameObject *ownerGO) : mesh(mesh), sceneCamera(compCamera), editorCamera(editorCam), Component(type, name, ownerGO){}

ComponentMeshRenderer::~ComponentMeshRenderer() {}

//Draw the geometry
void ComponentMeshRenderer::Update(float dt) {
	if (IsActive())
	{
		glm::mat4 model;

		const glm::vec3 translation = GetOwnerGO()->GetTransform()->GetWorldPosition();
		model = glm::translate(model, translation);

		const glm::quat rotQuat = GetOwnerGO()->GetTransform()->GetWorldRotation();
		const glm::mat4 rotMatrix = glm::mat4_cast(rotQuat);

		//TODO: Check matrix product order
		model = rotMatrix * model;

		const glm::vec3 scale = GetOwnerGO()->GetTransform()->GetWorldScale();
		model = glm::scale(model, scale);

		glm::mat4 normalMatrix;
		normalMatrix = glm::inverse(model);
		normalMatrix = glm::transpose(normalMatrix);

		//Set shader uniforms
		shader->Use();
		shader->SetMat4("model", model);
		shader->SetMat4("normalMatrix", normalMatrix);

		//Check if we need to use the editor camera or the in-scene camera
		if (IsPlaying())
		{
			shader->SetMat4("view", sceneCamera->GetViewMatrix());
			shader->SetMat4("projection", sceneCamera->GetProjectionMatrix());
		}
		else
		{
			shader->SetMat4("view", editorCamera->GetViewMatrix());
			shader->SetMat4("projection", editorCamera->GetProjectionMatrix());
		}

		//Bind Textures
		unsigned int diffuse_nr = 1;
		unsigned int specular_nr = 1;
		unsigned int normal_nr = 1;
		unsigned int height_nr = 1;

		for (int i = 0; i < mesh->GetTextures().size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string type = mesh->GetTextures()[i].type;

			if (type == "texture_diffuse")
				number = std::to_string(diffuse_nr++);
			else if (type == "texture_specular")
				number = std::to_string(specular_nr++);
			else if (type == "texture_normals")
				number = std::to_string(normal_nr++);
			else if (type == "texture_height")
				number = std::to_string(height_nr++);
			
			shader->SetInt("material." + type + number, i);

			glBindTexture(GL_TEXTURE_2D, mesh->GetTextures()[i].id);
		}
		

		//Draw
		glBindVertexArray(mesh->GetVAO());
		glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}

bool ComponentMeshRenderer::IsPlaying() const
{
	return isPlaying;
}

void ComponentMeshRenderer::SetPlaying()
{
	isPlaying = true;
}
