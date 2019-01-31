#include "Component.h"

Component::Component(ComponentType type, GameObject *owner, bool act) : type(type), ownerGO(owner), active(act) {}

Component::~Component() {}

void Component::Update(float dt) {}

void Component::Enable()
{
	active = true;
}
void Component::Disable()
{
	active = false;
}

ComponentType Component::GetComponentType() const
{
	return type;
}

bool Component::IsActive() const
{
	return active;
}

//Returns the gameobject this component is attached to.
const GameObject * Component::GetOwnerGO() const
{
	return ownerGO;
}
