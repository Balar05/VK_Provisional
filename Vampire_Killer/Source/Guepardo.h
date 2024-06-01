#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define GUEPARDO_SPEED        1
#define GUEPARDO_ANIM_DELAY   (2*ANIM_DELAY)
#define GUEPARDO_FRAME_SIZE   32

enum class GuepardoState { IDLE, ROAMING, FALLING };
enum class GuepardoAnim {
    IDLE_RIGHT, IDLE_LEFT, ROAMING_RIGHT, ROAMING_LEFT, FALLING_RIGHT, FALLING_LEFT,
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

    void SetTileMap(TileMap* tilemap); // Método para establecer el mapa de colisiones

private:
    void InitPattern();
    void UpdateLook(int anim_id);
    //void StartFalling_NJ();

    GuepardoState state;
    int current_step;
    int current_frames;
    std::vector<GuepardoStep> pattern;

    bool activated;
    AABB detection_area;
    Point dir;

    TileMap* map; // Puntero al TileMap

};
