#ifndef __EDITOR_CAM_GO_H__
#define __EDITOR_CAM_GO_H__

#include "GameObject.h"
#include <string>

class EditorCameraGO : public GameObject
{
public:
	EditorCameraGO(const std::string &name);
};

#endif
