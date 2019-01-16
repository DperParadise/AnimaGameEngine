#include <windows.h>
#include <stdio.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleEditorGUI.h"
#include "EditorConsoleWidget.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	
	//TODO: Commented while adding support to opengl 3.3
	/*
	if (App)
	{
		App->editor_gui->console.buff.appendv(tmp_string2, ap);
		App->editor_gui->console.scroll_to_bottom = true;
	}
	*/
}