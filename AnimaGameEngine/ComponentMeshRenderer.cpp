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
#include "Globals.h"

ComponentMeshRenderer::ComponentMeshRenderer(
	ComponentType type, 
	const Mesh *mesh,
	const Shader *shader,
	const ComponentCamera *compCamera,
	GameObject *ownerGO,
	bool active) : mesh(mesh), shader(shader), camera(compCamera), Component(type, ownerGO, active){}

ComponentMeshRenderer::~ComponentMeshRenderer() 
{
	RELEASE(mesh);
	RELEASE(shader);
}

//Draw the mesh
void ComponentMeshRenderer::Update(float dt) {
	if (IsEnabled())
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

		shader->SetMat4("view", camera->GetViewMatrix());
		shader->SetMat4("projection", camera->GetProjectionMatrix());
		
		shader->SetVec3("material.ambient", mesh->GetMaterial().ambient);
		shader->SetVec3("material.diffuse", mesh->GetMaterial().diffuse);
		shader->SetVec3("material.specular", mesh->GetMaterial().specular);
		shader->SetFloat("material.shininess", mesh->GetMaterial().shininess);

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetEBO());
		glDrawElements(GL_TRIANGLES, mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);


		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			MYLOG("Error OpenGL: %s", gluErrorString(error))


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
}
