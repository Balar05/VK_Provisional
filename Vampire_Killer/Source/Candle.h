#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define CANDLE_SPEED_X       2
#define CANDLE_SPEED_Y       2
#define CANDLEJUMP_FORCE    3
#define CANDLE_ANIM_DELAY    (3*ANIM_DELAY)
#define CANDLE_FRAME_SIZE    16
//#define MURCIELAGO_WAVE_AMPLITUDE 15  // Amplitud de la onda
//#define MURCIELAGO_WAVE_FREQUENCY 0.1  // Frecuencia de la onda,

enum class CandleState { IDLE, ROAMING };
enum class CandleAnim {
    IDLE, ROAMING,
    NUM_ANIMATIONS
};

struct CandleStep {
    Point speed;
    int frames;
    int anim;
};

class Candle : public Enemy {
public:
    Candle(const Point& p, int width, int height, int frame_width, int frame_height);
    ~Candle();

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

    CandleState state;
    int current_step;
    int current_frames;
    std::vector<CandleStep> pattern;

    float base_y;
    //bool activated;
    //AABB detection_area;
    Point dir;

    TileMap* map; // Puntero al TileMap

    //float verticalSpeed; // Nueva variable para manejar la velocidad vertical
};
