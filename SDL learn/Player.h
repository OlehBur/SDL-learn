#pragma once

#include <SDL.h>
#include <string>

using namespace std;

class Player
{
	SDL_Rect frameR;
	SDL_Texture *texture;
	float moveSpeed = { 0.0f },
		frameCntr{ 0.0f };
	int frameWidth{ 0 }, frameHeight{ 0 };
	int textureWidth{ 0 }, textureHeight{ 0 };
	bool isActive;
	SDL_Scancode keys[4];

public:
	SDL_Rect positionR;

	Player(SDL_Renderer* render, string filePath, int x, int y, int framesX, int framesY);
	void Update(float delta, const Uint8* keyState);
	void Draw(SDL_Renderer* render);
	~Player();
};

