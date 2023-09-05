#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include "AssetManager.h"

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

		if (distance > range && range!=0) // this range stuff is not needed for pong - will be deprecated eventually
		{
			entity->destroy();
			std::cout << "Ball destroyed bcs of range!" << std::endl;
		}
		else if (transform->position.y > Game::camera.y + Game::camera.h - 32|| // 32 == ball_height
			transform->position.y < Game::camera.y)
		{	// ball at lower or upper bound
			transform->velocity.y = -transform->velocity.y;
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w)
		{	// right bound > player scored
			entity->destroy();
			std::cout << "Player scored" << std::endl;
			Game::assets->CreateProjectiles(Vector2D(400, 320), Vector2D(-1, 0.2), 0, 2, "ball");
		}
		else if (transform->position.x < Game::camera.x)
		{
			entity->destroy();
			std::cout << "Enemy scored" << std::endl;
			Game::assets->CreateProjectiles(Vector2D(400, 320), Vector2D(-1, 0.2), 0, 2, "ball");
		}
	}

	void DoHorizontalCollision(float YPosition)
	{

		std::cout << "DoHorizontalCollision, Ybar: " << YPosition << " yvelocity: " << transform->velocity.y <<
			" plus: " << ((transform->position.y - YPosition - 64) / Y_VELOCITY_FACTOR) << std::endl;

		if (transform->velocity.x < 0) // ball going to the left
			transform->velocity.x = -transform->velocity.x + X_VELOCITY_INCREASE_PER_HIT;
		else // ball going to the right
			transform->velocity.x = -transform->velocity.x - X_VELOCITY_INCREASE_PER_HIT;

		// vertical
		transform->velocity.y += (transform->position.y - YPosition - 64) / Y_VELOCITY_FACTOR;
	}

	static Vector2D GenInitVelocity()
	{
		while (true)
		{
			Vector2D randInitVel = Vector2D((float)(rand() % 150) / 100, (float)(rand() % 250 - 125) / 100);
			if (randInitVel.x > 0.5)
				return randInitVel;
		}
	}


private:

	TransformComponent* transform;
	const float X_VELOCITY_INCREASE_PER_HIT = .3;
	const int Y_VELOCITY_FACTOR = 64; // makes the velocity change on y be up to 2 units (128/64)
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};
