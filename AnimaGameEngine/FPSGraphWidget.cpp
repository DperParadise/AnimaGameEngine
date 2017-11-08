#include "FPSGraphWidget.h"
#include "libraries/ImGui/imgui.h"
#include "Globals.h"
#include <string>

FPSGraphWidget::FPSGraphWidget(const char* title,
	int x,
	int y,
	unsigned int width,
	unsigned int height) : widget_title(title), pos_x(x), pos_y(y), width(width), height(height) {}

FPSGraphWidget::FPSGraphWidget() {}

FPSGraphWidget::~FPSGraphWidget() {}

void FPSGraphWidget::FillDraw(float dt)
{
	Draw(dt);

	frames++;

	if (frames >= 180);
		Reset();

	fps_vec.push_back(1/dt);
}

void FPSGraphWidget::Draw(float dt)
{
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiSetCond_FirstUseEver);

	ImGui::Begin(widget_title);
	std::string framerate = "Framerate ";
	framerate += std::to_string(1 / dt);
	ImGui::PlotHistogram("", fps_vec.data(), fps_vec.size(), 0,  framerate.c_str(), 0, 80, ImVec2(0, 80));
	ImGui::End();

}

void FPSGraphWidget::Reset()
{
	frames = 0;
	fps_vec.clear();
}