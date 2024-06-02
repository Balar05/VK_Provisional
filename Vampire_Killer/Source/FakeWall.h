#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define FAKEWALL_SPEED_X       2
#define FAKEWALL_SPEED_Y       2
#define FAKEWALLJUMP_FORCE    3
#define FAKEWALL_ANIM_DELAY    (3*ANIM_DELAY)
#define FAKEWALL_FRAME_SIZE    32
//#define MURCIELAGO_WAVE_AMPLITUDE 15  // Amplitud de la onda
//#define MURCIELAGO_WAVE_FREQUENCY 0.1  // Frecuencia de la onda,

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

    FakeWallState state;
    int current_step;
    int current_frames;
    std::vector<FakeWallStep> pattern;

    float base_y;
    //bool activated;
    //AABB detection_area;
    Point dir;

    TileMap* map; // Puntero al TileMap
    AABB hitbox;

    //float verticalSpeed; // Nueva variable para manejar la velocidad vertical
};


