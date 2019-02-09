#ifndef MODULEINPUT
#define MODULEINPUT

#include "Module.h"
#include "libraries/glm/glm.hpp"

struct GLFWwindow;

class ModuleInputGLFW : public Module
{
public:
	ModuleInputGLFW();
	
	bool Init(Config *config = nullptr) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	
	//callbacks
	static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	
	bool KeyPressed(int key) const;
	bool MouseButtonPressed(int button) const;
	const glm::vec2& GetMouseOffset() const;
	double GetScrollOffset() const;

private:
	static glm::vec2 lastMousePos;
	static glm::vec2 mouseOffset;
	static bool firstMouseInteraction;
	static double scrollOffsetY;

};

#endif
