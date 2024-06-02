#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define LLAMA_SPEED_X       2
#define LLAMA_SPEED_Y       2
#define LLAMA_JUMP_FORCE    3
#define LLAMA_ANIM_DELAY    (3*ANIM_DELAY)
#define LLAMA_FRAME_SIZE    16


enum class LLamaState { IDLE, ROAMING };
enum class LLamaAnim {
    IDLE, ROAMING,
    NUM_ANIMATIONS
};

struct LLamaStep {
    Point speed;
    int frames;
    int anim;
};

class LLama : public Enemy {
public:
    LLama(const Point& p, int width, int height, int frame_width, int frame_height);
    ~LLama();

    AppStatus Initialise(Look look, const AABB& area) override;
    bool Update(const AABB& box) override;
    bool CausesDamage() const override { return false; }

private:
    void InitPattern();
    void UpdateLook(int anim_id);

    LLamaState state;
    int current_step;
    int current_frames;
    std::vector<LLamaStep> pattern;

    float base_y;

    Point dir;

    TileMap* map; // Puntero al TileMap

};
