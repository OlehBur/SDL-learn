#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Player.h"

//float x = 0 has an implicit typecast from int to float.
//float x = 0.0f does not have such a typecast.
//float x = 0.0 has an implicit typecast from double to float.

using namespace std;

int main(int argc, char** argv) {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderTarg = nullptr;
	SDL_Event event;
	int currentTime = 0,
		prevTime = 0;
	float deltaTime = 0.0f;
	const Uint8* keyState;
	bool isRunning = true;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Test Wnd", 50, 30, 1920 - 100, 1080 - 70, SDL_WINDOW_OPENGL);
	renderTarg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//the renderer uses hardware acceleration | present is synchronized with the refresh rate

	Player player(renderTarg, "PLayer.png", 0, 0, 3, 4);
	Player player1(renderTarg, "PLayer.png", 1000, 500, 3, 4);

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

		SDL_RenderClear(renderTarg);//Clear the current rendering target with the drawing color.
		player.Draw(renderTarg);
		player1.Draw(renderTarg);
		SDL_RenderPresent(renderTarg); //Update the screen
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderTarg);

	window = nullptr;
	renderTarg = nullptr;
	IMG_Quit();
	SDL_Quit();

	return 0;
}