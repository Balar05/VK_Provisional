#pragma once

class LevelBackground
{
private:
	Texture2D backgroundImage; //= LoadTexture("images/Sprites/256x176 Levels.png");
public:
	LevelBackground();
	//~LevelBackground();
	void RenderBackground(int stage);
	void Release();
};

//#pragma once
//#include "Entity.h"
//
////#define OBJECT_PHYSICAL_SIZE	14
////#define OBJECT_FRAME_SIZE		16
//
////#define POINTS_APPLE	10
////#define POINTS_CHILI	20
//
//enum class /*ObjectType*/ LevelNum { 
//	AIR, LEVEL1, LEVEL2, LEVEL3
//};
//
//class /*Object*/ Level : public Entity
//{
//public:
//	/*Object*/ Level(const Point& p, LevelNum n);
//	/*~Object();*/ ~Level();
//
//	//void DrawDebug(const Color& col) const;
//	//int Points() const;
//
//private:
//	LevelNum num;
//};


//#pragma once
//#include <raylib.h>
//#include "Sprite.h"
//#include "Point.h"
//#include "AABB.h"
//#include "Globals.h"
//#include <unordered_map>
//
//enum class Level {
//	AIR = 0,
//	LEVEL1, LEVEL2, LEVEL3
//};
//
//class LevelMap
//{
//public:
//	LevelMap();
//	~LevelMap();
//
//	AppStatus Initialise();
//	AppStatus Load(int data[], int w, int h);
//	void Update();
//	void Render();
//	void Release();
//
//private:
//	void InitLevelDictionary();
//
//	Level GetLevelIndex(int x, int y) const;
//
//
//	//Tile map
//	//Tile* map;
//	Level* background;
//
//	//Size of the tile map
//	int size, width, height;
//
//	//Dictionary of tile frames
//	std::unordered_map<int, Rectangle> dict_rect;
//
//	//Tile sheet
//	const Texture2D* img_level;
//};