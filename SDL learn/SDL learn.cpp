#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

//#undef main //делітаєм індефікатор через який все кідроням злітає

using namespace std;

constexpr int Screen_Height = 1080;
constexpr int Screen_Width = 1920;


SDL_Texture* LoadTexture(string filePath, SDL_Renderer* renderTarg) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		cout << "BMP surface error\n";
	else {
		texture = SDL_CreateTextureFromSurface(renderTarg, surface);
		if (texture == NULL)
			cout << "Error texture init\n";
	}

	SDL_FreeSurface(surface);
	return texture;
}

int main(int argc, char** argv) {
	const Uint8* keyState;
	float frameTime = 0, deltaTime = 0, playerSpeed = 50.0f;
	int prevTime = 0, currentTime = 0;

	SDL_Window* window = nullptr;
	SDL_Texture* img = nullptr;
	SDL_Renderer* renderTarg = nullptr;
	SDL_Rect playerRect;
	SDL_Rect playerPosition;
	SDL_Event event;
	bool isRunning = true;

	int frameWidth, frameHeight;
	int textureWidth, textureHeight;

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
		cout << "Image init error: " << IMG_GetError() << endl;

	SDL_Init(SDL_INIT_VIDEO);
	if (TTF_Init() < 0)
		cout << "TTF init error: " << TTF_GetError() << endl;

	window = SDL_CreateWindow("Test Wnd", 50, 30, Screen_Width - 100, Screen_Height - 70, SDL_WINDOW_OPENGL);
	renderTarg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//the renderer uses hardware acceleration | present is synchronized with the refresh rate
	img = LoadTexture("Image.png", renderTarg);//Image.png

	TTF_Font* appFont = TTF_OpenFont("text.ttf", 20);
	SDL_Color textColor = { 121,55,0,255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(appFont, "This is text writing by SDL", textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderTarg, textSurface);
	SDL_Rect textRect = { 0,0,0,0 };

	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);

	SDL_FreeSurface(textSurface);
	textSurface = nullptr;

	SDL_QueryTexture(img, NULL, NULL, &textureWidth, &textureHeight);//set attributes of a texture
	frameWidth = textureWidth / 13;
	frameHeight = textureHeight / 21;

	playerRect.x = frameWidth;
	playerRect.y = frameHeight * 2;
	playerRect.w = frameWidth;
	playerRect.h = frameHeight;

	playerPosition.x = playerPosition.y = 0;
	playerPosition.w = frameWidth * 3;
	playerPosition.h = frameHeight * 3;


	SDL_SetRenderDrawColor(renderTarg, 90, 12, 41, 200);

	while (isRunning) {
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 100.0f;
		while (SDL_PollEvent(&event) != 0) {//dequeue of events

			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				//case SDL_KEYDOWN:
				//	switch (event.key.keysym.sym) {
				//	case SDLK_UP:
				//		playerRect.y = frameHeight * 8;
				//		playerPosition.y -= playerSpeed * deltaTime;
				//		break;
				//	case SDLK_DOWN:
				//		playerRect.y = frameHeight * 10;
				//		playerPosition.y += playerSpeed * deltaTime;
				//		break;
				//	case SDLK_RIGHT:
				//		playerRect.y = frameHeight * 11;
				//		playerPosition.x += playerSpeed*deltaTime;//move player on 100 px Regardles of the speed of the pc
				//		break;
				//	case SDLK_LEFT:
				//		playerRect.y = frameHeight * 9;
				//		playerPosition.x -= playerSpeed * deltaTime;
				//		break;
				//	}
				//	break;
			}

			keyState = SDL_GetKeyboardState(NULL);//get ALL keys that exist now
			if (keyState[SDL_SCANCODE_LEFT]) {
				playerRect.y = frameHeight * 9;
				playerPosition.x -= playerSpeed * deltaTime;
			}
			else if (keyState[SDL_SCANCODE_RIGHT]) {
				playerRect.y = frameHeight * 11;
				playerPosition.x += playerSpeed * deltaTime;//move player on 100 px Regardles of the speed of the pc
			}
			else if (keyState[SDL_SCANCODE_UP]) {
				playerRect.y = frameHeight * 8;
				playerPosition.y -= playerSpeed * deltaTime;
			}
			else if (keyState[SDL_SCANCODE_DOWN]) {
				playerRect.y = frameHeight * 10;
				playerPosition.y += playerSpeed * deltaTime;
			}

			//switch (*keyState)
			//{
			//case SDL_SCANCODE_LEFT:
			//	
			//	break;
			//case SDL_SCANCODE_RIGHT:
			//	break;
			//case SDL_SCANCODE_UP:
			//	playerRect.y = frameHeight * 8;
			//	playerPosition.y -= playerSpeed * deltaTime;
			//	break;
			//case SDL_SCANCODE_DOWN:
			//	playerRect.y = frameHeight * 10;
			//	playerPosition.y += playerSpeed * deltaTime;
			//	break;
			//}

		}

		frameTime += deltaTime;
		if (frameTime >= 0.85f) {
			frameTime = 0;
			playerRect.x += frameWidth;
			if (playerRect.x >= frameWidth * 9)
				playerRect.x = frameWidth;
		}

		SDL_RenderClear(renderTarg);//Clear the current rendering target with the drawing color.
		SDL_RenderCopy(renderTarg, img, &playerRect, &playerPosition);//(Copy to the screen) Copy a portion of the texture to the current rendering target.
		SDL_RenderCopy(renderTarg, text, NULL, &textRect);
		SDL_RenderPresent(renderTarg); //Update the screen


	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(img);//free texture
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(renderTarg);
	window = nullptr;
	img = nullptr;
	text = nullptr;
	renderTarg = nullptr;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}