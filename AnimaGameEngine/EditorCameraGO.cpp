#include "EditorCameraGO.h"

EditorCameraGO::EditorCameraGO(const std::string &name) : GameObject(name)
{
	AddEditorCameraComponent();
}