#include "LevelBackground.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

LevelMap::LevelMap()
{
	background = nullptr;
	width = 0;
	height = 0;
	img_level = nullptr;

	InitLevelDictionary();
}
LevelMap::~LevelMap()
{
	if (background != nullptr)
	{
		delete[] background;
		background = nullptr;
	}
}
void LevelMap::InitLevelDictionary()
{
	const int x = LEVEL_WIDTH * TILE_SIZE;
	const int y = LEVEL_HEIGHT * TILE_SIZE;

	dict_rect[(int)Level::LEVEL1] = { 0,  0, x, y };
	
}
AppStatus LevelMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_BACKGROUND,"images\Sprites\256x176 Levels.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_level = data.GetTexture(Resource::IMG_BACKGROUND);

	return AppStatus::OK;
}
AppStatus LevelMap::Load(int data[], int w, int h)
{
	size = w * h;
	width = w;
	height = h;

	if (background != nullptr)	delete[] background;

	background = new Level[size];
	if (background== nullptr)
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(background, data, size * sizeof(int));

	return AppStatus::OK;
}
void LevelMap::Update()
{
	//laser->Update();
}
Level LevelMap::GetLevelIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Level::AIR;
	}
	return background[x + y * width];
}

void LevelMap::Render()
{
	Level level;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			level = background[i * width + j];
			if (level != Level::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;
				rc = dict_rect[(int)background];
				DrawTextureRec(*img_level, rc, pos, WHITE);
			}
		}
	}
}
void LevelMap::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BACKGROUND);

	dict_rect.clear();
}
