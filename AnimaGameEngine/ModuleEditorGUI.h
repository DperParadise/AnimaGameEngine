#ifndef __MODULE_EDITOR_GUI_H__
#define __MODULE_EDITOR_GUI_H__

#include "Module.h"
#include "libraries/ImGui/imconfig.h"
#include "libraries/ImGui/imgui.h"
#include "libraries/ImGui/imgui_impl_sdl_gl3.h"
#include "libraries/ImGui/imgui_internal.h"
#include "libraries/ImGui/stb_rect_pack.h"
#include "libraries/ImGui/stb_textedit.h"
#include "libraries/ImGui/stb_truetype.h"
#include "Globals.h"

class Congif;
class ModuleEditorGUI : public Module
{
public:
	ModuleEditorGUI();
	~ModuleEditorGUI();

	bool Init(Config *config);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

};





#endif