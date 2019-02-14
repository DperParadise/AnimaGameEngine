
#include "EditorInspectorWidget.h"
#include "imgui.h"
#include "Component.h"
#include "GameObject.h"
#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtc/quaternion.hpp"
#include "Globals.h"
#include <cmath>
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"

EditorInspectorWidget::EditorInspectorWidget(const std::string &title, 
	int x, 
	int y, 
	unsigned int width, 
	unsigned int height) : title(title), pos_x(x), pos_y(y), width(width), height(height){}

EditorInspectorWidget::EditorInspectorWidget() {}

EditorInspectorWidget::~EditorInspectorWidget() {}

void EditorInspectorWidget::Draw(GameObject *go)
{
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiSetCond_FirstUseEver);

	ImGui::Begin(title.c_str());
	if (go)
	{
		ImGui::Checkbox("Enabled", &go->enabled);

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			glm::vec3 pos = go->GetTransform()->GetWorldPosition();

			bool XPosField = ImGui::InputFloat("x", &pos.x, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool YPosField = ImGui::InputFloat("y", &pos.y, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool ZPosField = ImGui::InputFloat("z", &pos.z, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");
			
			if (XPosField || YPosField || ZPosField)
			{		
				go->Translate(pos);
			}
			//TODO: Improve rotations stuff.
			ImGui::Text("Rotation");
			glm::quat rot = go->GetTransform()->GetWorldRotation();
			glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(rot));
			
			bool XRotField = ImGui::InputFloat("x", &eulerRot[0], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool YRotField = ImGui::InputFloat("y", &eulerRot[1], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool ZRotField = ImGui::InputFloat("z", &eulerRot[2], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			if (XRotField || YRotField || ZRotField)
			{			
				go->GetTransform()->SetWorldRotation(eulerRot);
				
			}

			ImGui::Text("Scale");
			glm::vec3 scale = go->GetTransform()->GetWorldScale();

			bool XScaleField = ImGui::InputFloat("x", &scale.x, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool YScaleField = ImGui::InputFloat("y", &scale.y, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool ZScaleField = ImGui::InputFloat("z", &scale.z, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			if (XScaleField || YScaleField || ZScaleField)
			{
				go->GetTransform()->SetWorldScale(scale);
			}
		}

		for (Component *comp : go->GetComponents())
		{
			ShowComponentInfo(comp);
		}
	}
	
	ImGui::End();
}

void EditorInspectorWidget::ShowComponentInfo(Component *comp)
{
	/*
	switch (comp->comp_type)
	{
	case ComponentType::MESH_RENDERER:
		ShowMeshRendererInfo((ComponentMeshRenderer*)comp);
		break;
	}
	*/
}

void EditorInspectorWidget::ShowMeshRendererInfo(ComponentMeshRenderer *mr)
{
	/*
	if (ImGui::CollapsingHeader("Mesh renderer"))
	{
		ImGui::Checkbox("Active", &mr->active);
		ImGui::NewLine();
		ImGui::Text("Mesh:");
		ImGui::Text(mr->owner_mesh->mesh_name.c_str());
		ImGui::NewLine();
		ImGui::Text("Material:");
		ImGui::Text(mr->owner_mesh->mesh_mat->mat_name.c_str());
		ImGui::ColorEdit4("ambient", mr->owner_mesh->mesh_mat->material.ambient);
		ImGui::ColorEdit4("diffuse", mr->owner_mesh->mesh_mat->material.diffuse);
		ImGui::ColorEdit4("specular", mr->owner_mesh->mesh_mat->material.specular);
		ImGui::InputFloat("shininess", &mr->owner_mesh->mesh_mat->material.shininess, 0.01f, 1.0f);
		ImGui::NewLine();
		ImGui::Text("Diffuse Texture:");
		ImGui::Text(mr->owner_mesh->mesh_mat->diffuse_texture.data);
	}
	*/
}

void EditorInspectorWidget::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}