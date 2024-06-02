#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "LevelBackground.h"
#include "Enemy.h"
#include "Guepardo.h"
//#include "Murcielago.h"
#include <algorithm>
#include "Game.h"
#include "FakeWall.h"

Scene::Scene() : currentStage(1), guepardoGenerated(false), llamaGenerated(false), candleGenerated(false), fakewallGenerated(false)
{
	player = nullptr;
	level = nullptr;
	background = nullptr;
	enemies = nullptr;

	font1 = nullptr;

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
	if (font1 != nullptr)
	{
		delete font1;
		font1 = nullptr;
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
	player = new Player({ 15, LEVEL_HEIGHT * TILE_SIZE - TILE_SIZE }, State::IDLE, Look::RIGHT);
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

	//Create text font 1
	font1 = new Text();
	if (font1 == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	//Initialise text font 1
	if (font1->Initialise(Resource::IMG_FONT1, "images/font8x8.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

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
	guepardoGenerated = false;
	llamaGenerated = false;
	candleGenerated = false;
	fakewallGenerated = false;

	if (stage == 1)
	{
		currentStage = 1;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4

		};
		//player->InitScore();
	}
	else if (stage == 2)
	{
		currentStage = 2;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
		};
	}
	else if (stage == 3)
	{
		currentStage = 3;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0,
				4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
		};
	}
	else if (stage == 4)
	{
		currentStage = 4;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 0, 0, 0, 600/*candle*/, 0, 0, 0, 0/*candle */, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 44, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 5)
	{
		currentStage = 5;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 7, 600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 6)
	{
		currentStage = 6;
		//guepardoGenerated = false;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 3, 9, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
				0, 0, 1, 2, 1, 2, 10, 3, 1, 2, 1, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 7, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 7)
	{
		currentStage = 7;
		//guepardoGenerated = false;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 3,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0,
				1, 2, 1, 2, 3, 9, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 7, 0, 0, 0, 0,
				0, 0, 0, 0, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0,
				0,0, 0, 0, 0, 0, 1, 2, 45, 0, 0, 0, 0, 0, 0, 0,
				44, 0, 0, 0, 0, 0, 1, 2, 5, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 8)
	{
		currentStage = 8;
		//guepardoGenerated = false;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 200, 0, 6, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 3, 6, 1, 2, 1, 2
		};
	}
	else if (stage == 9)
	{
		currentStage = 9;
		//guepardoGenerated = false;
		map = new int [size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 0, 0, 1, 2, 1, 9, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0,
			1, 2, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
			1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 1, 2, 1, 2,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 10)
	{
		currentStage = 10;
		//guepardoGenerated = false;
		map = new int [size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1, 2, 1, 2, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 200, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 11)
	{
		currentStage = 11;
		//guepardoGenerated = false;
		map = new int[size] {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 3, 1, 2, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 44, 7, 0, 0, 0, 0, 1, 2,
				1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
				1, 2, 1, 2, 7, 3, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2
		};
	}
	else if (stage == 12)
	{
		currentStage = 12;
		//guepardoGenerated = false;
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
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0,
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

	enemies->Release();

	//Entities and objects
	auto isCollected = [this](ObjectType type) {
		return std::find(collectedObjects.begin(), collectedObjects.end(), type) != collectedObjects.end();
	};
	i = 0;
	for (y = 0; y <= LEVEL_HEIGHT - 2; ++y) {
		for (x = 0; x < LEVEL_WIDTH; ++x) {
			tile = (Tile)map[i];
			if (tile == Tile::AIR) {
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::GOLDEN_KEY)
			{
				if (!isCollected(ObjectType::GOLDEN_KEY)) {
					pos.x = x * TILE_SIZE;
					pos.y = y * TILE_SIZE + TILE_SIZE - 1;
					obj = new Object(pos, ObjectType::GOLDEN_KEY);
					objects.push_back(obj);
				}
				map[i] = 0;
			}
			else if (tile == Tile::SILVER_KEY)
			{
				if (!isCollected(ObjectType::SILVER_KEY)) {
					pos.x = x * TILE_SIZE;
					pos.y = y * TILE_SIZE + TILE_SIZE - 1;
					obj = new Object(pos, ObjectType::SILVER_KEY);
					objects.push_back(obj);
				}
				map[i] = 0;
			}
			else if (tile == Tile::ZOMBIE) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::ZOMBIE);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::ZOMBIE, area, Look::RIGHT);
				map[i] = 0;
			}
			else if (tile == Tile::GUEPARDO) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::GUEPARDO);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::GUEPARDO, area, Look::RIGHT);
				guepardoGenerated = true;
				map[i] = 0;
			}
			else if (tile == Tile::MURCIELAGO) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::MURCIELAGO);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::MURCIELAGO, area, Look::RIGHT);
				map[i] = 0;
			}
			else if (tile == Tile::LLAMA) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::LLAMA);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::LLAMA, area, Look::RIGHT);
				llamaGenerated = true;
				map[i] = 0;
			}
			else if (tile == Tile::CANDLE) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::CANDLE);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::CANDLE, area, Look::RIGHT);
				candleGenerated = true;
				map[i] = 0;
			}
			else if (tile == Tile::FAKEWALL) {
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::FAKEWALL);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::FAKEWALL, area, Look::RIGHT);
				fakewallGenerated = true;
				map[i] = 0;
			}
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete map;

	frameCounter = 0;

	return AppStatus::OK;
}
void Scene::Update() {
	Point p1, p2;
	AABB hitbox, area;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1)) {
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
	else if (IsKeyPressed(KEY_D))		player->GetDamage(Look::RIGHT);
	else if (IsKeyPressed(KEY_F2))		player->god = true;
	else if (IsKeyPressed(KEY_F3))		player->god = false;

	level->Update();
	player->Update();

	// Generar nuevos zombies según la lógica de tu juego
	GenerateZombies();
	// Generar nuevos guepardos según la lógica de tu juego
	GenerateGuepardos();
	// Generar nuevos murcielagos según la lógica de tu juego
	GenerateMurcielagos();

	GenerateLLama();

	GenerateCandle();

	GenerateFakeWall();

	UpdateBackground(currentStage);

	CheckCollisions();

	hitbox = player->GetHitbox();
	enemies->Update(hitbox);

	// Si es necesario, podemos agregar cualquier otra lógica de actualización aquí
}


void Scene::GenerateZombies()
{
	if (currentStage != 4 && currentStage != 5 && currentStage != 8 && currentStage != 10)
	{
		return; // No generar zombies si no estamos en el nivel 4 o 5
	}

	// Lógica de generación de zombies
	static int frameCounter = 0;
	frameCounter++;

	if (frameCounter >= 120) // Genera un zombie cada 120 frames (2 segundos si el juego corre a 60 FPS)
	{
		frameCounter = 0;

		Point pos;
		pos.y = 191; // Altura en la que quieres que aparezcan los zombies

		AABB area; // Crear un área adecuada para el enemigo

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
void Scene::GenerateGuepardos() {

	if ((currentStage == 6 || currentStage == 11) && !guepardoGenerated) {
		Point pos;
		AABB area;

		if (currentStage == 6) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 46;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE - 32;
		}
		else if (currentStage == 11) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE-16;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 32;
		}

		if (player->GetPlayerPosX() > WINDOW_WIDTH / 2)
		{
			enemies->Add(pos, EnemyType::GUEPARDO, area, Look::LEFT);
			guepardoGenerated = true;
		}
		else
		{
			enemies->Add(pos, EnemyType::GUEPARDO, area, Look::RIGHT);
			guepardoGenerated = true;
		}
	}
}

void Scene::GenerateMurcielagos()
{
	if (currentStage != 9)
	{
		return; // No generar Murcielagos si no estamos en el nivel 4 o 5
	}

	// Lógica de generación de Murcielagos
	static int frameCounter = 0;
	frameCounter++;

	if (frameCounter >= 200) // Genera un zombie cada 120 frames (2 segundos si el juego corre a 60 FPS)
	{
		frameCounter = 0;

		Point pos;
		pos.y = player->GetPlayerPosY()-10; // Altura en la que quieres que aparezcan los zombies

		AABB area; // Crear un área adecuada para el enemigo

		if (player->GetPlayerPosX() > WINDOW_WIDTH / 2)
		{
			pos.x = 0;
			enemies->Add(pos, EnemyType::MURCIELAGO, area, Look::LEFT);
		}
		else
		{
			pos.x = (LEVEL_WIDTH - 1) * TILE_SIZE;
			enemies->Add(pos, EnemyType::MURCIELAGO, area, Look::RIGHT);
		}
	}
}

void Scene::GenerateLLama() {

	if ((currentStage == 1 || currentStage == 2 || currentStage == 3) && !llamaGenerated) {
		Point pos, pos2;
		AABB area;

		if (currentStage == 1) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 64;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 64;
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
		}
		else if (currentStage == 2) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 64;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 64;
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
		}
		else if (currentStage == 3) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 64;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
		}


			enemies->Add(pos, EnemyType::LLAMA, area, Look::LEFT);
			enemies->Add(pos2, EnemyType::LLAMA, area, Look::LEFT);
			llamaGenerated = true;

	}
}

void Scene::GenerateCandle() {

	if ((currentStage == 4 || currentStage == 5 || currentStage == 6 || currentStage == 7 || currentStage == 8 || currentStage == 9 || currentStage == 10 || currentStage == 11) && !candleGenerated) {
		Point pos, pos2, pos3, pos4, pos5;
		AABB area;

		if (currentStage == 4) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 16;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 48;
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
		}
		else if (currentStage == 5) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 16;  //derecha inf
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79; 
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 81; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
			pos3.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 16; //izquierda superior 
			pos3.y = (LEVEL_HEIGHT / 2) * TILE_SIZE -1;
			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 48; //derecha superior
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 63;
		}
		else if (currentStage == 6) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE ;		 //derecha inf 	
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;  
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 97; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
			pos3.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 65; //izquierda superior 
			pos3.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 31;
			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 48; //derecha superior 
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 15;
		}
		else if (currentStage == 7) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 80;		 //derecha inf 	
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
			pos3.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 32; //izquierda superior 
			pos3.y = (LEVEL_HEIGHT / 2) * TILE_SIZE - 1;
			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 48; //derecha superior 
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE - 1;
		}
		else if (currentStage == 8) {
	
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 65; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;

			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 16; //derecha superior
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 63;
		}
		else if (currentStage == 9) {

			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 32;  //derecha inf
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 81; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 31;
			pos3.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 81; //izquierda superior 
			pos3.y = (LEVEL_HEIGHT / 2) * TILE_SIZE - 33;
			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 32; //derecha superior
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 79;
			pos5.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 48; //derecha superior
			pos5.y = (LEVEL_HEIGHT / 2) * TILE_SIZE - 1;
		}
		else if (currentStage == 10) {

			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 16; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 63;

			pos4.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 80; //derecha superior 
			pos4.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 15;
		}
		else if (currentStage == 11) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE - 32;		 //derecha inf 	
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
			pos2.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 32; //izquierda inf
			pos2.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;
		}

		enemies->Add(pos, EnemyType::CANDLE, area, Look::LEFT);
		enemies->Add(pos2, EnemyType::CANDLE, area, Look::LEFT);
		enemies->Add(pos3, EnemyType::CANDLE, area, Look::LEFT);
		enemies->Add(pos4, EnemyType::CANDLE, area, Look::LEFT);
		enemies->Add(pos5, EnemyType::CANDLE, area, Look::LEFT);
		candleGenerated = true;

	}
}

void Scene::GenerateFakeWall() {

	if ((currentStage == 7 || currentStage == 11) && !fakewallGenerated) {
		Point pos;
		AABB area;

		if (currentStage == 7) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;

		}
		else if (currentStage == 11) {
			pos.x = (LEVEL_WIDTH / 2) * TILE_SIZE + 64;
			pos.y = (LEVEL_HEIGHT / 2) * TILE_SIZE + 95;

		}


		enemies->Add(pos, EnemyType::FAKEWALL, area, Look::LEFT);

		fakewallGenerated = true;

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
		enemies->Draw();
		player->Draw(); 
		
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemies->DrawDebug();
		//dibujar area de detección
		auto& enemiesList = enemies->GetEnemies();
		for (Enemy* enemy : enemiesList) {
			Guepardo* guepardo = dynamic_cast<Guepardo*>(enemy);
			if (guepardo) {
				guepardo->DrawDetectionArea();
			}
		}

	}

	EndMode2D();

	RenderGUI();
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

			player->items.push_back((*it)->type);
			collectedObjects.push_back((*it)->type);
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
	auto& enemiesList = enemies->GetEnemies();
	enemiesList.erase(std::remove_if(enemiesList.begin(), enemiesList.end(),
		[this, &player_box](Enemy* enemy) {
			AABB enemy_box = enemy->GetHitbox();

			// Check collision with player hitbox
			if (player_box.TestAABB(enemy_box)) {
				if (enemy->CausesDamage()) {
					// Determinar la direccion del daño
					Look damageDirection = player->GetPlayerPosX() > enemy->GetPos().x ? Look::LEFT : Look::RIGHT;
					player->GetDamage(damageDirection);
				}
				return false; // Don't remove enemy
			}

			// Check collision with player attack hitbox
			AABB attack_hitbox = player->GetAttackHitbox();
			if (attack_hitbox.TestAABB(enemy_box)) {
				// Eliminar al zombie
				player->IncrScore(100);
				delete enemy;
				return true; // Remove enemy
			}

			return false; // Don't remove enemy
		}), enemiesList.end());

	// Check collisions with fake walls
	for (const auto& enemy : enemiesList) {
		FakeWall* fakeWall = dynamic_cast<FakeWall*>(enemy);
		if (fakeWall) {
			AABB fakewall_box = fakeWall->GetHitbox();
			if (player_box.TestAABB(fakewall_box)) {
				// Ajustar la posición del jugador para evitar la colisión
				if (player_box.pos.x < fakewall_box.pos.x) {
					player->SetPos({ fakewall_box.pos.x - player_box.width, player->GetPlayerPosY() });
				}
				else if (player_box.pos.x > fakewall_box.pos.x) {
					player->SetPos({ fakewall_box.pos.x + fakewall_box.width, player->GetPlayerPosY() });
				}
			}
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
void Scene::RenderGUI() const
{
	static int frame;
	frame++;
	frame %= 1000;

	//UI stages
	if (currentStage == 1) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 0));
	}
	if (currentStage == 2) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 0));
	}
	if (currentStage == 3) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 0));
	}
	if (currentStage == 4) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 1));
	}
	if (currentStage == 5) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 1));
	}
	if (currentStage == 6) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 1));
	}
	if (currentStage == 7) {
		font1->Draw(155, 1, TextFormat("%d%d", 0, 1));
	}
	// UI score
	font1->Draw(192, 1, TextFormat("%d%d", 0, 3));
	font1->Draw(228, 1, TextFormat("%d%d", 0, 0));
	font1->Draw(58, 1, TextFormat("%d", player->GetScore()));

	const std::vector<ObjectType>& items = player->GetItems();

	for (int i = 0; i < items.size(); ++i)
	{
		Vector2 pos;
		if (items[i] == ObjectType::GOLDEN_KEY || items[i] == ObjectType::SILVER_KEY)
		{
			
			pos = { static_cast<float>(146 + i * (OBJECT_FRAME_SIZE )), 13.0f };
		}
		else
		{
			
			pos = { static_cast<float>(167 + i * (OBJECT_FRAME_SIZE + 1)), 12.0f };
		}

		// Dibuja el ítem en la interfaz
		player->DrawItem(items[i], pos);
	}

}

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
			LoadLevel(2);
			player->SetPos({ 15, y });
			
			break;
		}
	case 2:
		if (x < 0)
		{
			LoadLevel(1);
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			LoadLevel(3);
			player->SetPos({ 15, y });
			
			
			break;
		}
	case 3:
		if (x < 0)
		{
			LoadLevel(2);
			player->SetPos({ (LEVEL_WIDTH * TILE_SIZE) - 15 - PLAYER_PHYSICAL_WIDTH, y });
			
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			LoadLevel(4);
			player->SetPos({ 15, y });
			
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
		else if (y < 2 * TILE_SIZE)
		{
			player->SetPos({ (10 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, (LEVEL_HEIGHT * TILE_SIZE) - (TILE_SIZE) });
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
		else if (y < 2 * TILE_SIZE)
		{
			player->SetPos({ (6 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, (LEVEL_HEIGHT * TILE_SIZE) - (TILE_SIZE) });
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
		else if (y >= LEVEL_HEIGHT * TILE_SIZE)
		{
			LoadLevel(5);
			player->SetPos({ (15 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, 4 * TILE_SIZE });

			break;
		}
	case 9:
		if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			player->SetPos({ 15, y });
			LoadLevel(8);
			break;
		}
		else if (x < 0)
		{
			player->SetPos({ 0,y });
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
			player->SetPos({ 15, (LEVEL_HEIGHT * TILE_SIZE) - (TILE_SIZE) });
			LoadLevel(12);
			break;
		}
		else if (y >= LEVEL_HEIGHT * TILE_SIZE)
		{
			LoadLevel(6);
			player->SetPos({ (3 * TILE_SIZE) - PLAYER_PHYSICAL_WIDTH, 4 * TILE_SIZE });

			break;
		}
	case 12:
		
		if (x < 0)
		{
			player->SetPos({ 0,y });
			break;
		}
		else if (x + PLAYER_PHYSICAL_WIDTH >= LEVEL_WIDTH * TILE_SIZE)
		{
			exit = true;
			break;
		}
	}
}