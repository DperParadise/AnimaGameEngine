#include "ModuleEditorGUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"

#include "libraries/ImGui/imgui_impl_sdl_gl3.h"
#include "libraries/ImGui/imgui.h"

ModuleEditorGUI::ModuleEditorGUI() {}

ModuleEditorGUI::~ModuleEditorGUI() {}

bool ModuleEditorGUI::Init(Config *config)
{
	MYLOG("Init IMGUI");
	ImGui_ImplSdlGL3_Init(App->window->window);

	return true;
}


bool ModuleEditorGUI::Start()
{
	return true;
}

update_status ModuleEditorGUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditorGUI::Update(float dt)
{
	//ImGui::ShowTestWindow();
	console.Draw();
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

