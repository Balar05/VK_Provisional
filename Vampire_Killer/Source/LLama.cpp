#include "LLama.h"
#include "Sprite.h"

LLama::LLama(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height) {
    state = LLamaState::IDLE;
    current_step = 0;
    current_frames = 0;
    dir = { 0, 0 };
}
LLama::~LLama() {}

AppStatus LLama::Initialise(Look look, const AABB& area) {
    const int n = LLAMA_FRAME_SIZE;
    state = LLamaState::IDLE;
    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_LLAMA));
    if (render == nullptr) {
        LOG("Failed to allocate memory for guepardo sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)LLamaAnim::NUM_ANIMATIONS);

    sprite->SetAnimationDelay((int)LLamaAnim::ROAMING, 15);
    for (int i = 0; i < 2; ++i)
        sprite->AddKeyFrame((int)LLamaAnim::ROAMING, { (float)i * n, 16 , n, n });


    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)LLamaAnim::ROAMING);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)LLamaAnim::ROAMING);

    visibility_area = area;

    InitPattern();

    return AppStatus::OK;
}

void LLama::InitPattern() {
    const int n = LLAMA_ANIM_DELAY * 2;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {LLAMA_SPEED_X, 0}, n, (int)LLamaAnim::ROAMING });
    }
    else {
        pattern.push_back({ {-LLAMA_SPEED_X, 0}, n, (int)LLamaAnim::ROAMING });
    }

    current_step = 0;
    current_frames = 0;
}


bool LLama::Update(const AABB& player_box) {
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    if (sprite == nullptr) {
        LOG("Sprite is null in Update");
        return false;
    }

    int anim_id;

    if (state == LLamaState::ROAMING)
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

void LLama::UpdateLook(int anim_id) {
    LLamaAnim anim = (LLamaAnim)anim_id;
    look = (anim == LLamaAnim::ROAMING) ? Look::LEFT : Look::RIGHT;
}