#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}

	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range && range!=0)
		{
			entity->destroy();
			std::cout << "Ball destroyed bcs of range!" << std::endl;
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||// verifies if outside of screen
			transform->position.x < Game::camera.x ||
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y
			)
		{
			entity->destroy();
			std::cout << "Ball destroyed bcs of out of bounds!" << std::endl;
		}
	}

private:

	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};
