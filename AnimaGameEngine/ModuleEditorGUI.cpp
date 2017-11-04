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

update_status ModuleEditorGUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditorGUI::Update(float dt)
{
	ImGui::ShowTestWindow();
	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEditorGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditorGUI::CleanUp()
{
	MYLOG("Shutdown IMGUI");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

