#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define ZOMBIE_FRAME_SIZE		32

//Logical model size: 12x28
#define ZOMBIE_PHYSICAL_WIDTH	14
#define ZOMBIE_PHYSICAL_HEIGHT	28

//Horizontal speed and vertical speed while falling down
#define ZOMBIE_SPEED_X			1

#define ZOMBIE_SPEED_Y			2

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, DEAD };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class ZombieAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	NUM_ANIMATIONS
};

class Zombie : public Entity
{
public:
	Zombie(const Point& p, State s, Look view);
	~Zombie();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	int GetZombiePosX();
	int GetZombiePosY();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	//void MoveX();
	//void MoveY();

	//Animation management
	void SetAnimation(int id);
	ZombieAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void ChangeAnimRight();
	void ChangeAnimLeft();;

	State state;
	Look look;
	int jump_delay;

	TileMap* map;

	int score;
};
