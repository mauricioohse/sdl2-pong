#pragma once
#include <string>

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	//void Map::AddTile(int srcX, int srcY, int xpos, int ypos);

private:
	std::string textID;
	int mapScale;
	int tileSize;
	int scaledSize;

};

