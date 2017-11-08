#ifndef __EDITOR_CONSOLE_WIDGET_H__
#define __EDITOR_CONSOLE_WIDGET_H__

#include "libraries/ImGui/imgui.h"

class EditorConsoleWidget
{
public:
	EditorConsoleWidget(const char* title, int x, int y, unsigned int width, unsigned int height);
	EditorConsoleWidget();
	~EditorConsoleWidget();

	void Draw();

	const char* title = "Console";
	int pos_x = 100, pos_y = 0;
	unsigned int width = 100, height = 100;
	ImGuiTextBuffer buff;
	bool scroll_to_bottom = false;
};


#endif
