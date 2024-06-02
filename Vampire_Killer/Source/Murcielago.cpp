#include "Murcielago.h"
#include "Sprite.h"

Murcielago::Murcielago(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height), base_y(p.y) {
    state = MurcielagoState::ROAMING;
    current_step = 0;
    current_frames = 0;
    dir = { 0, 0 };
}
Murcielago::~Murcielago() {}

AppStatus Murcielago::Initialise(Look look, const AABB& area) {
    const int n = MURCIELAGO_FRAME_SIZE;
    state = MurcielagoState::ROAMING;
    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
    if (render == nullptr) {
        LOG("Failed to allocate memory for guepardo sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)MurcielagoAnim::NUM_ANIMATIONS);

    sprite->SetAnimationDelay((int)MurcielagoAnim::ROAMING_RIGHT, MURCIELAGO_ANIM_DELAY);
    for (int i = 0; i < 4; ++i)
        sprite->AddKeyFrame((int)MurcielagoAnim::ROAMING_RIGHT, { (float)i * n, 32, -n, n });
    sprite->SetAnimationDelay((int)MurcielagoAnim::ROAMING_LEFT, MURCIELAGO_ANIM_DELAY);
    for (int i = 0; i < 4; ++i)
        sprite->AddKeyFrame((int)MurcielagoAnim::ROAMING_LEFT, { (float)i * n, 32 , n, n });

    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)MurcielagoAnim::ROAMING_RIGHT);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)MurcielagoAnim::ROAMING_LEFT);

    visibility_area = area;

    InitPattern();

    return AppStatus::OK;
}

void Murcielago::InitPattern() {
    const int n = MURCIELAGO_ANIM_DELAY * 4;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {MURCIELAGO_SPEED_X, 0}, n, (int)MurcielagoAnim::ROAMING_RIGHT });
    }
    else {
        pattern.push_back({ {-MURCIELAGO_SPEED_X, 0}, n, (int)MurcielagoAnim::ROAMING_LEFT  });
    }

    current_step = 0;
    current_frames = 0;
}


bool Murcielago::Update(const AABB& player_box) {
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    if (sprite == nullptr) {
        LOG("Sprite is null in Update");
        return false;
    }

    if (state == MurcielagoState::ROAMING) {
        // Actualizar la posición X
        pos.x += pattern[current_step].speed.x;

  
        pos.y = base_y + MURCIELAGO_WAVE_AMPLITUDE * std::sin(current_frames * MURCIELAGO_WAVE_FREQUENCY);

        current_frames++;
        if (current_frames >= pattern[current_step].frames) {
            current_step++;
            current_step %= pattern.size();
            current_frames = 0;

            int anim_id = pattern[current_step].anim;
            sprite->SetAnimation(anim_id);
        }
    }

    sprite->Update();
    return false;
}

void Murcielago::UpdateLook(int anim_id) {
    MurcielagoAnim anim = (MurcielagoAnim)anim_id;
    look = ( anim == MurcielagoAnim::ROAMING_LEFT) ? Look::LEFT : Look::RIGHT;
}


