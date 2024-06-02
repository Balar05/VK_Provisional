#include "FakeWall.h"
#include "Sprite.h"

FakeWall::FakeWall(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height) {
    state = FakeWallState::IDLE;
    current_step = 0;
    current_frames = 0;
    dir = { 0, 0 };
    hitbox = AABB(p, width, height);
}
FakeWall::~FakeWall() {}

AppStatus FakeWall::Initialise(Look look, const AABB& area) {
    const int n = FAKEWALL_FRAME_SIZE;
    state = FakeWallState::IDLE;
    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_FAKEWALL));
    if (render == nullptr) {
        LOG("Failed to allocate memory for guepardo sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)FakeWallAnim::NUM_ANIMATIONS);

    sprite->SetAnimationDelay((int)FakeWallAnim::ROAMING, 15);
    for (int i = 0; i < 1; ++i)
        sprite->AddKeyFrame((int)FakeWallAnim::ROAMING, { (float)i * n, 16 , n, n });


    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)FakeWallAnim::ROAMING);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)FakeWallAnim::ROAMING);

    visibility_area = area;

    hitbox = AABB(pos, width, height);

    InitPattern();

    return AppStatus::OK;
}

void FakeWall::InitPattern() {
    const int n = FAKEWALL_ANIM_DELAY * 2;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {FAKEWALL_SPEED_X, 0}, n, (int)FakeWallAnim::ROAMING });
    }
    else {
        pattern.push_back({ {-FAKEWALL_SPEED_X, 0}, n, (int)FakeWallAnim::ROAMING });
    }

    current_step = 0;
    current_frames = 0;
}


bool FakeWall::Update(const AABB& player_box) {
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    if (sprite == nullptr) {
        LOG("Sprite is null in Update");
        return false;
    }

    int anim_id;

    if (state == FakeWallState::ROAMING)
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

void FakeWall::UpdateLook(int anim_id) {
    FakeWallAnim anim = (FakeWallAnim)anim_id;
    look = (anim == FakeWallAnim::ROAMING) ? Look::LEFT : Look::RIGHT;
}