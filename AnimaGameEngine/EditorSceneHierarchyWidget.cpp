
#include "EditorSceneHierarchyWidget.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "EditorInspectorWidget.h"
#include "imgui.h"

EditorSceneHierarchyWidget::EditorSceneHierarchyWidget(const std::string &title, 
	int x, 
	int y, 
	unsigned int width, 
	unsigned int height) : title(title), pos_x(x), pos_y(y), width(width), height(height) {} 


EditorSceneHierarchyWidget::EditorSceneHierarchyWidget(){}

EditorSceneHierarchyWidget::~EditorSceneHierarchyWidget() {}

void EditorSceneHierarchyWidget::Draw(EditorInspectorWidget *inspector)
{
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiSetCond_FirstUseEver);

	static void *selected_go = nullptr;

	ImGui::Begin(title.c_str());
	
	for (GameObject *go : App->scene->gameObjects)
	{
		DrawNode(go, selected_go);
	}

	inspector->Draw((GameObject*)selected_go);

	ImGui::End();
}

void EditorSceneHierarchyWidget::DrawNode(GameObject *go, void* &selected_go) const
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_OpenOnDoubleClick | (go == selected_go ? ImGuiTreeNodeFlags_Selected : 0);

	bool is_leaf = go->GetChildrenGO().size() == 0;

	if (is_leaf)
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool node_open = ImGui::TreeNodeEx(go, node_flags, go->GetName().c_str());
	if (ImGui::IsItemClicked())
	{
		selected_go = go;	
	}

	if (node_open && !is_leaf)
	{
		for (GameObject *child_go : go->GetChildrenGO())
		{
			DrawNode(child_go, selected_go);
		}

		ImGui::TreePop();
	}
}
