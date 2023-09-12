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

	mapFile.close();
}

//void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
//{
//	auto& tile(manager.addEntity());
//	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, textID);
//}

