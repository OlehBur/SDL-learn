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
	int centerX, centerY, radius;
	bool isActive;
	SDL_Scancode keys[4];

public:
	SDL_Rect positionR;

	Player(SDL_Renderer* render, string filePath, const int& x, const int& y, const int& framesX, const int& framesY);
	
	int& GetCenterX();
	int& GetCenterY();
	int& GetRadius();

	void Update(const float delta, const Uint8* keyState);
	void Draw(SDL_Renderer* render, const SDL_Rect& cameraR);
	bool IntersectsWith(Player& player);

	~Player();
};

