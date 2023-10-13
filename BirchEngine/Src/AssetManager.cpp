#include "AssetManager.h"
#include "ECS\Components.h"
#include "ECS\ECS.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectiles(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, BALL_WIDTH, BALL_WIDTH, 1);
	projectile.addComponent<SpriteComponent > (id, false);
	projectile.addComponent<ColliderComponent>(id);
	
	Vector2D randInitVel = ProjectileComponent::GenInitVelocity();
	projectile.addComponent<ProjectileComponent>(range, speed, randInitVel);
	projectile.addGroup(Game::GROUP_PROJECTILES);
}

void AssetManager::CreateTrail(Vector2D pos, int timeToDelete, std::string id)
{
	auto& trail(manager->addEntity());
	trail.addComponent<TransformComponent>(pos.x, pos.y, BALL_WIDTH, BALL_WIDTH, 1);
	trail.addComponent<SpriteComponent>(id);
	trail.addComponent<TrailComponent>(timeToDelete);
	trail.addGroup(Game::GROUP_SPRITES);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path)); // LoadTexture loads the texture and returns the SDL_Texture pointer to be stored in textures
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}
