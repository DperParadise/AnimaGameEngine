#include "ComponentBehaviour.h"
#include "Application.h"
#include "ModuleInput.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "GameObject.h"

ComponentBehaviour::ComponentBehaviour(const std::string &behav_name, const std::string &name, bool act, GameObject *go) : behav_name(behav_name), Component(name, act, go){}

ComponentBehaviour::~ComponentBehaviour() {}

void ComponentBehaviour::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		aiVector3D pos = owner_go->transform.local_position;

		aiVector3D dir = aiVector3D(owner_go->transform.forward.x * speed * dt,
			owner_go->transform.forward.y * speed * dt,
			owner_go->transform.forward.z * speed * dt);

		owner_go->transform.Translate(dir.x, dir.y, dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		aiVector3D pos = owner_go->transform.local_position;

		aiVector3D dir = aiVector3D(owner_go->transform.forward.x * -speed * dt,
			owner_go->transform.forward.y * -speed * dt,
			owner_go->transform.forward.z * -speed * dt);

		owner_go->transform.Translate(dir.x, dir.y, dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		aiVector3D pos = owner_go->transform.local_position;

		aiVector3D dir = aiVector3D(owner_go->transform.left.x * speed * dt,
			owner_go->transform.left.y * speed * dt,
			owner_go->transform.left.z * speed * dt);

		owner_go->transform.Translate(dir.x, dir.y, dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		aiVector3D pos = owner_go->transform.local_position;

		aiVector3D dir = aiVector3D(owner_go->transform.left.x * -speed * dt,
			owner_go->transform.left.y * -speed * dt,
			owner_go->transform.left.z * -speed * dt);

		owner_go->transform.Translate(dir.x, dir.y, dir.z);
	}


	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT)
	{
		owner_go->transform.Rotate(0.0f, speed * dt, 0.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
	{
		owner_go->transform.Rotate(0.0f, -speed * dt, 0.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		aiVector3D scale = owner_go->transform.local_scale;
		scale.x += scale_speed * dt;
		scale.y += scale_speed * dt;
		scale.z += scale_speed * dt;

		owner_go->transform.Scale(scale.x, scale.y, scale.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
	{
		aiVector3D scale = owner_go->transform.local_scale;
		scale.x -= scale_speed * dt;
		scale.y -= scale_speed * dt;
		scale.z -= scale_speed * dt;

		owner_go->transform.Scale(scale.x, scale.y, scale.z);
	}
}

void ComponentBehaviour::Enable()
{
	active = true;
}

void ComponentBehaviour::Disable()
{
	active = false;
}