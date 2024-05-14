#pragma once

class LevelBackground
{
private:
	Texture2D backgroundImage, ui; //= LoadTexture("images/Sprites/256x176 Levels.png");
public:
	LevelBackground();
	//~LevelBackground();
	void RenderBackground(int stage);
	void Release();
};