/*
#ifndef __EDITOR_SCENE_HIERARCHY_WIDGET_H__
#define __EDITOR_SCENE_HIERARCHY_WIDGET_H__

#include <string>

class GameObject;
class EditorInspectorWidget;
class EditorSceneHierarchyWidget
{
public:
	EditorSceneHierarchyWidget(const std::string &title, int x, int y, unsigned int width, unsigned int height);
	EditorSceneHierarchyWidget();
	~EditorSceneHierarchyWidget();

	void Draw(EditorInspectorWidget *inspector);
	void DrawNode(GameObject *go, void* &selected_go) const;

	std::string title = "Scene Hierarchy";
	int pos_x = 0, pos_y = 0;
	unsigned int width = 200, height = 400;
	
};




#endif // !__EDITOR_SCENE_HIERARCHY_WIDGET_H__
*/