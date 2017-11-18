#include "FPSGraphWidget.h"
#include "libraries/ImGui/imgui.h"
#include "Globals.h"

FPSGraphWidget::FPSGraphWidget(const std::string &title,
	int x,
	int y,
	unsigned int width,
	unsigned int height) : title(title), pos_x(x), pos_y(y), width(width), height(height) 
{
	memset(histogram, 0.0f, sizeof(histogram));
	memset(ms_array, 0.0f, sizeof(ms_array));
}

FPSGraphWidget::FPSGraphWidget() 
{
	memset(histogram, 0.0f, sizeof(histogram));
	memset(ms_array, 0.0f, sizeof(ms_array));
}

FPSGraphWidget::~FPSGraphWidget() {}

void FPSGraphWidget::Draw(float dt)
{
	frames++;

	if (frames == REFRESH_FRAMES)
	{
		for (int i = 1; i < NUM_COLS; i++)
		{
			histogram[i - 1] = histogram[i];
			ms_array[i - 1] = ms_array[i];
		}
		histogram[NUM_COLS - 1] = 1/dt;
		ms_array[NUM_COLS - 1] = 1000 * dt;
		
		frames = 0;
	}

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiSetCond_FirstUseEver);

	ImGui::Begin(title.c_str());
	
	std::string title_hist = std::to_string(histogram[NUM_COLS - 1]);
	std::string title_ms = std::to_string(ms_array[NUM_COLS - 1]);
	
	ImGui::PlotHistogram(framerate.c_str(), histogram, sizeof(histogram)/sizeof(histogram[0]), 0, title_hist.c_str(), 0, 100, ImVec2(0, 40));
	ImGui::PlotHistogram(milliseconds.c_str(), ms_array, sizeof(ms_array) / sizeof(ms_array[0]), 0, title_ms.c_str(), 0, 30, ImVec2(0, 40));

	ImGui::End();
}

