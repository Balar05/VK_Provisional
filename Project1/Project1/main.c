#include "libraries/raylib/include/raylib.h"
#include <stdio.h>

#define EXPLOSION_SIZE 272
#define SPRITE_SIZE 32
#define CLOUD_SIZE 64

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

int main(void)
{
    // Initialization
    const int screenWidth = 550;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Vampire Killer MSX2");
    InitAudioDevice();

    // Load resources
    soundArray[0] = LoadSound("resources/Audio/Whip-Sound Effect.wav");
    musicArray[0] = LoadMusicStream("resources/Audio/Prologue.ogg");
    background = LoadTexture("resources/sprites/space_background.png");
    intro = LoadTexture("resources/images/Sprites/intro1.png");
    animation2 = LoadTexture("resources/images/Sprites/intro2.png");
    characterFront = LoadTexture("resources/images/Sprites/simonbelmont.png"); // Cargar la textura frontal del personaje
    characterBack = LoadTexture("resources/images/Sprites/simonbelmont.png");  // Cargar la textura de espaldas del personaje
    cloudTexture = LoadTexture("resources/images/Sprites/cloud.png");    // Cargar la textura de la nube
    bat_intro = LoadTexture("resources/images/Sprites/batanim.png"); // Cargar la nueva textura
    bat_intro2 = LoadTexture("resources/images/Sprites/batanim.png"); // Cargar la nueva textura

    // Variables
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
    Vector2 characterPosition = { screenWidth - 100, screenHeight / 2 + 50 }; // Inicialmente fuera de la pantalla

    // Posición de la nube
    Vector2 cloudPosition = { screenWidth - 80, 100 }; // Posición inicial de la nube

    // Posición de la segunda animación
    Vector2 bat_introPosition = { screenWidth / 2 + 20, 130 }; // Posición inicial del murciélago

    Vector2 bat_intro2Position = { screenWidth / 2 - 180, 200 }; // Posición inicial del murciélago

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        if (IsKeyPressed(KEY_SPACE) && !introPlayed)
            PlaySound(soundArray[0]);

        if (!introPlayed)
        {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrameIntro++;
                if (currentFrameIntro >= totalFramesIntro)
                {
                    framesSpeed = 10;
                    currentFrameIntro = totalFramesIntro - 1;
                    if (IsKeyDown(KEY_SPACE))
                        introPlayed = true;
                }
            }
        }
        else if (!animation2Played)
        {
            // Actualizar la animación 2
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrameAnimation2++;
                if (currentFrameAnimation2 >= totalFramesAnimation2)
                {
                    framesSpeed = 10;
                    currentFrameAnimation2 = totalFramesAnimation2 - 1;
                    if (IsKeyDown(KEY_SPACE))
                        animation2Played = true;
                }
            }

            // Movimiento del personaje hacia el centro
            if (characterPosition.x > screenWidth / 2 && !characterStopped)
            {
                characterPosition.x -= 0.8f; // ajustar la velocidad
            }
            else
            {
                // Detener la animación de caminar
                characterStopped = true;

                // Cambiar la textura del personaje a la de espaldas
                characterFrontFacing = false;
            }

            // Actualizar la animación del personaje
            if (!characterStopped)
            {
                framesCounter++;
                if (framesCounter >= (60 / framesSpeed))
                {
                    framesCounter = 0;
                    currentFrameCharacterFront++;
                    if (currentFrameCharacterFront >= totalFramesCharacterFront)
                    {
                        framesSpeed = 1;
                        currentFrameCharacterFront = 0; // Reiniciar la animación frontal
                    }

                }
            }
            
            else if (!animationbatPlayed)
            {
                framesCounter2++;
                if (framesCounter2 >= (60 / framesSpeed2))
                {
                    framesCounter2 = 0;
                    currentFramebat_intro++;
                    currentFramebat_intro2++;
                    if (currentFramebat_intro >= totalFramesbat_intro)
                    {
                        framesSpeed2 = 3;
                        currentFramebat_intro = 0;
                    }
                    else if (currentFramebat_intro2 >= totalFramesbat_intro2)
                    {
                        framesSpeed2 = 3;
                        currentFramebat_intro2 = 0;
                    }
                }
            }

            // Actualizar la posición del murciélago
            bat_introPosition.x -= 0.2; // Desplazar el murciélago hacia la izquierda
            bat_intro2Position.x += 0.2;
            bat_intro2Position.y -= 0.1;

            // Actualizar los fotogramas de animación de los murciélagos
            framesCounter2++;
            if (framesCounter2 >= (60 / framesSpeed2))
            {
                framesCounter2 = 0;
                currentFramebat_intro++;
                currentFramebat_intro2++;
                if (currentFramebat_intro >= totalFramesbat_intro)
                {
                    framesSpeed2 = 5;
                    currentFramebat_intro = 0;
                }
                if (currentFramebat_intro2 >= totalFramesbat_intro2)
                {
                    framesSpeed2 = 5;
                    currentFramebat_intro2 = 0;
                }
            }

        }

        // Update Music Stream
        UpdateMusicStream(musicArray[0]);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        if (!introPlayed)
        {
            Rectangle source = { currentFrameIntro * EXPLOSION_SIZE, 0, EXPLOSION_SIZE, EXPLOSION_SIZE }; // Solo necesitas un frame de altura para la textura de introducción
            Rectangle dest = { screenWidth / 2, screenHeight / 2 + 50, EXPLOSION_SIZE * 2, EXPLOSION_SIZE * 2 };
            DrawTexturePro(intro, source, dest, (Vector2) { dest.width / 2, dest.height / 2 }, 0, WHITE);
        }
        else if (!animation2Played)
        {
            Rectangle source = { currentFrameAnimation2 * EXPLOSION_SIZE, 0, EXPLOSION_SIZE, EXPLOSION_SIZE };
            Rectangle dest = { screenWidth / 2, screenHeight / 2 + 50, EXPLOSION_SIZE * 2, EXPLOSION_SIZE * 2 };
            DrawTexturePro(animation2, source, dest, (Vector2) { dest.width / 2, dest.height / 2 }, 0, WHITE);

            // Dibujar la animación del personaje
            if (characterFrontFacing)
            {
                Rectangle sourceCharacter = { currentFrameCharacterFront * SPRITE_SIZE, 0, -SPRITE_SIZE, SPRITE_SIZE };
                Rectangle destCharacter = { characterPosition.x + SPRITE_SIZE, characterPosition.y, SPRITE_SIZE * 2, SPRITE_SIZE * 2 };
                DrawTexturePro(characterFront, sourceCharacter, destCharacter, (Vector2) { destCharacter.width / 2, destCharacter.height / 2 - 110 }, 0, WHITE);
            }
            else
            {
                Rectangle sourceCharacter = { currentFrameCharacterBack * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
                Rectangle destCharacter = { characterPosition.x, characterPosition.y, SPRITE_SIZE * 2, SPRITE_SIZE * 2 };
                DrawTexturePro(characterBack, sourceCharacter, destCharacter, (Vector2) { destCharacter.width / 2, destCharacter.height / 2 - 110 }, 0, WHITE);
            }

            // Dibujar la nube (aumentada en tamaño)
            Rectangle cloudSource = { 0, 0, CLOUD_SIZE, CLOUD_SIZE };
            Rectangle cloudDest = { cloudPosition.x, cloudPosition.y, CLOUD_SIZE * 2, CLOUD_SIZE * 2 }; // Doble tamaño
            DrawTexturePro(cloudTexture, cloudSource, cloudDest, (Vector2) { cloudDest.width / 2, cloudDest.height / 2 - 60 }, 0, WHITE);

            // Dibujar la segunda animación del murciélago
            Rectangle batSource1 = { (int)currentFramebat_intro * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
            Rectangle batDest1 = { (int)bat_introPosition.x, (int)bat_introPosition.y, SPRITE_SIZE * 2, SPRITE_SIZE * 2 };
            DrawTexturePro(bat_intro, batSource1, batDest1, (Vector2) { batDest1.width / 2, batDest1.height / 2 }, 0, WHITE);

            // Dibujar la segunda animación del murciélago
            Rectangle batSource2 = { (int)currentFramebat_intro2 * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
            Rectangle batDest2 = { (int)bat_intro2Position.x, (int)bat_intro2Position.y, SPRITE_SIZE * 2, SPRITE_SIZE * 2 };
            DrawTexturePro(bat_intro2, batSource2, batDest2, (Vector2) { batDest2.width / 2, batDest2.height / 2 }, 0, WHITE);
        }

        EndDrawing();
    }

    // Unload resources
    UnloadSound(soundArray[0]);
    UnloadMusicStream(musicArray[0]);
    UnloadTexture(background);
    UnloadTexture(intro);
    UnloadTexture(animation2);
    UnloadTexture(characterFront); // Descargar la textura frontal del personaje
    UnloadTexture(characterBack);  // Descargar la textura de espaldas del personaje
    UnloadTexture(cloudTexture);    // Descargar la textura de la nube
    UnloadTexture(bat_intro); // Descargar la nueva textura
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

