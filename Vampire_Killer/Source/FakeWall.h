#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define FAKEWALL_SPEED_X       2
#define FAKEWALL_SPEED_Y       2
#define FAKEWALLJUMP_FORCE    3
#define FAKEWALL_ANIM_DELAY    (3*ANIM_DELAY)
#define FAKEWALL_FRAME_SIZE    32


enum class FakeWallState { IDLE, ROAMING };
enum class FakeWallAnim {
    IDLE, ROAMING,
    NUM_ANIMATIONS
};

struct FakeWallStep {
    Point speed;
    int frames;
    int anim;
};

class FakeWall : public Enemy {
public:
    FakeWall(const Point& p, int width, int height, int frame_width, int frame_height);
    ~FakeWall();

    AppStatus Initialise(Look look, const AABB& area) override;
    bool Update(const AABB& box) override;
    bool CausesDamage() const override { return false; }
;

private:
    void InitPattern();
    void UpdateLook(int anim_id);

    FakeWallState state;
    int current_step;
    int current_frames;
    std::vector<FakeWallStep> pattern;

    float base_y;
 
    Point dir;

    TileMap* map; // Puntero al TileMap
    AABB hitbox;

};


