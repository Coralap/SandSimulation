
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>

#include <stdbool.h>
#include <string.h>

#include <math.h>

#include <time.h>
#include <stdlib.h>

typedef struct  {
	int x;
	int y;
	double velocity;

	int r;
	int g;
	int b;
} SandParticle;


//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;



const int particleSize = 20;
const int colorSpec = 7;


SandParticle sandParticles[1920 * 1080];
int numOfParticles = 0;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(char* path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(char* path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void renderRectangles(SandParticle rects[]) {
	for (int i = 0; i < numOfParticles; i++) {
			SDL_Rect fillRect = { rects[i].x, rects[i].y, particleSize, particleSize };
			SDL_SetRenderDrawColor(gRenderer,rects[i].r, rects[i].g, rects[i].b, 0xFF);
			SDL_RenderFillRect(gRenderer, &fillRect);
		
		
	}
	return;

}

bool isOccupied(int x, int y) {
	for (int i = 0; i < numOfParticles; i++) {
		if (sandParticles[i].x == x && (sandParticles[i].y / particleSize) *particleSize == y) {
			return true; // Already a particle here
		}
	}
	return false;
}

void addParticle(int x, int y) {
	// Snap to nearest multiple of particleSize
	x = (x / particleSize) * particleSize;
	y = (y / particleSize) * particleSize;

	// Check if space is already occupied
	if (isOccupied(x, y)) {
		return; // Do not add overlapping particle
	}

	sandParticles[numOfParticles].x = x;
	sandParticles[numOfParticles].y = y;
	sandParticles[numOfParticles].velocity = 0;

	double randomColorOffset = rand() % colorSpec - colorSpec / 2;
	sandParticles[numOfParticles].r = 0xCB + randomColorOffset;
	sandParticles[numOfParticles].g = 0xBD + randomColorOffset;
	sandParticles[numOfParticles].b = 0x93 + randomColorOffset;



	numOfParticles++;
}

bool bottomCollision(int x, int y) {
	for (int i = 0; i < numOfParticles; i++) {
		if (sandParticles[i].y != y&& (double)sandParticles[i].y - y >0&&(double)sandParticles[i].y -y<= particleSize && sandParticles[i].x == x) {
			return true;
		}

	}
	return false;
	


}


void Gravity() {
	for (int i = 0; i < numOfParticles; i++) {
		if (sandParticles[i].y >= SCREEN_HEIGHT- particleSize) {
			sandParticles[i].y = SCREEN_HEIGHT- particleSize;
			sandParticles[i].velocity = 0;
			continue;
		}
		if (bottomCollision(sandParticles[i].x, sandParticles[i].y)) {
			sandParticles[i].y = (sandParticles[i].y / particleSize) * particleSize;
			
			if (sandParticles[i].velocity != 0) {
				sandParticles[i].velocity = 0;
				continue;
			}
			if (sandParticles[i].x!=0 && !bottomCollision(sandParticles[i].x - particleSize, sandParticles[i].y)) {
				sandParticles[i].x -= particleSize;
				sandParticles[i].y += particleSize;
				
				continue;
			}
			if (sandParticles[i].x != SCREEN_WIDTH- particleSize &&!bottomCollision(sandParticles[i].x + particleSize, sandParticles[i].y)) {
				sandParticles[i].x += particleSize;
				sandParticles[i].y += particleSize;
				continue;
			}
			continue;
		}
		
		sandParticles[i].y += sandParticles[i].velocity;
		if (sandParticles[i].velocity >= particleSize) {
			sandParticles[i].velocity = particleSize;
		}
		else {
			sandParticles[i].velocity += 0.5f;

		}
		//printf("particle: x= %d   y=%d   vel=%f   \n", sandParticles[i].x, sandParticles[i].y, sandParticles[i].velocity);
	}


	return;
}


int main(int argc, char* args[])
{
	srand(time(NULL));

	Uint64 lastUpdateTime = SDL_GetTicks64();
	const Uint64 updateInterval = 16; // approx 60 times per second (1000ms / 60)

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			bool mouseDown= false;
			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}else if(e.type ==SDL_MOUSEBUTTONDOWN){
						mouseDown = true;
			
					}
					else if (e.type == SDL_MOUSEBUTTONUP) {
						mouseDown = false;
					}

				}
				if (mouseDown) {
					int x;
					int y;
					SDL_GetMouseState(&x, &y);
					addParticle(x, y);

				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderRectangles(sandParticles);
			
				

				//Update screen
				SDL_RenderPresent(gRenderer);
				Uint64 currentTime = SDL_GetTicks64();
				if (currentTime - lastUpdateTime >= updateInterval) {
					Gravity();
					lastUpdateTime = currentTime;
				}

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
