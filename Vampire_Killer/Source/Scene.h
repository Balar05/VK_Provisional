#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "LevelBackground.h"
#include "EnemyManager.h"
#include "text.h"


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
    int returnStage();
    int currentStage;
    bool exit;

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void GenerateZombies();
    void GenerateGuepardos();
    void GenerateMurcielagos();
    void GenerateLLama();
    void GenerateCandle();
    void GenerateFakeWall();
    void RenderGUI() const;
    std::vector<ObjectType> collectedObjects;

    Player* player;
    TileMap* level;

    LevelBackground* background;

    std::vector<Object*> objects;

    Camera2D camera;
    DebugMode debug;
    EnemyManager* enemies;
    //Texture2D background = LoadTexture("images/Sprites/256x176 Levels.png");
    Rectangle rc;
    
    int frameCounter;

    bool guepardoGenerated;
    bool llamaGenerated;
    bool candleGenerated;
    bool fakewallGenerated;


    Text* font1;
};
