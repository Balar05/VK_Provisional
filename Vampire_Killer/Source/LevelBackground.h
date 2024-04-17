#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Level {
	AIR = 0,
	LEVEL1, LEVEL2, LEVEL3
};

class LevelMap
{
public:
	LevelMap();
	~LevelMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

private:
	void InitLevelDictionary();

	Level GetLevelIndex(int x, int y) const;


	//Tile map
	//Tile* map;
	Level* background;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_level;
};