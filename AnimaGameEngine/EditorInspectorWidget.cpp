#include "EditorInspectorWidget.h"
#include "libraries/ImGui/imgui.h"
#include "Component.h"
#include "GameObject.h"

#include "libraries/assimp/include/assimp/vector3.h"

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
			aiVector3D pos;
			
			pos = go->transform.local_position;

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
				if (pos != go->transform.local_position)
				{
					go->transform.local_position = pos;
					go->dirty = true;
				}
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