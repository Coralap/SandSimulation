
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>

#include <stdbool.h>
#include <string.h>

#include <math.h>

#include <time.h>
#include <stdlib.h>

//defining a struct for the sand particle
typedef struct  {
	//positions
	int x; 
	int y;

	//speed downwards
	double velocity;

	//colors
	int r;
	int g;
	int b;
} SandParticle;


//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


//info about the particle

//size of the particle
#define particleSize 20

//the brightness spectrum that the colors of the sand will be changed by(higher = more bright and more dark pixels)
const int colorSpec = 7;

//array with the size of the screen divided by the size of the pixel to keep the space cost to a minimum.
SandParticle sandParticles[(1920 * 1080)/particleSize];

//a counter to the number of sand particles currently on screen.
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
	//go through each particle on the list
	for (int i = 0; i < numOfParticles; i++) {
			//create a rectangle with his x,y width and height using the array
			SDL_Rect fillRect = { rects[i].x, rects[i].y, particleSize, particleSize }; 
			//setting the render color...
			SDL_SetRenderDrawColor(gRenderer,rects[i].r, rects[i].g, rects[i].b, 0xFF);
			//giving the order to do it
			SDL_RenderFillRect(gRenderer, &fillRect);
		
		
	}
	return;

}
//if a particle is in the place you enter return true
bool isOccupied(int x, int y) {
	//go through each particle on the list
	for (int i = 0; i < numOfParticles; i++) {
		//the y is divided by the particle size and multiplied again is to make sure its an int so checking positions is easier.
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

	//setting all of the values

	sandParticles[numOfParticles].x = x;
	sandParticles[numOfParticles].y = y;
	sandParticles[numOfParticles].velocity = 0;

	//offseting the color by a random value.
	double randomColorOffset = rand() % colorSpec - colorSpec / 2;
	sandParticles[numOfParticles].r = 0xCB + randomColorOffset;
	sandParticles[numOfParticles].g = 0xBD + randomColorOffset;
	sandParticles[numOfParticles].b = 0x93 + randomColorOffset;



	numOfParticles++;
}

//check if there is another particle below a point
bool bottomCollision(int x, int y) {
	//looping through every particle
	for (int i = 0; i < numOfParticles; i++) {
		//if the particle is not in the exact same position(meaning the same one), the particle is bellow them and the space between them is lower then the particleSize(indicating collision) then return true
		if (sandParticles[i].y != y&& (double)sandParticles[i].y - y >0&&(double)sandParticles[i].y -y<= particleSize && sandParticles[i].x == x) {
			return true;
		}

	}
	return false;
	


}


void Gravity() {
	//looping through every particle
	for (int i = 0; i < numOfParticles; i++) {

		//check if the particle is touching the ground
		if (sandParticles[i].y >= SCREEN_HEIGHT- particleSize) {
			sandParticles[i].y = SCREEN_HEIGHT- particleSize;
			sandParticles[i].velocity = 0;
			continue;
		}
		//check if the particle is touching another particle on the bottom
		if (bottomCollision(sandParticles[i].x, sandParticles[i].y)) {
			//fix its position
			sandParticles[i].y = (sandParticles[i].y / particleSize) * particleSize;
			
			//reset the velocity only if its moving 
			if (sandParticles[i].velocity != 0) {
				sandParticles[i].velocity = 0;
				continue;
			}

			//if there isnt a particle on the bottom left corner go there
			if (sandParticles[i].x!=0 && !bottomCollision(sandParticles[i].x - particleSize, sandParticles[i].y)) {
				sandParticles[i].x -= particleSize;
				sandParticles[i].y += particleSize;
				
				continue;
			}

			//same for the right
			if (sandParticles[i].x != SCREEN_WIDTH- particleSize &&!bottomCollision(sandParticles[i].x + particleSize, sandParticles[i].y)) {
				sandParticles[i].x += particleSize;
				sandParticles[i].y += particleSize;
				continue;
			}
			continue;
		}
		
		//apply velocity
		sandParticles[i].y += sandParticles[i].velocity;

		//make sure the velocity is not too big
		if (sandParticles[i].velocity >= particleSize) {
			sandParticles[i].velocity = particleSize;
		}
		else {
			//increase it
			sandParticles[i].velocity += 0.5f;

		}
		//printf("particle: x= %d   y=%d   vel=%f   \n", sandParticles[i].x, sandParticles[i].y, sandParticles[i].velocity);
	}


	return;
}

void alphaCursorRender(int x,int y) {
	//create a rectangle with his x,y width and height of a regular particle
	SDL_Rect fillRect = {(x/particleSize)*particleSize,( y / particleSize) * particleSize, particleSize, particleSize };
	//setting the render color but with a lower a value to make it transparent
	SDL_SetRenderDrawColor(gRenderer, 0xCB, 0xBD, 0x93, 0x66);
	//giving the order to do it

	SDL_RenderFillRect(gRenderer, &fillRect);

}


int main(int argc, char* args[])
{
	//setup the random value generator
	srand(time(NULL));

	//setting up a timer
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

			//is a mouse button down
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
					}else 
					//if a mouse button is pressed down set the value to true	
					if(e.type ==SDL_MOUSEBUTTONDOWN){
						mouseDown = true;
			
					}
					//if up set to false
					else if (e.type == SDL_MOUSEBUTTONUP) {
						mouseDown = false;
					}

				}
				//get the position of the mouse
				int x;
				int y;
				SDL_GetMouseState(&x, &y);
				
				//if the mouse is down try to place a particle there
				if (mouseDown) {

					addParticle(x, y);
				}
				
				

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				
				renderRectangles(sandParticles);
				alphaCursorRender(x, y);
				

				//Update screen
				SDL_RenderPresent(gRenderer);

				//get the current time
				Uint64 currentTime = SDL_GetTicks64();
				//compare the times between the start of the function to now to see if the wanted time has passed in order to keep the same speed on multiple framerates
				if (currentTime - lastUpdateTime >= updateInterval) {
					//reset the timer and call the gravity function
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
