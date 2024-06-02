#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define MURCIELAGO_SPEED_X       2
#define MURCIELAGO_SPEED_Y       2
#define MURCIELAGO_JUMP_FORCE    3
#define MURCIELAGO_ANIM_DELAY    (3*ANIM_DELAY)
#define MURCIELAGO_FRAME_SIZE    32
#define MURCIELAGO_WAVE_AMPLITUDE 15  // Amplitud de la onda
#define MURCIELAGO_WAVE_FREQUENCY 0.1  // Frecuencia de la onda,

enum class MurcielagoState {IDLE, ROAMING};
enum class MurcielagoAnim {
    IDLE_RIGHT, IDLE_LEFT,ROAMING_RIGHT, ROAMING_LEFT,
    NUM_ANIMATIONS
};

struct MurcielagoStep {
    Point speed;
    int frames;
    int anim;
};

class Murcielago : public Enemy {
public:
    Murcielago(const Point& p, int width, int height, int frame_width, int frame_height);
    ~Murcielago();

    AppStatus Initialise(Look look, const AABB& area) override;
    bool Update(const AABB& box) override;
    // void DrawDetectionArea() const;

    //void SetTileMap(TileMap* tilemap); // Método para establecer el mapa de colisiones

    //void MoveX();
    //void MoveY();
    //void LogicJumping();

    //void StartFalling_NJ();
    //bool IsAscending() const;
    //bool IsLevitating() const;
    //bool IsDescending() const;
    //void ChangeAnimRight();
    //void ChangeAnimLeft();
    //bool IsLookingRight() const;
    //bool IsLookingLeft() const;

private:
    void InitPattern();
    void UpdateLook(int anim_id);

    MurcielagoState state;
    int current_step;
    int current_frames;
    std::vector<MurcielagoStep> pattern;

    float base_y;
    //bool activated;
    //AABB detection_area;
    Point dir;

    TileMap* map; // Puntero al TileMap

    //float verticalSpeed; // Nueva variable para manejar la velocidad vertical
};