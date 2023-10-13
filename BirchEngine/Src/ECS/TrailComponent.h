#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include "AssetManager.h"
#include "../Game.h"

class TrailComponent : public Component
{
public:

	void init() override
	{
	}

	TrailComponent(float tTimeToDelete)
	{
		timeToDelete = tTimeToDelete;
		currentTime = 0;
	}

	void update() override
	{
		if (currentTime < timeToDelete)
		{
			currentTime += 3; 
		}
		else
		{
			entity->destroy();
			//std::cout << "destroyed trail" << std::endl;
		}
	}


private:
	float timeToDelete;
	float currentTime;
};