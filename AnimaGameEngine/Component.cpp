#include "Component.h"

Component::Component(const std::string &name, bool act, GameObject *go) : comp_name(name), active(act), owner_go(go) {}

Component::~Component() {}
