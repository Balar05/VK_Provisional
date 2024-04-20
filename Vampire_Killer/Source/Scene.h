#pragma once
#include <raylib.h>
#include "Player.h"
#include "Zombie.h"
#include "TileMap.h"
#include "Object.h"
#include "LevelBackground.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void UpdateBackground(int s);
    void Render();
    void Release();

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player* player;
    TileMap* level;

    LevelBackground* background;

    std::vector<Object*> objects;

    Camera2D camera;
    DebugMode debug;

    //Texture2D background = LoadTexture("images/Sprites/256x176 Levels.png");
    Rectangle rc;
    int currentStage;
};