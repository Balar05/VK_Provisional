﻿#include "EnemyManager.h"
#include "Zombie.h"
#include "Guepardo.h"
#include "Murcielago.h"

EnemyManager::EnemyManager()
{

}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/sprites/32x32 Enemies.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}

void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look) {
	Enemy* enemy;

	if (type == EnemyType::ZOMBIE) {
		enemy = new Zombie(pos, ZOMBIE_PHYSICAL_WIDTH, ZOMBIE_PHYSICAL_HEIGHT, ENEMIES_FRAME_SIZE, ENEMIES_FRAME_SIZE);
	}
	else if (type == EnemyType::GUEPARDO) {
		enemy = new Guepardo(pos, GUEPARDO_PHYSICAL_WIDTH, GUEPARDO_PHYSICAL_HEIGHT, GUEPARDO_FRAME_SIZE, GUEPARDO_FRAME_SIZE);
	}
	else if (type == EnemyType::MURCIELAGO) {
		enemy = new Murcielago(pos, MURCIELAGO_PHYSICAL_WIDTH, MURCIELAGO_PHYSICAL_HEIGHT, MURCIELAGO_FRAME_SIZE, MURCIELAGO_FRAME_SIZE);
	}
	else {
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	enemy->Initialise(look, area);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::ZOMBIE)
	{
		width = ZOMBIE_PHYSICAL_WIDTH;
		height = ZOMBIE_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::GUEPARDO)
	{
		width = GUEPARDO_PHYSICAL_WIDTH;
		height = GUEPARDO_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::MURCIELAGO)
	{
		width = MURCIELAGO_PHYSICAL_WIDTH;
		height = MURCIELAGO_PHYSICAL_HEIGHT;
	}

	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(player_hitbox); // Llama a la función Update de cada enemigo
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

std::vector<Point> EnemyManager::GetZombiePositions() const {
	std::vector<Point> positions;

	// Recorre todos los enemigos
	for (const Enemy* enemy : enemies) {
		// Si el enemigo es un zombie, añade su posición a la lista
		if (dynamic_cast<const Zombie*>(enemy)) {
			positions.push_back(enemy->GetPos());
		}
	}

	return positions;
}

std::vector<Point> EnemyManager::GetGuepardoPositions() const {
	std::vector<Point> positions;

	// Recorre todos los enemigos
	for (const Enemy* enemy : enemies) {
		// Si el enemigo es un guepardo, añade su posición a la lista
		if (dynamic_cast<const Guepardo*>(enemy)) {
			positions.push_back(enemy->GetPos());
		}
	}

	return positions;
}

std::vector<Point> EnemyManager::GetMurcielagoPositions() const {
	std::vector<Point> positions;

	// Recorre todos los enemigos
	for (const Enemy* enemy : enemies) {
		// Si el enemigo es un murcielago, añade su posición a la lista
		if (dynamic_cast<const Guepardo*>(enemy)) {
			positions.push_back(enemy->GetPos());
		}
	}

	return positions;
}