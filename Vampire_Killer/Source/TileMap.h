#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles

	//Falta DOOR!

	BLOCK = 1, BLOCK_LEFT = 2, BLOCK_RIGHT = 3, STAIRS_LEFT = 4, STAIRS_RIGHT = 5, GRASS_BLOCK = 45,
	CHEST = 6,

	STAIRS_BOSS = 60,
	
	WHIP = 15, AXE = 16, KNIFE = 17,
	BLUE_CROSS = 18, SILVER_CROSS = 19, GOLDEN_CROSS = 20,
	SMALL_KEY = 21, BIG_KEY = 22,
	WHITE_MONEY = 23, BLUE_MONEY = 24,
	HOURGLASS = 25, HOLY_WATER = 26, STAFF = 27,
	BLUE_ORB = 31,
	BLUE_WATER = 32,
	BOOTS = 33, RED_SHIELD = 34, GOLDEN_SHIELD = 35,
	BLUE_RING = 36,
	BLACK_BIBLE = 40, WHITE_BIBLE = 41,
	WINGS = 42,
	PARCHMENT = 43,
	

	HEART_SMALL = 10, HEART_BIG = 11, HEART_PLAIN = 12,
	CANDLE_BIG = 7, CANDLE_SMALL = 8, FIRE_1 = 13, FIRE_2 = 14,
	BOSS_ITEM1 = 28, BOSS_ITEM2 = 29, BOSS_ITEM3 = 30,
	OBJECT_ANIM1 = 37, OBJECT_ANIM2 = 38, OBJECT_ANIM3 = 39,
	BOSS_ATTACK = 44,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	//Intervals

	STATIC_FIRST = BLOCK,
	STATIC_LAST = CHEST,
	SOLID_FIRST = BLOCK,
	SOLID_LAST = STAIRS_BOSS,
	SPECIAL_FIRST = WHIP,
	SPECIAL_LAST = BOSS_ATTACK,

	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;

	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int* py) const;

	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;

	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	// TestOnLadder(const AABB& box, int* px) const;

	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	//bool TestOnLadderTop(const AABB& box, int* px) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	//bool IsTileLadderTop(Tile tile) const;
	//bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	//int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_tiles;
};