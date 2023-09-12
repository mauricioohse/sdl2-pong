#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Types.h"

class ColliderComponent;
class AssetManager;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	//static void AddTile(int srcX, int srcY, int xpos, int ypos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager* assets;
	static int playerPoints;
	static int enemyPoints;

	enum groupLabels : std::size_t
	{
		GROUP_MAP,
		GROUP_PLAYERS,
		GROUP_ENEMIES,
		GROUP_COLLIDERS,
		GROUP_PROJECTILES
	};

private:
	int cnt = 0;
	SDL_Window *window;
};