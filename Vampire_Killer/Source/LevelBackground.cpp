#include <stdio.h>
#include "Globals.h"
#include <raylib.h>
#include "LevelBackground.h"
#include "ResourceManager.h"

LevelBackground::LevelBackground()
{
	backgroundImage = LoadTexture("images/Sprites/256x176 Levels.png");
}
void LevelBackground::RenderBackground(int stage)
{
	Rectangle source = { 0, 0, 0, 0 };
	float width = LEVEL_WIDTH * TILE_SIZE;
	float height = LEVEL_HEIGHT * TILE_SIZE;
	switch (stage)
	{
		case 1:
			source = { 0 * width, 0 * height, width, height }; break;
		case 2:
			source = { 1 * width, 0 * height, width, height }; break;
		case 3:
			source = { 2 * width, 0 * height, width, height }; break;
		case 4:
			source = { 0 * width, 2 * height, width, height }; break;
		case 5:
			source = { 1 * width, 2 * height, width, height }; break;
		case 6:
			source = { 3 * width, 2 * height, width, height }; break;
		case 7:
			source = { 2 * width, 2 * height, width, height }; break;
		case 8:
			source = { 1 * width, 1 * height, width, height }; break;
		case 9:
			source = { 0 * width, 1 * height, width, height }; break;
		case 10:
			source = { 2 * width, 1 * height, width, height }; break;
		case 11:
			source = { 3 * width, 2 * height, width, height }; break;
		case 12:
			source = { 0 * width, 3 * height, width, height }; break;
		default:
			LOG("Level background does not exist.");
	}
	DrawTextureRec(backgroundImage, source, { 0, 16 }, WHITE);
}
void LevelBackground::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BACKGROUND);
	//dict_rect.clear();
}