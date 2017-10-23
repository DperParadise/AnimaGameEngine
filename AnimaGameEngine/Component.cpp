#include "Component.h"

Component::Component(component_type t, bool act, GameObject *go) : type(t), active(act), owner_go(go) {}

Component::~Component() {}
