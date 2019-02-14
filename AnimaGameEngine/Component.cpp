#include "Component.h"

Component::Component(ComponentType type, GameObject *owner, bool act) : type(type), ownerGO(owner), enabled(act) {}

Component::~Component() {}

void Component::Update(float dt) {}

void Component::Enable()
{
	enabled = true;
}
void Component::Disable()
{
	enabled = false;
}

ComponentType Component::GetComponentType() const
{
	return type;
}

bool Component::IsEnabled() const
{
	return enabled;
}

//Returns the gameobject this component is attached to.
 GameObject * Component::GetOwnerGO()
{
	return ownerGO;
}
