#include "Component.h"

Component::Component(ComponentType type, const std::string &name, GameObject *owner, bool act) : type(type), name(name), ownerGO(owner), active(act) {}

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

const std::string & Component::GetComponentName() const
{
	return name;
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
