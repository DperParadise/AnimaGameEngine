#include "EditorInspectorWidget.h"
#include "libraries/ImGui/imgui.h"
#include "Component.h"
#include "GameObject.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/quaternion.h"
#include "Globals.h"
#include <cmath>
#include "libraries/MathGeoLib/MathGeoLib.h"
#include "ComponentMeshRenderer.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

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
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			aiVector3D pos = go->transform.relative_position;

			bool x_pos_field = ImGui::InputFloat("x", &pos.x, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool y_pos_field = ImGui::InputFloat("y", &pos.y, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool z_pos_field = ImGui::InputFloat("z", &pos.z, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");
			
			if (x_pos_field || y_pos_field || z_pos_field)
			{							
				go->transform.ResetPosition(pos.x, pos.y, pos.z);		
			}
			//TODO: Improve rotations stuff.
			ImGui::Text("Rotation");
			aiQuaternion rot = go->transform.relative_rotation;
			Quat rot_mathgeo = Quat(rot.x, rot.y, rot.z, rot.w);
			float3 rad = rot_mathgeo.ToEulerZYX();
			float3 degrees = RadToDeg(rad);

			/*if(go->name != "world origin")
				MYLOG("rotation q=(%f, %f, %f, %f) euler=(%f, %f, %f)", rot.x, rot.y, rot.z, rot.w, degrees.z, degrees.y, degrees.x);*/

			bool x_rot_field = ImGui::InputFloat("rx", &degrees[2], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool y_rot_field = ImGui::InputFloat("ry", &degrees[1], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool z_rot_field = ImGui::InputFloat("rz", &degrees[0], 1.0f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			if (x_rot_field || y_rot_field || z_rot_field)
			{			
				go->transform.world_rotation = go->parent_go ? go->parent_go->transform.world_rotation : aiQuaternion();			
				go->transform.relative_rotation = aiQuaternion(); //to reset rotation
				go->transform.Rotate(degrees[2], degrees[1], degrees[0]);
				go->dirty = true;			
			}

			ImGui::Text("Scale");
			aiVector3D scale = go->transform.relative_scale;

			bool x_scale_field = ImGui::InputFloat("x", &scale.x, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool y_scale_field = ImGui::InputFloat("y", &scale.y, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			bool z_scale_field = ImGui::InputFloat("z", &scale.z, 0.01f, 1.0f);
			ImGui::SameLine();
			ShowHelpMarker("CTRL + click on +/- for fast step");

			if (x_scale_field || y_scale_field || z_scale_field)
			{
				go->transform.relative_scale = scale;
			}
		}

		for (Component *comp : go->components)
		{
			ShowComponentInfo(comp);
		}
	}
	
	ImGui::End();
}

void EditorInspectorWidget::ShowComponentInfo(Component *comp)
{
	switch (comp->comp_type)
	{
	case ComponentType::MESH_RENDERER:
		ShowMeshRendererInfo((ComponentMeshRenderer*)comp);
		break;
	}
}

void EditorInspectorWidget::ShowMeshRendererInfo(ComponentMeshRenderer *mr)
{
	if (ImGui::CollapsingHeader("Mesh renderer"))
	{
		ImGui::Checkbox("Active", &mr->owner_go->active);
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