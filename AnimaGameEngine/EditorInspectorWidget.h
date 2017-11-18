#ifndef __EDITOR_INSPECTOR_WIDGET_H__
#define __EDITOR_INSPECTOR_WIDGET_H__

#include <string>

class GameObject;
class EditorInspectorWidget
{
public:
	EditorInspectorWidget(const std::string &title, int x, int y, unsigned int width, unsigned int height);
	EditorInspectorWidget();
	~EditorInspectorWidget();

	void Draw(GameObject *go = nullptr);
	
	std::string title = "Inspector";
	int pos_x = 200, pos_y = 0;
	unsigned int width = 200, height = 400;

private:
	void ShowHelpMarker(const char* desc);
	

};
#endif // !__EDITOR_INSPECTOR_WIDGET_H__

