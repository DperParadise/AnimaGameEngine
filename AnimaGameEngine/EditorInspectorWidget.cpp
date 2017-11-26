#include "EditorInspectorWidget.h"
#include "libraries/ImGui/imgui.h"
#include "Component.h"
#include "GameObject.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/quaternion.h"
#include "Globals.h"
#include <cmath>
#include "libraries/MathGeoLib/MathGeoLib.h"

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

		}

	}
	ImGui::End();
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