#include "Guepardo.h"
#include "Sprite.h"


Guepardo::Guepardo(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height) {
    state = GuepardoState::IDLE;
    current_step = 0;
    current_frames = 0;
    dir = { 0, 0 };
}
Guepardo::~Guepardo() {}

AppStatus Guepardo::Initialise(Look look, const AABB& area) {
    const int n = GUEPARDO_FRAME_SIZE;
    state = GuepardoState::IDLE;
    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
    if (render == nullptr) {
        LOG("Failed to allocate memory for guepardo sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)GuepardoAnim::NUM_ANIMATIONS);

    // IDLE animation (ensure this points to the correct part of your sprite sheet)
    sprite->SetAnimationDelay((int)GuepardoAnim::IDLE_RIGHT, GUEPARDO_ANIM_DELAY);
    sprite->AddKeyFrame((int)GuepardoAnim::IDLE_RIGHT, { 0, 96, n, n });
    sprite->SetAnimationDelay((int)GuepardoAnim::IDLE_LEFT, GUEPARDO_ANIM_DELAY);
    sprite->AddKeyFrame((int)GuepardoAnim::IDLE_LEFT, { 0, 96 , -n, n });

    sprite->SetAnimationDelay((int)GuepardoAnim::ROAMING_RIGHT, GUEPARDO_ANIM_DELAY);
    for (int i = 1; i < 4; ++i)
        sprite->AddKeyFrame((int)GuepardoAnim::ROAMING_RIGHT, { (float)i * n, 96, -n, n });
    sprite->SetAnimationDelay((int)GuepardoAnim::ROAMING_LEFT, GUEPARDO_ANIM_DELAY);
    for (int i = 1; i < 4; ++i)
        sprite->AddKeyFrame((int)GuepardoAnim::ROAMING_LEFT, { (float)i * n, 96, n, n });

    sprite->SetAnimationDelay((int)GuepardoAnim::FALLING_RIGHT, GUEPARDO_ANIM_DELAY);
    sprite->AddKeyFrame((int)GuepardoAnim::FALLING_RIGHT, { 2 * n, 3 * n, n, n });
    sprite->SetAnimationDelay((int)GuepardoAnim::FALLING_LEFT, GUEPARDO_ANIM_DELAY);
    sprite->AddKeyFrame((int)GuepardoAnim::FALLING_LEFT, { 2 * n, 3 * n, -n, n });



    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)GuepardoAnim::IDLE_LEFT);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)GuepardoAnim::IDLE_RIGHT);

    visibility_area = area;

    detection_area = AABB({ pos.x-50, pos.y - 200 }, 128, 512);

    InitPattern();

    return AppStatus::OK;
}


void Guepardo::InitPattern() {
    const int n = GUEPARDO_ANIM_DELAY * 2;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {-GUEPARDO_SPEED, 0}, n, (int)GuepardoAnim::ROAMING_LEFT });
    } else {
        pattern.push_back({ {GUEPARDO_SPEED, 0}, n, (int)GuepardoAnim::ROAMING_RIGHT });
    }

    current_step = 0;
    current_frames = 0;
}


bool Guepardo::Update(const AABB& player_box) {
    Sprite* sprite = dynamic_cast<Sprite*>(render);

    // Actualizar la hitbox de detección para que siga al guepardo
    detection_area = AABB({ pos.x - 50, pos.y - 200 }, 128, 512);

    // Verificar si el jugador está dentro del área de detección y si el guepardo no ha sido activado
    if (!activated && detection_area.TestAABB(player_box)) {
        activated = true;
        state = GuepardoState::ROAMING;
        look = (player_box.pos.x < pos.x) ? Look::LEFT : Look::RIGHT; // Actualizar la dirección basada en la posición del jugador
        InitPattern(); // Reinicializar el patrón para moverse en la nueva dirección
    }

    if (state == GuepardoState::ROAMING) {
        pos += pattern[current_step].speed;
        current_frames++;

        if (current_frames == pattern[current_step].frames) {
            current_step++;
            current_step %= pattern.size();
            current_frames = 0;
        }

        sprite->SetAnimation(pattern[current_step].anim);
    }
    else {
        sprite->SetAnimation((int)GuepardoAnim::IDLE_RIGHT);
    }

    sprite->Update();
    return false;
}


void Guepardo::DrawDetectionArea() const {
    // Dibujar el área de detección
    DrawRectangleLines(detection_area.pos.x, detection_area.pos.y, detection_area.width, detection_area.height, GRAY);
}

void Guepardo::UpdateLook(int anim_id)
{
    GuepardoAnim anim = (GuepardoAnim)anim_id;
    look = (anim == GuepardoAnim::IDLE_LEFT ||
        anim == GuepardoAnim::ROAMING_LEFT) ? Look::LEFT : Look::RIGHT;
}

void Guepardo::SetTileMap(TileMap* tilemap) {
    map = tilemap;
}

//void Guepardo::UpdateDirection(const AABB& player_box) {
//    if (player_box.pos.x < pos.x) {
//        look = Look::LEFT;
//    }
//    else {
//        look = Look::RIGHT;
//    }
//
//    InitPattern(); 
//}