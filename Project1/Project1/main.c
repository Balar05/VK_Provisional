#include "libraries/raylib/include/raylib.h"
#include <stdio.h>

#define EXPLOSION_SIZE 272

Sound soundArray[10];
Music musicArray[10];
Texture2D background;
Texture2D intro;
Texture2D animation2;

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

    // Variables
    const int totalFramesIntro = 3;
    const int totalFramesAnimation2 = 3;
    float currentFrameIntro = 0;
    float currentFrameAnimation2 = 0;
    float framesCounter = 0;
    float framesSpeed = 0.3;
    bool introPlayed = false;
    bool animation2Played = false;
    bool music2Played = false;

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
            if (!music2Played)
            {
                PlayMusicStream(musicArray[0]);
                music2Played = true;
            }
        }

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
        }
        EndDrawing();
    }

    // Unload resources
    UnloadSound(soundArray[0]);
    UnloadMusicStream(musicArray[0]);
    UnloadTexture(background);
    UnloadTexture(intro);
    UnloadTexture(animation2);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
