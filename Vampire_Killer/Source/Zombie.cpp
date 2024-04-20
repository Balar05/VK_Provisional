
#include "Zombie.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Zombie::Zombie(const Point& p, State s, Look view) :
	Entity(p, ZOMBIE_PHYSICAL_WIDTH, ZOMBIE_PHYSICAL_HEIGHT, ZOMBIE_FRAME_SIZE, ZOMBIE_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	score = 0;
}
Zombie::~Zombie()
{
}
AppStatus Zombie::Initialise()
{
	int i;
	const int n = ZOMBIE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/Sprites/32x32 Enemies.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZombieAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZombieAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::IDLE_RIGHT, { 0, 0, -n, n });
	sprite->SetAnimationDelay((int)ZombieAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)ZombieAnim::IDLE_LEFT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)i * n, 0 * n, -n, n });
	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)i * n, 0 * n, -n, n });

	sprite->SetAnimation((int)ZombieAnim::IDLE_RIGHT);

	return AppStatus::OK;
}

void Zombie::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Zombie::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Zombie::IsLookingLeft() const
{
	return look == Look::LEFT;
}
void Zombie::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
ZombieAnim Zombie::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (ZombieAnim)sprite->GetAnimation();
}
void Zombie::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)ZombieAnim::IDLE_RIGHT);
	else					SetAnimation((int)ZombieAnim::IDLE_LEFT);
}
void Zombie::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)ZombieAnim::WALKING_LEFT);
}
void Zombie::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)ZombieAnim::WALKING_RIGHT);
}
void Zombie::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)ZombieAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)ZombieAnim::WALKING_RIGHT); break;
	}
}
void Zombie::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)ZombieAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)ZombieAnim::WALKING_LEFT); break;
	}
}
void Zombie::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	//MoveX();
	//MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
//void Zombie::MoveX()
//{
//	AABB box;
//	int prev_x = pos.x;
//
//
//	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
//	{
//		pos.x += -PLAYER_SPEED_X;
//		if (state == State::IDLE) StartWalkingLeft();
//		else
//		{
//			if (IsLookingRight()) ChangeAnimLeft();
//		}
//
//		box = GetHitbox();
//		if (map->TestCollisionWallLeft(box))
//		{
//			pos.x = prev_x;
//			if (state == State::WALKING) Stop();
//		}
//	}
//	else if (IsKeyDown(KEY_RIGHT))
//	{
//		pos.x += PLAYER_SPEED_X;
//		if (state == State::IDLE) StartWalkingRight();
//		else
//		{
//			if (IsLookingLeft()) ChangeAnimRight();
//		}
//
//		box = GetHitbox();
//		if (map->TestCollisionWallRight(box))
//		{
//			pos.x = prev_x;
//			if (state == State::WALKING) Stop();
//		}
//	}
//	else
//	{
//		if (state == State::WALKING) Stop();
//	}
//}
//void Player::MoveY()
//{
//	AABB box;
//
//	if (state == State::JUMPING)
//	{
//		LogicJumping();
//	}
//
//	else //idle, walking, falling
//	{
//		pos.y += PLAYER_SPEED_Y;
//		box = GetHitbox();
//		if (map->TestCollisionGround(box, &pos.y))
//		{
//			if (state == State::FALLING) Stop();
//
//
//			else if (IsKeyPressed(KEY_SPACE))
//			{
//				StartJumping();
//			}
//		}
//		else
//		{
//			if (state != State::FALLING) StartFalling();
//		}
//	}
//}

void Zombie::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Zombie::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}

int Zombie::GetZombiePosX()
{
	return pos.x;
}

int Zombie::GetZombiePosY()
{
	return pos.y;
}