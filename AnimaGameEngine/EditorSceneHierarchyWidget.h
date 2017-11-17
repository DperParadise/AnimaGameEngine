#ifndef __EDITOR_SCENE_HIERARCHY_WIDGET_H__
#define __EDITOR_SCENE_HIERARCHY_WIDGET_H__


class GameObject;
class EditorSceneHierarchyWidget
{
public:
	EditorSceneHierarchyWidget(const char* title, int x, int y, unsigned int width, unsigned int height);
	EditorSceneHierarchyWidget();
	~EditorSceneHierarchyWidget();

	void Draw();
	void DrawNode(GameObject *go, void* &selected_go);

	const char* title = "Scene Hierarchy";
	int pos_x = 0, pos_y = 0;
	unsigned int width = 200, height = 400;
	
};




#endif // !__EDITOR_SCENE_HIERARCHY_WIDGET_H__
