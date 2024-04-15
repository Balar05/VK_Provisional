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

	BLOCK = 1, BLOCK_LEFT = 2, BLOCK_RIGHT = 3, STAIRS_LEFT = 4, STAIRS_RIGHT = 5,
	HEART_SMALL = 10, HEART_BIG = 11, HEART_PLAIN = 12,
	CHEST = 6,
	CANDLE_BIG = 7, CANDLE_SMALL = 8, FIRE_1 = 13, FIRE_2 = 14,
	WHIP = 15, AXE = 16, KNIFE = 17,
	BLUE_CROSS = 18, SILVER_CROSS = 19, GOLDEN_CROSS = 20,
	SMALL_KEY = 21, BIG_KEY = 22,
	WHITE_MONEY = 23, BLUE_MONEY = 24,
	HOURGLASS = 25, HOLY_WATER = 26, STAFF = 27,
	BOSS_ITEM1 = 28, BOSS_ITEM2 = 29, BOSS_ITEM3 = 30,
	BLUE_ORB = 31,
	BLUE_WATER = 32,
	BOOTS = 33, RED_SHIELD = 34, GOLDEN_SHIELD = 35,
	BLUE_RING = 36,
	OBJECT_ANIM1 = 37, OBJECT_ANIM2 = 38, OBJECT_ANIM3 = 39,
	BLACK_BIBLE = 40, WHITE_BIBLE = 41,
	WINGS = 42,
	PARCHMENT = 43,
	BOSS_ATTACK = 44,
	STAIRS_BOSS = 60,


	/*BLOCK_SQUARE1_TL = 1, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
	BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
	BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
	BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	BLOCK_BEAM_L, BLOCK_BEAM_R,*/
	/*LADDER_L = 20, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
	LOCK_RED = 30, LOCK_YELLOW,
	LASER_L = 40, LASER_R,*/

	// 50 <= id < 100: special tiles
	/*DOOR = 50,
	KEY_RED = 60, YELLOW_KEY, ITEM_APPLE, ITEM_CHILI,
	LASER = 70, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,*/

	// id >= 100: entities' initial locations
	PLAYER = 100,

	//Intervals

	/*STATIC_FIRST = BLOCK_SQUARE1_TL,
	STATIC_LAST = LASER_R,
	SOLID_FIRST = BLOCK_SQUARE1_TL,
	SOLID_LAST = BLOCK_BEAM_R,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = LASER,*/

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
	bool TestOnLadder(const AABB& box, int* px) const;

	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile* map;

	//Size of the tile map
	int size, width, height;

	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Tile sheet
	const Texture2D* img_tiles;
};