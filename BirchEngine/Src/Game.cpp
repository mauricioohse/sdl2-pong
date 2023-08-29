#include "Game.h"
#include "TextureManager.h"
#include "map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>


Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& enemy(manager.addEntity());

auto& label(manager.addEntity());
auto& labelEnemy(manager.addEntity());

AssetManager* Game::assets = new AssetManager(&manager);


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	// these are the assets that can be loaded by their string id
	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("enemy", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/bar_sprites.png");
	assets->AddTexture("ball", "assets/fireball.png");
	assets->AddFont("arial", "assets/arial.ttf", 16);

	map = new Map("terrain", 3, 32);

	// ecs implementation

	map->LoadMap("assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(0, 320 - 64, 64, 16, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(GROUP_PLAYERS);

	enemy.addComponent<TransformComponent>(800 - 32, 320 - 64, 64, 16, 2);
	enemy.addComponent<SpriteComponent>("player", true);
	//enemy.addComponent<EnemyController>();
	enemy.addComponent<ColliderComponent>("player");
	enemy.addGroup(GROUP_ENEMIES);



	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "Test string", "arial", white);
	labelEnemy.addComponent<UILabel>(10, 26, "Test string", "arial", white);

	assets->CreateProjectiles(Vector2D(400, 320), Vector2D(-1, 0.2), 0, 2, "ball");

}

// list of entities per group
auto& tiles(manager.getGroup(Game::GROUP_MAP));
auto& players(manager.getGroup(Game::GROUP_PLAYERS));
auto& enemies(manager.getGroup(Game::GROUP_ENEMIES));
auto& colliders(manager.getGroup(Game::GROUP_COLLIDERS));
auto& projectiles(manager.getGroup(Game::GROUP_PROJECTILES));


void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update() // currently doing things here to test, but the scripts will change places
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	Vector2D playerVel = player.getComponent<TransformComponent>().velocity;
	Vector2D enemyPos = enemy.getComponent<TransformComponent>().position;
	Vector2D enemyVel = enemy.getComponent<TransformComponent>().velocity;
	cnt++;

	std::stringstream ss;


	// text displayed
	ss << "Player pos/speed: " << playerPos << "/" << playerVel;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

	std::stringstream ss2;
	ss2 << "Enemy pos/speed: " << enemyPos << "/" << enemyVel;
	labelEnemy.getComponent<UILabel>().SetLabelText(ss2.str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	// projectile logic
	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			p->getComponent<ProjectileComponent>().DoHorizontalCollision(player.getComponent<TransformComponent>().position.y);
			//p->getComponent<TransformComponent>().velocity = -p->getComponent<TransformComponent>().velocity;
		}

		if (Collision::AABB(enemy.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			//todo: clear this mess. this should be overloaded - on vector2d, not sure why it didnt work
			p->getComponent<TransformComponent>().velocity = -p->getComponent<TransformComponent>().velocity;
		}

		// oponent follow the ball
		if (p->getComponent<TransformComponent>().position.y - 64 > enemy.getComponent<TransformComponent>().position.y)
			enemy.getComponent<TransformComponent>().velocity.y = .8;
		else if (p->getComponent<TransformComponent>().position.y - 64 < enemy.getComponent<TransformComponent>().position.y)
			enemy.getComponent<TransformComponent>().velocity.y = -.8;
		else
			enemy.getComponent<TransformComponent>().velocity.y = 0;
	}

	Vector2D pVel = player.getComponent<TransformComponent>().velocity;
	int pSpeed = player.getComponent<TransformComponent>().speed;

	if (player.getComponent<TransformComponent>().position.y <= 0)
		player.getComponent<TransformComponent>().position.y = 0;
	if (player.getComponent<TransformComponent>().position.y >= 640 - 128)
		player.getComponent<TransformComponent>().position.y = 640-128;
	//camera.x = player.getComponent<TransformComponent>().position.x - 400;
	//camera.y = player.getComponent<TransformComponent>().position.y - 320;

	
	

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.w;


	for (auto t : tiles)
	{
		t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
		t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);

	}


	//std::cout << cnt << std::endl;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& c : colliders)
	{
		c->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	for (auto& e : enemies)
	{
		e->draw();
	}

	label.draw();
	labelEnemy.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

