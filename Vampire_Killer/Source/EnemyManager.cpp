#include "EnemyManager.h"
#include "Zombie.h"

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

void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy;

	if (type == EnemyType::ZOMBIE)
	{
		enemy = new Zombie(pos, ZOMBIE_PHYSICAL_WIDTH, ZOMBIE_PHYSICAL_HEIGHT, ZOMBIE_FRAME_SIZE, ZOMBIE_FRAME_SIZE); //Slime.h
	}
	//else if (type == EnemyType::AQUAMAN)
	//{
	//	enemy = new Aquaman(pos, AQUAMAN_PHYSICAL_WIDTH, AQUAMAN_PHYSICAL_HEIGHT, AQUAMAN_FRAME_SIZE, AQUAMAN_FRAME_SIZE); //Turret.h
	//}
	else
	{
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
	bool shoot;
	Point p, d;

	for (Enemy* enemy : enemies)
	{
		/*shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}*/
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
		delete enemy;
	enemies.clear();
}