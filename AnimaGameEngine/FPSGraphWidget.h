#ifndef __FPS_GRAPH_H__
#define __FPS_GRAPH_H__

#include <vector>

class FPSGraphWidget
{
public:
	FPSGraphWidget(const char* title, int x, int y, unsigned int width, unsigned int height);
	FPSGraphWidget();
	~FPSGraphWidget();

	void FillDraw(float dt);

	std::vector<float> fps_vec = { 0 };
	std::vector<float> ms_vec;


	const char *widget_title = "Application Performance";
	int pos_x = 0, pos_y = 0;
	unsigned int width = 100, height = 100;

	const char *title_fps = "Framerate: ";
	const char *title_ms = "Milliseconds: ";

private:
	
	void Draw(float dt);
	void Reset();
	unsigned frames = 0;
	 
};



#endif
