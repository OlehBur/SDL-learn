#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <iostream>
#include "Player.h"

//float x = 0 has an implicit typecast from int to float.
//float x = 0.0f does not have such a typecast.
//float x = 0.0 has an implicit typecast from double to float.

using namespace std;

SDL_Texture* LoadTexture(const string &filePath, SDL_Renderer* renderTarg) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		cout << "Img surface error\n"<<IMG_GetError()<<endl;
	else {
		texture = SDL_CreateTextureFromSurface(renderTarg, surface);
		if (texture == NULL)
			cout << "Error texture init\n";
	}

	SDL_FreeSurface(surface);
	return texture;
}

int main(int argc, char** argv) {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderTarg = nullptr;
	SDL_Event event;
	SDL_Rect cameraR = { 0,0, 1000,  800 };
	int currentTime = 0,
		prevTime = 0;
	float deltaTime = 0.0f;
	const Uint8* keyState;
	int backgrWidth, backgrHeight;
	bool isRunning = true;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Test Wnd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_OPENGL);
	renderTarg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//the renderer uses hardware acceleration | present is synchronized with the refresh rate

	Player player(renderTarg, "PLayer.png", 0, 0, 3, 4);
	Player player1(renderTarg, "PLayer.png", 100, 50, 3, 4);

	SDL_Texture* backgrTexture = LoadTexture("M-N.png", renderTarg);
	SDL_QueryTexture(backgrTexture, NULL, NULL, &backgrWidth, &backgrHeight);

	while (isRunning) {
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 1000.0f;
		while (SDL_PollEvent(&event) != 0) {//dequeue of events

			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		keyState = SDL_GetKeyboardState(NULL);

		player.Update(deltaTime, keyState);
		player1.Update(deltaTime, keyState);

		player.IntersectsWith(player1);

		cameraR.x = player.GetCenterX() - 500;
		cameraR.y = player.GetCenterY() - 400;
		if (cameraR.x < 0)
			cameraR.x = 0;
		if (cameraR.y < 0)
			cameraR.y = 0;
		if (cameraR.x + cameraR.w >= backgrWidth)
			cameraR.x = backgrWidth - 1000;
		if (cameraR.y + cameraR.h >= backgrHeight)
			cameraR.y = backgrHeight - 800;

		SDL_RenderClear(renderTarg);//Clear the current rendering target with the drawing color.
		SDL_RenderCopy(renderTarg, backgrTexture, &cameraR, NULL);
		player.Draw(renderTarg, cameraR);
		player1.Draw(renderTarg, cameraR);
		SDL_RenderPresent(renderTarg); //Update the screen
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderTarg);
	SDL_DestroyTexture(backgrTexture);

	backgrTexture = nullptr;
	window = nullptr;
	renderTarg = nullptr;
	IMG_Quit();
	SDL_Quit();

	return 0;
}