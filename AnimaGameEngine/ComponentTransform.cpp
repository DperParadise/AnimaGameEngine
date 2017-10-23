#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	//TODO:initialize with quaternion identity

}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Update() {}

void ComponentTransform::Enable() {}

void ComponentTransform::Disable() {}
