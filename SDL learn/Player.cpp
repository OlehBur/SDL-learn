#include "Player.h"
#include <SDL_image.h>
#include <iostream>
#include <cmath>

Player::Player(SDL_Renderer* render, string filePath, const int& x, const int& y, const int& framesX, const int& framesY) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		cout << "IMG surface error\n";
	else {
		texture = SDL_CreateTextureFromSurface(render, surface);
		if (texture == NULL)
			cout << "Error texture init\n";
	}

	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &frameR.w, &frameR.h);

	positionR.x = x;
	positionR.y = y;

	textureWidth = frameR.w;

	frameR.w /= framesX;
	frameR.h /= framesY;

	frameWidth = positionR.w = frameR.w;
	frameHeight = positionR.h = frameR.h;

	centerX = positionR.x + frameWidth / 2;
	centerY = positionR.y + frameHeight / 2;
	radius = frameHeight / 2.4;

	isActive = false;
	static int playerNumber = 0;
	playerNumber++;
	if (playerNumber == 1) {
		keys[0] = SDL_SCANCODE_W;
		keys[1] = SDL_SCANCODE_S;
		keys[2] = SDL_SCANCODE_A;
		keys[3] = SDL_SCANCODE_D;
	}
	else {
		keys[0] = SDL_SCANCODE_UP;
		keys[1] = SDL_SCANCODE_DOWN;
		keys[2] = SDL_SCANCODE_LEFT;
		keys[3] = SDL_SCANCODE_RIGHT;
	}
	
	moveSpeed = 200.0f;
};

void Player::Update(const float delta, const Uint8* keyState) {
	isActive = true;

	if (keyState[keys[0]]) {
		positionR.y -= moveSpeed * delta;
		frameR.y = frameHeight * 3;
	}
	else if (keyState[keys[1]]) {
		positionR.y += moveSpeed * delta;
		frameR.y = 0;
	}
	else if (keyState[keys[2]]) {
		positionR.x -= moveSpeed * delta;
		frameR.y = frameHeight;
	}
	else if (keyState[keys[3]]) {
		positionR.x += moveSpeed * delta;
		frameR.y = frameHeight * 2;
	}
	else
		isActive = false;

	centerX = positionR.x + frameWidth / 2;
	centerY = positionR.y + frameHeight / 2;

	if (isActive) {
		frameCntr += delta;

		if (frameCntr >= 0.25f) {
			frameCntr = 0;
			frameR.x += frameWidth*2;
			if (frameR.x >= textureWidth)
				frameR.x = 0;
		}
	}
	else {
		frameCntr = 0;
		frameR.x = frameWidth;
	}
};

void Player::Draw(SDL_Renderer* render) {
	SDL_RenderCopy(render, texture, &frameR, &positionR);
};

int& Player::GetCenterX() {
	return centerX;
};

int& Player::GetCenterY() {
	return centerY;
};

int& Player::GetRadius() {
	return radius;
};

bool Player::IntersectsWith(/*const*/ Player& player) {
	if (sqrt(pow(centerX - player.GetCenterX(), 2) + pow(centerY - player.GetCenterY(), 2)) >= radius + player.GetRadius()) {
		SDL_SetTextureColorMod(texture, 255, 255, 255);
		return false;
	}
	else {
		SDL_SetTextureColorMod(texture, 255, 0, 0);
		return true;
	}
};


Player::~Player() {
	SDL_DestroyTexture(texture);
};

