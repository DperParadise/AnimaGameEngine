
#include "EditorSceneHierarchyWidget.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "EditorInspectorWidget.h"
#include "imgui.h"
#include "ComponentGizmo.h"

EditorSceneHierarchyWidget::EditorSceneHierarchyWidget(const std::string &title, 
	int x, 
	int y, 
	unsigned int width, 
	unsigned int height) : title(title), posX(x), posY(y), width(width), height(height) {} 


EditorSceneHierarchyWidget::EditorSceneHierarchyWidget(){}

EditorSceneHierarchyWidget::~EditorSceneHierarchyWidget() {}

void EditorSceneHierarchyWidget::Draw(EditorInspectorWidget *inspector)
{
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiSetCond_FirstUseEver);

	ImGui::Begin(title.c_str());
	
	for (GameObject *go : App->scene->gameObjects)
	{
		DrawNode(go);
	}

	inspector->Draw(selectedGO);

	ImGui::End();
}

void EditorSceneHierarchyWidget::DrawNode(GameObject *go) 
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_OpenOnDoubleClick | (go == selectedGO ? ImGuiTreeNodeFlags_Selected : 0);

	bool isLeaf = go->GetChildrenGO().size() == 0;

	if (isLeaf)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	bool isNodeOpen = ImGui::TreeNodeEx(go, nodeFlags, go->GetName().c_str());
	if (ImGui::IsItemClicked())
	{
		if (selectedGO)
		{
			ComponentGizmo *lastGizmo = (ComponentGizmo*)selectedGO->FindComponentByType(ComponentType::GIZMO);
			//lastGizmo->Disable();   pinto todos los gizmos
		}
		
		selectedGO = go;
		ComponentGizmo *currentGizmo = (ComponentGizmo*)selectedGO->FindComponentByType(ComponentType::GIZMO);
		currentGizmo->Enable();
	}

	if (isNodeOpen && !isLeaf)
	{
		for (GameObject *child_go : go->GetChildrenGO())
		{
			DrawNode(child_go);
		}

		ImGui::TreePop();
	}
}
