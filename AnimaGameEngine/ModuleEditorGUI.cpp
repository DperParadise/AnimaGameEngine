
#include "ModuleEditorGUI.h"
#include "Application.h"
#include "ModuleWindowGLFW.h"
#include "Globals.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ModuleEditorGUI::ModuleEditorGUI() {}

ModuleEditorGUI::~ModuleEditorGUI() {}

bool ModuleEditorGUI::Init(Config *config)
{
	MYLOG("Init IMGUI");
	
	const char* glsl_version = "#version 130";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(App->window->window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return true;
}


bool ModuleEditorGUI::Start()
{
	return true;
}

update_status ModuleEditorGUI::PreUpdate(float dt)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleEditorGUI::Update(float dt)
{
	ImGui::ShowTestWindow();
	fps_graph.Draw(dt);
	console.Draw();
	scene_hierarchy.Draw(&inspector);
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleEditorGUI::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditorGUI::CleanUp()
{
	MYLOG("Shutdown IMGUI");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}

