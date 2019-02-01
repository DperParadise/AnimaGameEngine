#ifndef __COMPONENT_ED_CAM_H__
#define __COMPONENT_ED_CAM_H__

#include "ComponentCamera.h"

class GameObject;

class ComponentEditorCamera : public ComponentCamera
{
public:
	ComponentEditorCamera(ComponentType type, GameObject *ownerGO);

	 void Update(float dt) override;
};

#endif
