
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/Sprites/32x32 Simon Belmont.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * n, 0 * n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * n, 0 * n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, 1 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, 1 * n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, 0 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_RIGHT, ANIM_LADDER_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_RIGHT, { (float)i * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_LEFT, ANIM_LADDER_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_LEFT, { (float)i * n, 1 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_RIGHT, ANIM_LADDER_DELAY);
	for (i = 2; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_RIGHT, { (float)i * n, 1 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_LEFT, ANIM_LADDER_DELAY);
	for (i = 2; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_LEFT, { (float)i * n, 1 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 4 * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 5 * n, 6 * n, n, n });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}

void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED_Y;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}

void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}

void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
	case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
	}
}

void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
	case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
	case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
	case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
	}
}

void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}

void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;


	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && state != State::CLIMBING)
	{
		pos.x += -PLAYER_SPEED_X;
		if (state == State::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT) && state != State::CLIMBING)
	{
		pos.x += PLAYER_SPEED_X;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}

void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED_Y;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}

void Player::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}

void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}

int Player::GetPlayerPosX()
{
	return pos.x;
}

int Player::GetPlayerPosY()
{
	return pos.y;
}



bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}

void Player::StartClimbingRight()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}

void Player::StartClimbingLeft()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}

void Player::StartClimbingDownRight()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_DOWN_RIGHT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}

void Player::StartClimbingDownLeft()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_DOWN_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}

void Player::MoveY()
{
	AABB box;

	CheckPosY();

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else if (state == State::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED_Y;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x) && IsLookingRight())
					StartClimbingRight();
				else if (map->TestOnLadder(box, &pos.x) && IsLookingLeft())
					StartClimbingLeft();
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x) and IsLookingRight())
					StartClimbingDownRight();
				else if (map->TestOnLadder(box, &pos.x) and IsLookingLeft())
					StartClimbingDownLeft();
			}

			if (IsKeyPressed(KEY_UP))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x) && IsLookingRight())
				{

				}
				else if (map->TestOnLadder(box, &pos.x) && IsLookingLeft())
				{

				}
				else
				{
					StartJumping();
				}
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
	CheckPosY();
}

void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		CheckPosY();
		if (IsLookingRight())
		{
			pos.x += PLAYER_LADDER_SPEED;
		}
		else if (IsLookingLeft())
		{
			pos.x -= PLAYER_LADDER_SPEED;
		}
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		CheckPosY();
		if (IsLookingRight())
		{
			look = Look::LEFT;
			pos.x -= PLAYER_LADDER_SPEED;
		}
		else if (IsLookingLeft())
		{
			look = Look::RIGHT;
			pos.x -= PLAYER_LADDER_SPEED;
		}
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	
	box = GetHitbox();
	if (map->TestOnLadderTop(box, &tmp))
	{
		if (IsLookingRight())
		{
			if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
		}
		else if (IsLookingLeft())
		{
			if (IsInSecondHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
		}
		
		else	LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	}

	else if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else if (!map->TestOnLadder(box, &tmp))
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	/*else
	{
		if (GetAnimation() != PlayerAnim::CLIMBING_RIGHT)	SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
		else if (GetAnimation() != PlayerAnim::CLIMBING_LEFT)	SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
	}*/
}

void Player::CheckPosY()
{
	if (pos.y > WINDOW_HEIGHT - TILE_SIZE*3-5)
	{
		pos.y = (WINDOW_HEIGHT - TILE_SIZE*3-5);
	}
	else if (pos.y < WINDOW_HEIGHT - LEVEL_HEIGHT * TILE_SIZE - TILE_SIZE)
	{
		pos.y = WINDOW_HEIGHT - LEVEL_HEIGHT * TILE_SIZE - TILE_SIZE;
	}
}