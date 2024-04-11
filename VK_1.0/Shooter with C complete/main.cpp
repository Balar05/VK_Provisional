
#include <stdio.h>
#include <raylib.h>

//Windows size
#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   768

//Velocities
#define PLAYER_SPEED    8
#define SCENE_SPEED     2
#define SHOOT_SPEED     16

//Maximum number of shots on screen
#define MAX_SHOTS       32

//Entity struct definition
typedef struct Entity {
    Vector2 pos;
    int width, height;
    int speed;
    bool is_alive;
} Entity;

//Global variables
Entity player, shots[MAX_SHOTS], scene;
int idx_shot;   //next free position in the shots array
bool god_mode;
Texture img_background, img_player, img_shot;

//Basic structure functions
void Setup();
bool Update();
void Render();
void Cleanup();

//Auxiliar functions
bool LoadTextures();
void UnloadTextures();

int main()
{
    Setup();
    while(!WindowShouldClose())
    {
        if(!Update()) break;
        Render();
    }
    Cleanup();

    return 0;
}

void Setup()
{
    //Initialise windows
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Spaceship: arrow keys + space, f1: god mode");

    //Load resources
    if (!LoadTextures())
    {
        printf("Failed to load textures\n");
        CloseWindow();
        return;
    }

    //Initialise player
    player.pos = { 20, WINDOW_HEIGHT / 2 };
    player.width = 104;
    player.height = 82;
    player.speed = PLAYER_SPEED;
    player.is_alive = true;

    //Initialise shots
    idx_shot = 0;
    for (int i = 0; i < MAX_SHOTS; ++i)
    {
        shots[i].is_alive = false;
    }

    //Initialise scene
    scene.pos = { 0, 0 };
    scene.width = img_background.width;
    scene.height = WINDOW_HEIGHT;
    scene.speed = SCENE_SPEED;
    scene.is_alive = true;

    //Initialise control variables
    god_mode = false;

    //Set the target frame rate for the application
    SetTargetFPS(60);
}
bool LoadTextures()
{
    img_background = LoadTexture("images/background.png");
    if (img_background.id == 0) return false;
    img_player = LoadTexture("images/spaceship.png");
    if (img_player.id == 0) return false;
    img_shot = LoadTexture("images/shot.png");
    if (img_shot.id == 0) return false;

    return true;
}

bool Update()
{
    int fx = 0, fy = 0;
    
    //PROCESS INPUT

    if (IsKeyDown(KEY_ESCAPE))  return false;
    if (IsKeyPressed(KEY_F1))   god_mode = !god_mode;

    if (IsKeyDown(KEY_UP))          fy = -1;
    else if (IsKeyDown(KEY_DOWN))   fy = 1;
    if (IsKeyDown(KEY_LEFT))        fx = -1;
    else if (IsKeyDown(KEY_RIGHT))  fx = 1;

    if (IsKeyPressed(KEY_SPACE))
    {
        shots[idx_shot].pos = { player.pos.x + 29, player.pos.y + 3 };
        shots[idx_shot].width = 56;
        shots[idx_shot].height = 20;
        shots[idx_shot].speed = 10;
        shots[idx_shot].is_alive = true;
        idx_shot++;
        idx_shot %= MAX_SHOTS;

        shots[idx_shot].pos = { player.pos.x + 29, player.pos.y + 59 };
        shots[idx_shot].width = 56;
        shots[idx_shot].height = 20;
        shots[idx_shot].speed = 10;
        shots[idx_shot].is_alive = true;
        idx_shot++;
        idx_shot %= MAX_SHOTS;
    }

    //GAME LOGIC

    //Scene scroll
    scene.pos.x -= SCENE_SPEED;
    if (scene.pos.x <= -scene.width) scene.pos.x = 0;

    //Player movement
    player.pos.x += fx * PLAYER_SPEED;
    player.pos.y += fy * PLAYER_SPEED;

    //Shots management
    for (int i = 0; i < MAX_SHOTS; ++i)
    {
        if (shots[i].is_alive)
        {
            shots[i].pos.x += SHOOT_SPEED;
            if (shots[i].pos.x > WINDOW_WIDTH)
                shots[i].is_alive = false;
        }
    }

    return true;
}

void Render()
{
    Rectangle rect;

    BeginDrawing();
    ClearBackground(BLACK);

    //Scene
    DrawTexture(img_background, scene.pos.x, 0, WHITE);
    DrawTexture(img_background, scene.pos.x + scene.width, 0, WHITE);

    //Player
    DrawTexture(img_player, player.pos.x, player.pos.y, WHITE);
    if (god_mode)
    {
        rect = { player.pos.x, player.pos.y, (float)player.width, (float)player.height };
        DrawRectangleLinesEx(rect, 2, RED);
    }

    //Shots
    for (int i = 0; i < MAX_SHOTS; ++i)
    {
        if (shots[i].is_alive)
        {
            DrawTexture(img_shot, shots[i].pos.x, shots[i].pos.y, WHITE);
            if (god_mode)
            {
                rect = { shots[i].pos.x, shots[i].pos.y, (float)shots[i].width, (float)shots[i].height };
                DrawRectangleLinesEx(rect, 2, RED);
            }
        }
    }

    EndDrawing();
}

void Cleanup()
{
    UnloadTextures();
}
void UnloadTextures()
{
    UnloadTexture(img_background);
    UnloadTexture(img_player);
    UnloadTexture(img_shot);
}