#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28
#define ATTACK_WIDTH			24
#define ATTACK_HEIGHT			14

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED_X			1

#define PLAYER_SPEED_Y			2

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		8

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	0

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1


//Logic states
enum class State { IDLE, WALKING, JUMPING, SNEAKING, FALLING, ATTACKING, DEAD };
enum class Look { RIGHT, LEFT };
typedef struct
{
	float Lifetime;
}Timer;
//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	FALLING_LEFT_NJ, FALLING_RIGHT_NJ,
	SNEAKING_LEFT, SNEAKING_RIGHT,
	ATTACKING_LEFT, ATTACKING_RIGHT,
	DEAD_LEFT, DEAD_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS
};

class Player : public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	const int totalFramesAttack = 3;
	float currentFrameAttack = 0;
	float framesCounter = 0;
	float framesSpeed = 1;

	double waitTime = 2.0; // Tiempo de espera en segundos
	double timer = 0.0; // Inicializa el temporizador

	void StartTimer(Timer* timer, float lifetime)
	{
		if (timer != NULL)
			timer->Lifetime = lifetime;
	}

	// update a timer with the current frame time
	void UpdateTimer(Timer* timer)
	{
		// subtract this frame from the timer if it's not allready expired
		if (timer != NULL && timer->Lifetime > 0)
			timer->Lifetime -= GetFrameTime();
	}

	// check if a timer is done.
	bool TimerDone(Timer* timer)
	{
		if (timer != NULL)
			return timer->Lifetime <= 0;

		return false;
	}

	Timer attackTimer = { 0 };
	float attackLife = 0.5f; // Duration of the attack animation
private:

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void MoveY_SNEAK();
	void LogicJumping();
	void Attack();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartFalling_NJ();
	void StartJumping();
	void StartSneaking();
	void StopSneaking();
	void StartAttacking();
	void StopAttacking();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;


	bool attackAnimationComplete = false;

	State state;
	Look look;
	int jump_delay;

	TileMap* map;

	int score;

	Texture2D attackAnim;
};

