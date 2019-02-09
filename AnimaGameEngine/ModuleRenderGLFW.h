#ifndef __MODULERENDER__
#define __MODULERENDER__

#include "Module.h"

class ModuleRenderGLFW : public Module
{
public:

	 bool Init(Config *config = nullptr);
	 update_status PreUpdate(float dt) override;
	 update_status PostUpdate(float dt) override;
};

#endif
