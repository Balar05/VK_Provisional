#include "Candle.h"
#include "Sprite.h"

Candle::Candle(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height) {
    state = CandleState::IDLE;
    current_step = 0;
    current_frames = 0;
    dir = { 0, 0 };
}
Candle::~Candle() {}

AppStatus Candle::Initialise(Look look, const AABB& area) {
    const int n = CANDLE_FRAME_SIZE;
    state = CandleState::IDLE;
    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_CANDLE));
    if (render == nullptr) {
        LOG("Failed to allocate memory for guepardo sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)CandleAnim::NUM_ANIMATIONS);

    sprite->SetAnimationDelay((int)CandleAnim::ROAMING, 15);
    for (int i = 0; i < 2; ++i)
        sprite->AddKeyFrame((int)CandleAnim::ROAMING, { (float)i * n, 16 , n, n });


    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)CandleAnim::ROAMING);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)CandleAnim::ROAMING);

    visibility_area = area;

    InitPattern();

    return AppStatus::OK;
}

void Candle::InitPattern() {
    const int n = CANDLE_ANIM_DELAY * 2;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {CANDLE_SPEED_X, 0}, n, (int)CandleAnim::ROAMING });
    }
    else {
        pattern.push_back({ {-CANDLE_SPEED_X, 0}, n, (int)CandleAnim::ROAMING });
    }

    current_step = 0;
    current_frames = 0;
}


bool Candle::Update(const AABB& player_box) {
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    if (sprite == nullptr) {
        LOG("Sprite is null in Update");
        return false;
    }

    int anim_id;

    if (state == CandleState::ROAMING)
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

void Candle::UpdateLook(int anim_id) {
    CandleAnim anim = (CandleAnim)anim_id;
    look = (anim == CandleAnim::ROAMING) ? Look::LEFT : Look::RIGHT;
}
