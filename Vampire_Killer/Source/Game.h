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

    const int totalFramesIntro = 3;
    const int totalFramesAnimation2 = 3;
    const int totalFramesCharacterFront = 3; // Número total de fotogramas para la animación frontal del personaje
    const int totalFramesCharacterBack = 1;  // Número total de fotogramas para la animación de espaldas del personaje
    const int totalFramesbat_intro = 2;
    const int totalFramesbat_intro2 = 2;
    float currentFrameIntro = 0;
    float currentFrameAnimation2 = 0;
    float currentFrameCharacterFront = 0; // Fotograma actual para la animación frontal del personaje
    float currentFrameCharacterBack = 5;  // Fotograma actual para la animación de espaldas del personaje
    float currentFramebat_intro = 0;
    float currentFramebat_intro2 = 0;
    float framesCounter = 0;
    float framesCounter2 = 0;
    float framesSpeed2 = 1;
    float framesSpeed = 1;
    bool introPlayed = false;
    bool animation2Played = false;
    bool music2Played = false;
    bool characterFrontFacing = true; // Variable para controlar si el personaje está de frente o de espaldas
    bool characterStopped = false;    // Variable para controlar si el personaje ha detenido su animación de caminar
    bool animationbatPlayed = false;
    bool animationbat2Played = false;
    // Posición del personaje
    Vector2 characterPosition = { WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 + 50 }; // Inicialmente fuera de la pantalla

    // Posición de la nube
    Vector2 cloudPosition = { WINDOW_WIDTH - 80, 100 }; // Posición inicial de la nube

    // Posición de la segunda animación
    Vector2 bat_introPosition = { WINDOW_WIDTH / 2 + 20, 130 }; // Posición inicial del murciélago

    Vector2 bat_intro2Position = { WINDOW_WIDTH / 2 - 180, 200 }; // Posición inicial del murciélago
private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_menu;
    const Texture2D* img_intro;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;


    Sound soundArray[10];
    Music musicArray[10];

    Texture2D background;
    Texture2D intro;
    Texture2D animation2;
    Texture2D characterFront; // Textura para la animación frontal del personaje
    Texture2D characterBack;  // Textura para la animación de espaldas del personaje
    Texture2D cloudTexture;   // Textura para la animación de la nube
    Texture2D bat_intro; // Nueva textura para la segunda animación
    Texture2D bat_intro2; // Nueva textura para la segunda animación

};