#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
	bool delete_entity_flag = false;

public:

	int animIndex = 0;
	std::map<const char*, Animation > animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		SetTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");
		SetTex(id);
	}

	~SpriteComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width; // in the future, this has to be changed based on the particular asset/png size.
		srcRect.h = transform->height;
		destRect.w = srcRect.w * 2;
		destRect.h = srcRect.h * 2;

	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;

	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void SetTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

};
