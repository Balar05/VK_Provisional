#include "Guepardo.h"
#include "Sprite.h"

Guepardo::Guepardo(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height), verticalSpeed(0.0f) {
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
    sprite->SetAnimationDelay((int)GuepardoAnim::IDLE_RIGHT, 0);
    sprite->AddKeyFrame((int)GuepardoAnim::IDLE_RIGHT, { 0, 96, n, n });
    sprite->SetAnimationDelay((int)GuepardoAnim::IDLE_LEFT, 0);
    sprite->AddKeyFrame((int)GuepardoAnim::IDLE_LEFT, { 0, 96 , -n, n });

    sprite->SetAnimationDelay((int)GuepardoAnim::ROAMING_RIGHT, 10);
    for (int i = 1; i < 4; ++i)
        sprite->AddKeyFrame((int)GuepardoAnim::ROAMING_RIGHT, { (float)i * n, 96, -n, n });  // Aquí n es positivo
    sprite->SetAnimationDelay((int)GuepardoAnim::ROAMING_LEFT, 10);
    for (int i = 1; i < 4; ++i)
        sprite->AddKeyFrame((int)GuepardoAnim::ROAMING_LEFT, { (float)i * n, 96, n, n });  // Aquí n es negativo

    sprite->SetAnimationDelay((int)GuepardoAnim::FALLING_RIGHT, 10);
    sprite->AddKeyFrame((int)GuepardoAnim::FALLING_RIGHT, { 2 * n, 3 * n, n, n });
    sprite->SetAnimationDelay((int)GuepardoAnim::FALLING_LEFT, 10);
    sprite->AddKeyFrame((int)GuepardoAnim::FALLING_LEFT, { 2 * n, 3 * n, -n, n });

    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)GuepardoAnim::IDLE_LEFT);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)GuepardoAnim::IDLE_RIGHT);

    visibility_area = area;

    detection_area = AABB({ pos.x - 50, pos.y - 200 }, 128, 512);

    InitPattern();

    return AppStatus::OK;
}

void Guepardo::InitPattern() {
    const int n = GUEPARDO_ANIM_DELAY * 3;
    pattern.clear();

    if (look == Look::LEFT) {
        pattern.push_back({ {-GUEPARDO_SPEED_X, 0}, n, (int)GuepardoAnim::ROAMING_LEFT });
    }
    else {
        pattern.push_back({ {GUEPARDO_SPEED_X, 0}, n, (int)GuepardoAnim::ROAMING_RIGHT });
    }

    current_step = 0;
    current_frames = 0;
}

bool Guepardo::Update(const AABB& player_box) {
    //si descomento este primer if y decomento la logica de fisica de caida el juego peta
//if (map == nullptr) {   
//    LOG("TileMap is null in Guepardo::Update");
//    return false;
//}

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    if (sprite == nullptr) {
        LOG("Sprite is null in Update");
        return false;
    }

    // Actualizar la hitbox de detección para que siga al guepardo
    detection_area = AABB({ pos.x - 50, pos.y - 200 }, 128, 512);

    // Verificar si el jugador está dentro del área de detección y si el guepardo no ha sido activado
    if (!activated && detection_area.TestAABB(player_box)) {
        state = GuepardoState::ROAMING;
        activated = true;
        look = (player_box.pos.x < pos.x) ? Look::LEFT : Look::RIGHT; // Actualizar la dirección basada en la posición del jugador
        InitPattern(); // Reinicializar el patrón para moverse en la nueva dirección
    }

    int anim_id;

    if (state == GuepardoState::ROAMING)
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

    //// fisica de caida
    //AABB box = GetHitbox();
    //pos.y += verticalSpeed;

    //if (!map->TestCollisionGround(box, &pos.y)) {
    //    if (verticalSpeed < MAX_FALL_SPEED) {
    //        verticalSpeed += GRAVITY;
    //    }
    //    state = GuepardoState::FALLING;
    //}
    //else {
    //    verticalSpeed = 0.0f;
    //    if (state == GuepardoState::FALLING) {
    //        state = GuepardoState::IDLE;
    //    }
    //}

    //if (state == GuepardoState::FALLING) {
    //    if (IsLookingRight()) {
    //        sprite->SetAnimation((int)GuepardoAnim::FALLING_RIGHT);
    //    }
    //    else {
    //        sprite->SetAnimation((int)GuepardoAnim::FALLING_LEFT);
    //    }
    //}

    sprite->Update();
    return false;
}

void Guepardo::DrawDetectionArea() const {
    DrawRectangleLines(detection_area.pos.x, detection_area.pos.y, detection_area.width, detection_area.height, GRAY);
}

void Guepardo::UpdateLook(int anim_id) {
    GuepardoAnim anim = (GuepardoAnim)anim_id;
    look = (anim == GuepardoAnim::IDLE_LEFT || anim == GuepardoAnim::ROAMING_LEFT) ? Look::LEFT : Look::RIGHT;
}

void Guepardo::SetTileMap(TileMap* tilemap) {
    if (tilemap == nullptr) {
        LOG("Attempted to set a null TileMap in Guepardo::SetTileMap");
        return;
    }
    map = tilemap;
}

//void Guepardo::StartFalling_NJ() {
//    dir.y = GUEPARDO_SPEED;
//    state = GuepardoState::FALLING;
//    if (IsLookingRight()) {
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::FALLING_RIGHT);
//    }
//    else {
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::FALLING_LEFT);
//    }
//}
//
//bool Guepardo::IsAscending() const {
//    return dir.y < -GUEPARDO_SPEED;
//}
//
//bool Guepardo::IsLevitating() const {
//    return abs(dir.y) <= GUEPARDO_SPEED;
//}
//
//bool Guepardo::IsDescending() const {
//    return dir.y > GUEPARDO_SPEED;
//}

//void Guepardo::ChangeAnimRight() {
//    look = Look::RIGHT;
//    switch (state) {
//    case GuepardoState::IDLE:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::IDLE_RIGHT);
//        break;
//    case GuepardoState::ROAMING:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::ROAMING_RIGHT);
//        break;
//    case GuepardoState::FALLING:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::FALLING_RIGHT);
//        break;
//    }
//}
//
//void Guepardo::ChangeAnimLeft() {
//    look = Look::LEFT;
//    switch (state) {
//    case GuepardoState::IDLE:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::IDLE_LEFT);
//        break;
//    case GuepardoState::ROAMING:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::ROAMING_LEFT);
//        break;
//    case GuepardoState::FALLING:
//        dynamic_cast<Sprite*>(render)->SetAnimation((int)GuepardoAnim::FALLING_LEFT);
//        break;
//    }
//}
//
//bool Guepardo::IsLookingRight() const {
//    return look == Look::RIGHT;
//}
//
//bool Guepardo::IsLookingLeft() const {
//    return look == Look::LEFT;
//}


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