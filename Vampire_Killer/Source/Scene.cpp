#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "LevelBackground.h"
#include "Enemy.h"

Scene::Scene() : currentStage(1)
{
	player = nullptr;
	level = nullptr;
	background = nullptr;
	enemies = nullptr;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, 0 };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}

Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	if (level != nullptr)
	{
		level->Release();
		delete level;
		level = nullptr;
	}
	if (background != nullptr)
	{
		background->Release();
		delete background;
		background = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0, LEVEL_HEIGHT * TILE_SIZE - TILE_SIZE }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create level 
	level = new TileMap();
	if (level == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}

	//Create background
	background = new LevelBackground();
	if (background == nullptr)
	{
		LOG("Failed to allocate memory for background");
		return AppStatus::ERROR;
	}
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

	return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int* map = nullptr;
	Object* obj;
	AABB hitbox, area;

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;

	if (stage == 1)
	{
		currentStage = 1;
		map = new int[size] {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4

		};
		//player->InitScore();
	}
	else if (stage == 2)
	{
		currentStage = 2;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
		};
	}
	else if (stage == 3)
	{
		currentStage = 3;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
		};
	}
	else if (stage == 4)
	{
		currentStage = 4;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0/*candle*/, 0, 0, 0, 0/*candle */, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 44, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 5)
	{
		currentStage = 5;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 3, 1, 2, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 10, 3, 1, 2, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 7, 0, 0/*candle*/, 0, 0, 0, 0/*candle */, 0, 0, 0, 0,
				0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 6)
	{
		currentStage = 6;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
				0, 0, 1, 2, 1, 2, 7, 3, 1, 2, 1, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 7)
	{
		currentStage = 7;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   	   -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 3,
				0, 0, 0, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0,
				1, 2, 1, 2, 3, 6, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 0, 0, 0,
				0, 0, 0, 0, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 8)
	{
		currentStage = 8;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 1, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 9)
	{
	currentStage = 9;
	map = new int [size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 0, 0, 1, 2, 1, 6, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
			1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 1, 2, 1, 2,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 10)
	{
		currentStage = 10;
		map = new int [size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1, 2, 1, 2, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 11)
	{
		currentStage = 11;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 3, 1, 2, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 1, 2,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 2,
				1, 2, 1, 2, 7, 3, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 12)
	{
		currentStage = 12;
		map = new int [size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 3, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::GOLDEN_KEY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::GOLDEN_KEY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::SILVER_KEY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::SILVER_KEY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ZOMBIE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;

				// Obtenemos la posición del jugador
				int playerPosX = player->GetPlayerPosX();

				// Aquí agregas la condición para que los zombis solo aparezcan en los niveles
				if (stage == 4)
				{
					hitbox = enemies->GetEnemyHitBox(pos, EnemyType::ZOMBIE);
					area = level->GetSweptAreaX(hitbox);
				}
				// Comprobamos si el jugador está a la derecha o a la izquierda
				if (playerPosX > WINDOW_WIDTH / 2)
				{
					// Si el jugador está a la derecha, hacemos aparecer al zombie a la izquierda
					pos.x = 0;
					enemies->Add(pos, EnemyType::ZOMBIE, area, Look::LEFT); // El zombie se moverá hacia la derecha
				}
				else
				{
					// Si el jugador está a la izquierda, hacemos aparecer al zombie a la derecha
					pos.x = (LEVEL_WIDTH - 1) * TILE_SIZE; // Aseguramos que el zombie aparezca dentro del mapa
					enemies->Add(pos, EnemyType::ZOMBIE, area, Look::RIGHT); // El zombie se moverá hacia la izquierda
				}

			}


			else
			{
				LOG("Internal error loading scene: invalid entity or object tile id")
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB hitbox,area;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))			LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))		LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);
	else if (IsKeyPressed(KEY_FOUR))	LoadLevel(4);
	else if (IsKeyPressed(KEY_FIVE))	LoadLevel(5);
	else if (IsKeyPressed(KEY_SIX))		LoadLevel(6);
	else if (IsKeyPressed(KEY_SEVEN))	LoadLevel(7);
	else if (IsKeyPressed(KEY_EIGHT))	LoadLevel(8);
	else if (IsKeyPressed(KEY_NINE))	LoadLevel(9);
	else if (IsKeyPressed(KEY_T))		LoadLevel(10);
	else if (IsKeyPressed(KEY_Y))		LoadLevel(11);
	else if (IsKeyPressed(KEY_U))		LoadLevel(12);
	else if (IsKeyPressed(KEY_D))		player->GetDamage();

	level->Update();
	player->Update();

	UpdateBackground(currentStage);

	CheckCollisions();

	hitbox = player->GetHitbox();
	enemies->Update(hitbox);

	std::vector<Point> zombiePositions = enemies->GetZombiePositions();

	for (const Point& zombiePos : zombiePositions)
	{
		// Si un zombie llega a la mitad de la pantalla
		if (zombiePos.x == WINDOW_WIDTH / 2)
		{
			// Añade otro zombie
			Point pos;
			pos.y = 191; // Asumiendo que quieres que el nuevo zombie aparezca en la misma posición y que el jugador

			if (player->GetPlayerPosX() > WINDOW_WIDTH / 2)
			{
				pos.x = 0;
				enemies->Add(pos, EnemyType::ZOMBIE, area, Look::LEFT);
			}
			else
			{
				pos.x = (LEVEL_WIDTH - 1) * TILE_SIZE;
				enemies->Add(pos, EnemyType::ZOMBIE, area, Look::RIGHT);
			}
		}
	}

}
void Scene::Render()
{
	BeginMode2D(camera);

	background->RenderBackground(currentStage);
	level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw(); // Llama a la nueva función Draw del jugador
		enemies->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemies->DrawDebug();
	}

	EndMode2D();

	//RenderGUI();
}
void Scene::Release()
{
	background->Release();
	level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;

	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			//player->IncrScore((*it)->Points());

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}

	// Check collisions with enemies
	for (const Enemy* enemy : enemies->GetEnemies())
	{
		enemy_box = enemy->GetHitbox();
		if (player_box.TestAABB(enemy_box))
		{
			player->GetDamage();
			break; // Only take damage once per update cycle
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	enemies->Release();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
//void Scene::RenderGUI() const
//{
//	//Temporal approach
//	DrawText(TextFormat("LIVES : %d", player->GetLives()), 10, 10, 8, WHITE);
//}

void Scene::UpdateBackground(int s)
{
	int x = player->GetPlayerPosX();
	int y = player->GetPlayerPosY();
	switch (currentStage)
	{
	case 1:
		if (x < 0)
		{
			player->SetPos({ 0,y });
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(2);
			break;
		}
	case 2:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(1);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(3);
			LoadLevel(3);
			break;
		}
	case 3:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(2);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(4);
			break;
		}
	case 4:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(6);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(5);
			break;
		}
	case 5:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(4);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(7);
			break;
		}
		else if (y + PLAYER_PHYSICAL_HEIGHT >= LEVEL_HEIGHT * TILE_SIZE)
		{
			player->SetPos({ (10 * TILE_SIZE)-PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(8);
			break;
		}
	case 6:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(7);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(4);
			break;
		}
		else if (y + PLAYER_PHYSICAL_HEIGHT >= LEVEL_HEIGHT * TILE_SIZE)
		{
			player->SetPos({ (5 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(11);
			break;
		}
	case 7:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(5);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(6);
			break;
		}
	case 8:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(9);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(10);
			break;
		}
		else if (y + PLAYER_PHYSICAL_HEIGHT <= LEVEL_HEIGHT * TILE_SIZE)
		{
			player->SetPos({ (11 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(5);
			break;
		}
	case 9:
		if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(8);
			break;
		}
	case 10:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(8);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(11);
			break;
		}
	case 11:
		if (x < 0)
		{
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(10);
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(12);
			break;
		}
		else if (y + PLAYER_PHYSICAL_HEIGHT <= LEVEL_HEIGHT * TILE_SIZE)
		{
			player->SetPos({ (5 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, y });
			LoadLevel(6);
			break;
		}
	case 12:
		if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			//Release();
		}
	}
}