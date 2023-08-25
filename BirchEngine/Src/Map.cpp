#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

extern Manager manager;

Map::Map(std::string tID, int ms, int ts) : textID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	char c; // for collider
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;


	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);
			srcY = atoi(&tile) * tileSize;
			mapFile.get(tile);
			srcX = atoi(&tile) * tileSize;
			AddTile(srcX, srcY, x * tileSize * mapScale, y * tileSize * mapScale);
			mapFile.ignore(); // ignores the comma
		}
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '2') // 21 is the full water texture -> that is because I am using the same map of collision as the tilemap for ease
			{ // first char is 2
				mapFile.get(c);
				if (c == '1')
				{
					auto& tcol(manager.addEntity());
					tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
					tcol.addGroup(Game::GROUP_COLLIDERS);
					std::cout << "COLLISION TILE ADDED!" << std::endl;
				}
				mapFile.ignore();
			}
			else
			{	// ignores twice otherwise
				mapFile.ignore();
				mapFile.ignore();
				std::cout << "COLLISION TILE IGNORED!" << std::endl;
			}
		}
	}

	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textID);
	tile.addGroup(Game::GROUP_MAP);
}

