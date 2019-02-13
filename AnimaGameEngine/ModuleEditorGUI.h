#ifndef __MODULE_EDITOR_GUI_H__
#define __MODULE_EDITOR_GUI_H__

#include "Module.h"
#include "Globals.h"
#include "EditorConsoleWidget.h"
#include "FPSGraphWidget.h"
#include "EditorSceneHierarchyWidget.h"
#include "EditorInspectorWidget.h"

class Config;
class ModuleEditorGUI : public Module
{
public:
	ModuleEditorGUI();
	~ModuleEditorGUI();

	bool Init(Config *config);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	EditorConsoleWidget console;
	FPSGraphWidget fps_graph;
	EditorSceneHierarchyWidget scene_hierarchy;
	EditorInspectorWidget inspector;

};

#endif
