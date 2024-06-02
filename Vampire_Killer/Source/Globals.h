#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	4.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    256 
#define WINDOW_HEIGHT   212

//Game constants
//#define MARGIN_GUI_Y	36 
#define TILE_SIZE		16
#define LEVEL_WIDTH		16 
#define LEVEL_HEIGHT	13


//Entities animation delay
#define ANIM_DELAY		10 

#define EXPLOSION_SIZE 276
#define SPRITE_SIZE 32
#define CLOUD_SIZE 64

#define CREDIT_SCROLL_SPEED 0.3f