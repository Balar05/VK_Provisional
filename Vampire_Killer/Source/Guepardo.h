#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define GUEPARDO_SPEED_X       2
#define GUEPARDO_SPEED_Y       2
#define GUEPARDO_JUMP_FORCE    3
#define GUEPARDO_ANIM_DELAY    (2*ANIM_DELAY)
#define GUEPARDO_FRAME_SIZE    32
#define GRAVITY                0.2f
#define MAX_FALL_SPEED         5.0f

enum class GuepardoState { IDLE, ROAMING, JUMPING, FALLING, ATTACKING };
enum class GuepardoAnim {
    IDLE_RIGHT, IDLE_LEFT, ROAMING_RIGHT, ROAMING_LEFT, JUMPING_RIGHT, JUMPING_LEFT, FALLING_RIGHT, FALLING_LEFT,
    NUM_ANIMATIONS
};

struct GuepardoStep {
    Point speed;
    int frames;
    int anim;
};

class Guepardo : public Enemy {
public:
    Guepardo(const Point& p, int width, int height, int frame_width, int frame_height);
    ~Guepardo();

    AppStatus Initialise(Look look, const AABB& area) override;
    bool Update(const AABB& box) override;
    void DrawDetectionArea() const;

    void SetTileMap(TileMap* tilemap); 


private:
    void InitPattern();
    void UpdateLook(int anim_id);

    GuepardoState state;
    int current_step;
    int current_frames;
    std::vector<GuepardoStep> pattern;

    bool activated;
    AABB detection_area;
    Point dir;

    TileMap* map; 

    float verticalSpeed;
};
