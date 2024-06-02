#pragma once
#include "Enemy.h" 

#define ZOMBIE_SPEED			1
#define ZOMBIE_ANIM_DELAY	(2*ANIM_DELAY)

enum class ZombieState { ROAMING }; //, ATTACK
enum class ZombieAnim {
	IDLE_LEFT, IDLE_RIGHT, WALKING_LEFT, WALKING_RIGHT,
	NUM_ANIMATIONS
};

struct Step
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Zombie : public Enemy
{
public:
	Zombie(const Point& p, int width, int height, int frame_width, int frame_height);
	~Zombie();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

	//Retrieve the position and direction of the shot to be thrown

private:
	//Create the pattern behaviour
	void InitPattern();

	void UpdateLook(int anim_id);

	int attack_delay;	//delay between attacks
	ZombieState state;

	int current_step;	
	int current_frames;	
	std::vector<Step> pattern;
};
