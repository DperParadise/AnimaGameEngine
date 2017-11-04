#include "ModuleEditorGUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"

ModuleEditorGUI::ModuleEditorGUI() {}

ModuleEditorGUI::~ModuleEditorGUI() {}

bool ModuleEditorGUI::Init(Config *config)
{
	MYLOG("Init IMGUI");
	ImGui_ImplSdlGL3_Init(App->window->window);

	return true;
}

update_status ModuleEditorGUI::Update(float dt)
{
	ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}

