#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

//#undef main //делітаєм індефікатор через який все кідроням злітає

using namespace std;

constexpr int Screen_Height = 1080;
constexpr int Screen_Width = 1920;
SDL_Texture* LoadTexture(string filePath, SDL_Renderer* renderTarg);
/* 
desc of func
*/
SDL_Texture* LoadTexture(string filePath, SDL_Renderer* renderTarg) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		cout << "BMP surface error\n";
	else {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));//erase some color from img
		texture = SDL_CreateTextureFromSurface(renderTarg, surface);
		SDL_SetTextureColorMod(texture, 20, 100, 10);//add color mask on your img
		if (texture == NULL)
			cout << "Error texture init\n";
	}

	SDL_FreeSurface(surface);
	return texture;
}

void DrawText(SDL_Renderer* render, const int& x, const int& y, const string& str, const string fontPath = "text.ttf") {
	TTF_Font* appFont = TTF_OpenFont(fontPath.c_str(), 20);
	SDL_Color textColor = { 121,55,0,255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(appFont, str.c_str(), textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(render, textSurface);
	SDL_Rect textRect = { x,y,0,0 };

	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);//get w h text

	SDL_FreeSurface(textSurface);
	textSurface = nullptr;

	SDL_RenderCopy(render, text, NULL, &textRect);//draw text

	SDL_DestroyTexture(text);//del data
	text = nullptr;
	TTF_CloseFont(appFont);
}


int main(int argc, char** argv) {
	const Uint8* keyState;
	float frameTime = 0, deltaTime = 0, playerSpeed = 50.0f;
	int prevTime = 0, currentTime = 0;

	SDL_Window* window = nullptr;
	SDL_Texture* img = nullptr;
	SDL_Texture* img1 = nullptr;
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
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048/*~2kB*/) < 0)//44100
		cout << "Mixer init error: " << Mix_GetError() << endl;
	Mix_Music* music = Mix_LoadMUS("06. Mutter.mp3");//waw\midi\ogg
	Mix_Chunk* chunkSong = Mix_LoadWAV("gin.wav"); //for short (<10s) mucic

	renderTarg = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//the renderer uses hardware acceleration | present is synchronized with the refresh rate
	img = LoadTexture("Image.png", renderTarg);//Image.jpg
	img1 = LoadTexture("Image1.png", renderTarg);

	SDL_Color textColor = { 121,55,0,255 };

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


	//SDL_SetRenderDrawColor(renderTarg, 90, 12, 41, 200);

	while (isRunning) {
		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - prevTime) / 100.0f;
		while (SDL_PollEvent(&event) != 0) {//dequeue of events

			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						playerRect.y = frameHeight * 8;
						playerPosition.y -= playerSpeed * deltaTime;
						Mix_HaltMusic();//stop play music
						break;
					case SDLK_DOWN:
						playerRect.y = frameHeight * 10;
						playerPosition.y += playerSpeed * deltaTime;
						if (!Mix_PlayingMusic())
							Mix_PlayMusic(music, -1);//2 param - cnt of rep, if -1 wil rep until don`t say stop
						else if (Mix_PausedMusic())
							Mix_ResumeMusic();
						else Mix_PauseMusic(); 
						break;
					case SDLK_RIGHT:
						playerRect.y = frameHeight * 11;
						playerPosition.x += playerSpeed*deltaTime;//move player on 100 px Regardles of the speed of the pc
						break;
					case SDLK_LEFT:
						playerRect.y = frameHeight * 9;
						playerPosition.x -= playerSpeed * deltaTime;
						break;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					Mix_PlayChannel(0, chunkSong, 0);//first param - id chanel, if -1 -play first channel
					//Mix_HaltChannel(0); stop song and del channel
					break;
			}

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
		SDL_RenderCopy(renderTarg, img1, NULL, NULL);
		DrawText(renderTarg, 234, 575, "cscscsc");
		SDL_RenderPresent(renderTarg); //Update the screen


	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(img);//free texture
	SDL_DestroyTexture(img1);
	SDL_DestroyRenderer(renderTarg);

	Mix_FreeChunk(chunkSong);
	Mix_FreeMusic(music);
	chunkSong = nullptr;
	music = nullptr;

	window = nullptr;
	img = nullptr;
	img1 = nullptr;
	renderTarg = nullptr;
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}