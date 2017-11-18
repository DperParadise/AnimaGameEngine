#include "EditorConsoleWidget.h"

EditorConsoleWidget::EditorConsoleWidget(const std::string &title,
	int x,
	int y,
	unsigned int width,
	unsigned int height) : title(title), pos_x(x), pos_y(y), width(width), height(height) {}


EditorConsoleWidget::EditorConsoleWidget() {}

EditorConsoleWidget::~EditorConsoleWidget() {}

void EditorConsoleWidget::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);	
	ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiSetCond_FirstUseEver);	

	ImGui::Begin(title.c_str());

	if (ImGui::Button("clear"))
	{
		buff.clear();
	}

	ImGui::BeginChild("Log", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(buff.begin());
	if (scroll_to_bottom)
	{
		ImGui::SetScrollHere(1.0f);
		scroll_to_bottom = false;
	}
	ImGui::EndChild();
	ImGui::End();

}



