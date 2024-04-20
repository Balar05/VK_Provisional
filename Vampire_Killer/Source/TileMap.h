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

	// 0 < id < 10: static tiles
	BLOCK_LEFT = 1, BLOCK_RIGHT = 2, BLOCK = 3, GRASS = 4,
	CHEST = 5,
	STAIRS_LEFT = 6, STAIRS_RIGHT = 7, STAIRS_BOSS = 8,
	

	// 10 < id < 45: objects

	WHIP, AXE, KNIFE, BLUE_CROSS, HOURGLASS, HOLY_WATER,
	STAFF, MONEY_WHITE, MONEY_BLUE, BLUE_ORB, BLUE_WATER, BOOTS,
	RED_SHIELD, GOLDEN_SHIELD, BLUE_RING, BLACK_BIBLE, WHITE_BIBLE, WINGS,
	SILVER_CROSS, GOLDEN_CROSS, PARCHMENT,
	FIRE1, FIRE2,
	CANDLE_BIG, CANDLE_SMALL,
	OBJECT_ANIM1, OBJECT_ANIM2, OBJECT_ANIM3,
	BOSS_ITEM1, BOSS_ITEM2, BOSS_ITEM3, BOSS_ATTACK,
	GOLDEN_KEY, SILVER_KEY,
	SMALL_HEART, BIG_HEART, PLAIN_HEART,

	// id >= 100: entities' initial locations
	PLAYER = 100,

	//Intervals
	/*STATIC_FIRST = BLOCK_SQUARE1_TL,
	STATIC_LAST = LASER_R,
	SOLID_FIRST = BLOCK_SQUARE1_TL,
	SOLID_LAST = BLOCK_BEAM_R,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = LASER,*/
	STATIC_FIRST = BLOCK_LEFT,
	STATIC_LAST = CHEST,
	SOLID_FIRST = BLOCK_LEFT,
	SOLID_LAST = CHEST,
	STAIR_FIRST = STAIRS_LEFT,
	STAIR_LAST = STAIRS_BOSS,
	SPECIAL_FIRST = WHIP,
	SPECIAL_LAST = PLAIN_HEART,
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
	//bool TestOnLadder(const AABB& box, int* px) const;

	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	//bool TestOnLadderTop(const AABB& box, int* px) const;

	bool TestOnStair(const AABB& box) const;
	Tile GetTileIndex(int x, int y) const;
	bool IsTileStair(Tile tile) const;

private:
	void InitTileDictionary();
	bool IsTileSolid(Tile tile) const;
	//bool IsTileLadderTop(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	//int GetLadderCenterPos(int pixel_x, int pixel_y) const;


	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite* laser;

	//Tile sheet
	const Texture2D* img_tiles;
};