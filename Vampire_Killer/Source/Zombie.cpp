#include "Zombie.h"
#include "Sprite.h"

Zombie::Zombie(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = ZombieState::ROAMING;

	current_step = 0;
	current_frames = 0;
}

Zombie::~Zombie()
{
}

AppStatus Zombie::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ENEMIES_FRAME_SIZE;
	state = ZombieState::ROAMING;
	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for zombie sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZombieAnim::NUM_ANIMATIONS);

	//sprite->SetAnimationDelay((int)ZombieAnim::IDLE_RIGHT, ZOMBIE_ANIM_DELAY);
	//sprite->AddKeyFrame((int)ZombieAnim::IDLE_RIGHT, { 0, 0 * n, n, n });
	//sprite->SetAnimationDelay((int)ZombieAnim::IDLE_LEFT, ZOMBIE_ANIM_DELAY);
	//sprite->AddKeyFrame((int)ZombieAnim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_RIGHT, ZOMBIE_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_RIGHT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)ZombieAnim::WALKING_LEFT, ZOMBIE_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)ZombieAnim::WALKING_LEFT, { (float)i * n, 0 * n, n, n });

	this->look = look;
	if (look == Look::LEFT)        sprite->SetAnimation((int)ZombieAnim::WALKING_RIGHT);
	else if (look == Look::RIGHT) sprite->SetAnimation((int)ZombieAnim::WALKING_LEFT);

	visibility_area = area;

	InitPattern();

	return AppStatus::OK;
}

void Zombie::InitPattern()
{
	const int n = ZOMBIE_ANIM_DELAY * 2;

	if (look == Look::LEFT)
	{
		pattern.push_back({ {ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_RIGHT });
	}
	else
	{
		pattern.push_back({ {-ZOMBIE_SPEED, 0}, n, (int)ZombieAnim::WALKING_LEFT });
	}

	current_step = 0;
	current_frames = 0;
}

bool Zombie::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int anim_id;

	if (state == ZombieState::ROAMING)
	{
		pos += pattern[current_step].speed;
		current_frames++;

		if (current_frames == pattern[current_step].frames)
		{
			current_step++;
			current_step %= pattern.size();
			current_frames = 0;

			anim_id = pattern[current_step].anim;
			sprite->SetAnimation(anim_id);
			
		}
	}
	sprite->Update();

	return false; 
}


void Zombie::UpdateLook(int anim_id)
{
	ZombieAnim anim = (ZombieAnim)anim_id;
	look = (anim == ZombieAnim::IDLE_LEFT ||
		anim == ZombieAnim::WALKING_LEFT) ? Look::LEFT : Look::RIGHT;
}
