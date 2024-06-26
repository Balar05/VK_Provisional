#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	//laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK_LEFT] = { 0,  0, n, n };
	dict_rect[(int)Tile::BLOCK_RIGHT] = { n,  0, n, n };
	dict_rect[(int)Tile::BLOCK] = { 2 * n,  0, n, n };
	dict_rect[(int)Tile::GRASS] = { 22 * n, 0, n, n };

	dict_rect[(int)Tile::STAIRS_LEFT] = { 0, n, n, n };
	dict_rect[(int)Tile::STAIRS_LEFT_TOP] = { 0, n, n, n };
	dict_rect[(int)Tile::STAIRS_RIGHT] = { n, n, n, n };
	dict_rect[(int)Tile::STAIRS_RIGHT_TOP] = { n, n, n, n };
	dict_rect[(int)Tile::STAIRS_BOSS] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::STAIRS_BOSS_TOP] = { 2 * n, n, n, n };

	dict_rect[(int)Tile::BOSS_ITEM1] = { 10 * n,  0, n, n };
	dict_rect[(int)Tile::BOSS_ITEM2] = { 5 * n, n, n, n };
	dict_rect[(int)Tile::BOSS_ITEM3] = { 11 * n,  0, n, n };

	dict_rect[(int)Tile::BOSS_ATTACK] = { 21 * n,  0, n, n };

	dict_rect[(int)Tile::OBJECT_ANIM1] = { 12 * n,  0, n, n };
	dict_rect[(int)Tile::OBJECT_ANIM2] = { 13 * n,  0, n, n };
	dict_rect[(int)Tile::OBJECT_ANIM3] = { 14 * n,  0, n, n };

	dict_rect[(int)Tile::CHEST] = { 15 * n,  0, n, n };

	dict_rect[(int)Tile::FIRE1] = { 18 * n, n, n, n };
	dict_rect[(int)Tile::FIRE2] = { 19 * n, n, n, n };
	dict_rect[(int)Tile::CANDLE_BIG] = { 20 * n, n, n, n };
	dict_rect[(int)Tile::CANDLE_SMALL] = { 21 * n, n, n, n };

	dict_rect[(int)Tile::GOLDEN_KEY] = { 16 * n,  0, n, n };
	dict_rect[(int)Tile::SILVER_KEY] = { 17 * n,  0, n, n };

	dict_rect[(int)Tile::SMALL_HEART] = { 18 * n,  0, n, n };
	dict_rect[(int)Tile::BIG_HEART] = { 19 * n,  0, n, n };
	dict_rect[(int)Tile::PLAIN_HEART] = { 20 * n,  0, n, n };

	dict_rect[(int)Tile::WHIP] = { 3 * n,  0, n, n };
	dict_rect[(int)Tile::AXE] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::KNIFE] = { 5 * n,  0, n, n };
	dict_rect[(int)Tile::BLUE_CROSS] = { 6 * n,  0, n, n };
	dict_rect[(int)Tile::HOURGLASS] = { 7 * n,  0, n, n };
	dict_rect[(int)Tile::HOLY_WATER] = { 8 * n,  0, n, n };
	dict_rect[(int)Tile::STAFF] = { 9 * n,  0, n, n };
	dict_rect[(int)Tile::MONEY_WHITE] = { 3 * n, n, n, n };
	dict_rect[(int)Tile::MONEY_BLUE] = { 4 * n, n, n, n };
	dict_rect[(int)Tile::BLUE_ORB] = { 6 * n, n, n, n };
	dict_rect[(int)Tile::BLUE_WATER] = { 7 * n, n, n, n };
	dict_rect[(int)Tile::BOOTS] = { 8 * n, n, n, n };
	dict_rect[(int)Tile::RED_SHIELD] = { 9 * n, n, n, n };
	dict_rect[(int)Tile::GOLDEN_SHIELD] = { 10 * n, n, n, n };
	dict_rect[(int)Tile::BLUE_RING] = { 11 * n, n, n, n };
	dict_rect[(int)Tile::BLACK_BIBLE] = { 12 * n, n, n, n };
	dict_rect[(int)Tile::WHITE_BIBLE] = { 13 * n, n, n, n };
	dict_rect[(int)Tile::WINGS] = { 14 * n, n, n, n };
	dict_rect[(int)Tile::SILVER_CROSS] = { 15 * n, n, n, n };
	dict_rect[(int)Tile::GOLDEN_CROSS] = { 16 * n, n, n, n };
	dict_rect[(int)Tile::PARCHMENT] = { 17 * n, n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/Tileset1.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	//laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}

bool TileMap::IsTileLadder(Tile tile) const
{
	return (Tile::STAIR_FIRST <= tile && tile <= Tile::STAIR_LAST);
}

bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}

void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::EMPTY or tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;
				rc = dict_rect[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TILES);

	//laser->Release();

	dict_rect.clear();
}

bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadder(tile2) || IsTileLadder(tile1))
	{
		return true;
	}
	return false;
}


//int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
//{
//	int tx, ty;
//
//	tx = pixel_x / TILE_SIZE;
//	ty = pixel_y / TILE_SIZE;
//	Tile tile = GetTileIndex(tx, ty);
//
//	if (tile == Tile::STAIRS_LEFT || tile == Tile::STAIRS_BOSS)		return tx * TILE_SIZE + TILE_SIZE;
//	else if (tile == Tile::STAIRS_RIGHT)	return tx * TILE_SIZE;
//	else
//	{
//		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
//		return 0;
//	}
//}

bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadderTop(tile2) || IsTileLadderTop(tile1))
	{
		//*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}

bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::STAIRS_LEFT_TOP || tile == Tile::STAIRS_RIGHT_TOP || tile == Tile::STAIRS_BOSS_TOP;
}