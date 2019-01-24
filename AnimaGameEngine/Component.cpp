#include "Component.h"

Component::Component(ComponentType type, const std::string &name, bool act, GameObject *go) : comp_type(type), comp_name(name), active(act), owner_go(go) {}

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