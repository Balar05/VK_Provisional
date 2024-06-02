#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, INTRO, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    void ResetAnimations();
    void ResetCredits();

    const int totalFramesIntro = 3;
    const int totalFramesAnimation2 = 3;
    const int totalFramesCharacterFront = 3; 
    const int totalFramesCharacterBack = 1; 
    const int totalFramesbat_intro = 2;
    const int totalFramesbat_intro2 = 2;
    float currentFrameIntro = 0;
    float currentFrameAnimation2 = 0;
    float currentFrameCharacterFront = 0; 
    float currentFrameCharacterBack = 5; 
    float currentFramebat_intro = 0;
    float currentFramebat_intro2 = 0;
    float framesCounter = 0;
    float framesCounter2 = 0;
    float framesSpeed2 = 1;
    float framesSpeed = 1;
    bool introPlayed = false;
    bool animation2Played = false;
    bool music1Played = false;
    bool music2Played = false;
    bool music3Played = false;
    bool characterFrontFacing = true;
    bool characterStopped = false;   
    bool animationbatPlayed = false;
    bool animationbat2Played = false;
    bool creditsFinished = false;

    float creditPositionY = WINDOW_HEIGHT;
    // Posición del personaje
    Vector2 characterPosition = { WINDOW_WIDTH - 30, 95 }; // Inicialmente fuera de la pantalla

    // Posición de la nube
    Vector2 cloudPosition = { WINDOW_WIDTH - 60, 100 }; // Posición inicial de la nube

    // Posición de la segunda animación
    Vector2 bat_introPosition = { WINDOW_WIDTH / 2 + 20, 70 }; // Posición inicial del murciélago

    Vector2 bat_intro2Position = { WINDOW_WIDTH / 2 - 100, 120 }; // Posición inicial del murciélago
    


private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_menu;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;


    Sound soundArray[10];
    Music musicArray[10];

    Texture2D background;
    Texture2D intro;
    //Texture2D animation2;
    Texture2D castle;
    Texture2D characterFront; 
    Texture2D characterBack; 
    Texture2D cloudTexture;  
    Texture2D bat_intro; 
    Texture2D bat_intro2; 
    Texture2D creditImage;



};