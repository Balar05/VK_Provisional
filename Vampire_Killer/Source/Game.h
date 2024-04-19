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
    const int totalFramesCharacterFront = 3; // N�mero total de fotogramas para la animaci�n frontal del personaje
    const int totalFramesCharacterBack = 1;  // N�mero total de fotogramas para la animaci�n de espaldas del personaje
    const int totalFramesbat_intro = 2;
    const int totalFramesbat_intro2 = 2;
    float currentFrameIntro = 0;
    float currentFrameAnimation2 = 0;
    float currentFrameCharacterFront = 0; // Fotograma actual para la animaci�n frontal del personaje
    float currentFrameCharacterBack = 5;  // Fotograma actual para la animaci�n de espaldas del personaje
    float currentFramebat_intro = 0;
    float currentFramebat_intro2 = 0;
    float framesCounter = 0;
    float framesCounter2 = 0;
    float framesSpeed2 = 1;
    float framesSpeed = 1;
    bool introPlayed = false;
    bool animation2Played = false;
    bool music2Played = false;
    bool characterFrontFacing = true; // Variable para controlar si el personaje est� de frente o de espaldas
    bool characterStopped = false;    // Variable para controlar si el personaje ha detenido su animaci�n de caminar
    bool animationbatPlayed = false;
    bool animationbat2Played = false;
    // Posici�n del personaje
    Vector2 characterPosition = { WINDOW_WIDTH - 100, WINDOW_HEIGHT / 2 + 50 }; // Inicialmente fuera de la pantalla

    // Posici�n de la nube
    Vector2 cloudPosition = { WINDOW_WIDTH - 80, 100 }; // Posici�n inicial de la nube

    // Posici�n de la segunda animaci�n
    Vector2 bat_introPosition = { WINDOW_WIDTH / 2 + 20, 130 }; // Posici�n inicial del murci�lago

    Vector2 bat_intro2Position = { WINDOW_WIDTH / 2 - 180, 200 }; // Posici�n inicial del murci�lago
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
    Texture2D characterFront; // Textura para la animaci�n frontal del personaje
    Texture2D characterBack;  // Textura para la animaci�n de espaldas del personaje
    Texture2D cloudTexture;   // Textura para la animaci�n de la nube
    Texture2D bat_intro; // Nueva textura para la segunda animaci�n
    Texture2D bat_intro2; // Nueva textura para la segunda animaci�n

};