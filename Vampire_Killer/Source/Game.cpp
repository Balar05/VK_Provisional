#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>


Game::Game()
{
    state = GameState::MAIN_MENU;
    scene = nullptr;
    img_menu = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vampire Killer MSX2");
    InitAudioDevice();
    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    musicArray[0] = LoadMusicStream("Audio/Prologue.ogg");
    musicArray[1] = LoadMusicStream("Audio/Vampire Killer.ogg");
    musicArray[2] = LoadMusicStream("Audio/Ending.ogg");
    intro = LoadTexture("images/Sprites/Intro 256x212.png");
    //animation2 = LoadTexture("images/Sprites/intro2.png");
    castle = LoadTexture("images/Sprites/Castle Background 256x212.png");
    characterFront = LoadTexture("images/Sprites/simonbelmont.png"); 
    characterBack = LoadTexture("images/Sprites/simonbelmont.png"); 
    cloudTexture = LoadTexture("images/Sprites/cloud.png");    
    bat_intro = LoadTexture("images/Sprites/batanim.png"); 
    bat_intro2 = LoadTexture("images/Sprites/batanim.png");
    creditImage = LoadTexture("images/Sprites/Ending.png");

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::MAIN_MENU:
    {
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (BeginPlay() != AppStatus::OK)   return AppStatus::ERROR;
        if (IsKeyPressed(KEY_SPACE))
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;

            state = GameState::INTRO;
        }
        break;
    }

    case GameState::INTRO:
    {
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;

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
                }
            }

            if (!music1Played) {
                PlayMusicStream(musicArray[0]);
                float timePlayed = GetMusicTimePlayed(musicArray[0]);
                // Actualiza la música en cada iteración del bucle principal
                UpdateMusicStream(musicArray[0]);

                // Controla el tiempo de reproducción si es necesario
                if (timePlayed > 6.5) {
                    StopMusicStream(musicArray[0]);
                    music1Played = true;
                    ResetAnimations();
                    state = GameState::PLAYING;

                }
            }

            // Movimiento del personaje hacia el centro
            if (characterPosition.x > WINDOW_WIDTH / 2 && !characterStopped)
            {
                characterPosition.x -= 0.5f; // ajustar la velocidad

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

            cloudPosition.x -= 0.1;
            // Actualizar la posición del murciélago
            bat_introPosition.x -= 0.15; // Desplazar el murciélago hacia la izquierda
            bat_intro2Position.x += 0.15;
            bat_intro2Position.y -= 0.08;

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

            break;
        }
    }
    case GameState::PLAYING:
    {
        // Actualizar el flujo de música si es necesario
        if (!music2Played && state == GameState::PLAYING) {
            PlayMusicStream(musicArray[1]);
        }
        UpdateMusicStream(musicArray[1]); // Llama a UpdateMusicStream() en cada iteración del bucle

        // Si se presiona la tecla ESC, termina el juego
        if (IsKeyPressed(KEY_ESCAPE)) {
            FinishPlay();
            StopMusicStream(musicArray[1]);
            music2Played = true;

            // Cambia el estado a MAIN_MENU
            state = GameState::MAIN_MENU;

            // Reinicia las animaciones
            ResetAnimations();
        }
        // Si se presiona la tecla F4, cambia al estado CREDITS
        else if (IsKeyPressed(KEY_F4)) {
            // Cambia el estado a CREDITS
            state = GameState::CREDITS;

            // Detiene la música actual si es necesario
            StopMusicStream(musicArray[1]);
            music2Played = true;

        }
        else {
            // Lógica de juego
            scene->Update();
        }
        break;
    }
    case GameState::CREDITS:
        // Actualizar la posición de los créditos
        creditPositionY -= CREDIT_SCROLL_SPEED;
        if (!music3Played && state == GameState::CREDITS) {
            PlayMusicStream(musicArray[2]);
        }
        UpdateMusicStream(musicArray[2]); // Llama a UpdateMusicStream() en cada iteración del bucle

        if (IsKeyPressed(KEY_ESCAPE)) {
            state = GameState::MAIN_MENU;
            StopMusicStream(musicArray[1]);
            music3Played = true;
            ResetCredits();
        }
        // Si la imagen de los créditos se ha desplazado completamente fuera de la pantalla
        if (creditPositionY + creditImage.height < 0) {
            creditsFinished = true;
        }

        // Si los créditos han terminado, cambia al estado MAIN_MENU
        if (creditsFinished) {
            state = GameState::MAIN_MENU;
            music3Played = true;
            ResetCredits();

        }
        break;
    }
    return AppStatus::OK;
}

void Game::ResetAnimations()
{
    currentFrameIntro = 0;
    currentFrameAnimation2 = 0;
    currentFrameCharacterFront = 0; 
    currentFrameCharacterBack = 5; 
    currentFramebat_intro = 0;
    currentFramebat_intro2 = 0;
    framesCounter = 0;
    framesCounter2 = 0;
    framesSpeed2 = 1;
    framesSpeed = 1;
    introPlayed = false;
    animation2Played = false;
    music1Played = false;
    music2Played = false;
    characterFrontFacing = true; 
    characterStopped = false;    
    animationbatPlayed = false;
    animationbat2Played = false;

    // Posición del personaje
    characterPosition = { WINDOW_WIDTH - 30, 95 }; 

    // Posición de la nube
    cloudPosition = { WINDOW_WIDTH - 60, 100 }; 

    // Posición de la segunda animación
    bat_introPosition = { WINDOW_WIDTH / 2 + 20, 70 }; 

    bat_intro2Position = { WINDOW_WIDTH / 2 - 100, 120 }; 
}

void Game::ResetCredits() {
    music3Played = false;
    creditsFinished = false;
    creditPositionY = WINDOW_HEIGHT;
}

void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MAIN_MENU:
    {
        Image imgMenu = LoadImage("images/Sprites/initial screen.png");
        ImageResize(&imgMenu, WINDOW_WIDTH, WINDOW_HEIGHT);
        Texture2D img_menu = LoadTextureFromImage(imgMenu);
        UnloadImage(imgMenu);
        DrawTexture(img_menu, 0, 0, WHITE);
    }
    break;

    case GameState::INTRO:
        ClearBackground(RAYWHITE);
        if (!introPlayed)
        {
            Rectangle source = { currentFrameIntro * WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT }; 
            Rectangle dest = { 0, 0, 0, 0 };
            DrawTextureRec(intro, source, Vector2{ 0, 0 }, WHITE);
        }
        else if (!animation2Played)
        {
            Rectangle source = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
            DrawTextureRec(castle, source, Vector2{ 0,0 }, WHITE);
            //Rectangle dest = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50, EXPLOSION_SIZE * 2, EXPLOSION_SIZE * 2 };
            //DrawTexturePro(animation2, source, dest, Vector2{ dest.width / 2, dest.height / 2 }, 0, WHITE);

            // Dibujar la animación del personaje
            if (characterFrontFacing)
            {
                Rectangle sourceCharacter = { currentFrameCharacterFront * SPRITE_SIZE, 0, -SPRITE_SIZE, SPRITE_SIZE };
                Rectangle destCharacter = { characterPosition.x + SPRITE_SIZE, characterPosition.y, SPRITE_SIZE, SPRITE_SIZE };
                DrawTexturePro(characterFront, sourceCharacter, destCharacter, Vector2{ destCharacter.width, destCharacter.height - 110 }, 0, WHITE);
            }
            else
            {
                Rectangle sourceCharacter = { currentFrameCharacterBack * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
                Rectangle destCharacter = { characterPosition.x, characterPosition.y, SPRITE_SIZE, SPRITE_SIZE };
                DrawTexturePro(characterBack, sourceCharacter, destCharacter, Vector2{ destCharacter.width - 20, destCharacter.height - 110 }, 0, WHITE);
            }

            // Dibujar la nube 
            Rectangle cloudSource = { 0, 0, CLOUD_SIZE, CLOUD_SIZE };
            Rectangle cloudDest = { cloudPosition.x, cloudPosition.y, CLOUD_SIZE, CLOUD_SIZE }; 
            DrawTexturePro(cloudTexture, cloudSource, cloudDest, Vector2{ cloudDest.width / 2 - 35, cloudDest.height / 2 + 25 }, 0, WHITE);

            // Dibujar la segunda animación del murciélago
            Rectangle batSource1 = { (int)currentFramebat_intro * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
            Rectangle batDest1 = { (int)bat_introPosition.x, (int)bat_introPosition.y, SPRITE_SIZE, SPRITE_SIZE };
            DrawTexturePro(bat_intro, batSource1, batDest1, Vector2{ batDest1.width / 2, batDest1.height / 2 }, 0, WHITE);

            // Dibujar la segunda animación del murciélago
            Rectangle batSource2 = { (int)currentFramebat_intro2 * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE };
            Rectangle batDest2 = { (int)bat_intro2Position.x, (int)bat_intro2Position.y, SPRITE_SIZE, SPRITE_SIZE };
            DrawTexturePro(bat_intro2, batSource2, batDest2, Vector2{ batDest2.width / 2, batDest2.height / 2 }, 0, WHITE);
        }
        break;

    case GameState::PLAYING:
        scene->Render();
        break;


    case GameState::CREDITS:
       
        DrawTexture(creditImage, 0, creditPositionY, WHITE);
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}

void Game::Cleanup()
{
    // Unload resources
    UnloadMusicStream(musicArray[0]);
    UnloadMusicStream(musicArray[1]);
    UnloadTexture(background);
    UnloadTexture(intro);
    UnloadTexture(characterFront); 
    UnloadTexture(characterBack); 
    UnloadTexture(cloudTexture);   
    UnloadTexture(bat_intro); 
    UnloadTexture(creditImage);
    CloseAudioDevice();
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);

    UnloadRenderTexture(target);
}