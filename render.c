#include "render.h"
#include "globals.h"

void renderRectangles(SDL_Renderer* gRenderer,SandParticle rects[]) {
	//go through each particle on the list
	for (int i = 0; i < numOfParticles; i++) {
		//create a rectangle with his x,y width and height using the array
		SDL_Rect fillRect = { rects[i].x, rects[i].y, particleSize, particleSize };
		//setting the render color...
		SDL_SetRenderDrawColor(gRenderer, rects[i].r, rects[i].g, rects[i].b, 0xFF);
		//giving the order to do it
		SDL_RenderFillRect(gRenderer, &fillRect);


	}
	return;

}


void alphaCursorRender(SDL_Renderer* gRenderer,int x, int y) {
	//create a rectangle with his x,y width and height of a regular particle
	SDL_Rect fillRect = { (x / particleSize) * particleSize,(y / particleSize) * particleSize, particleSize, particleSize };
	//setting the render color but with a lower a value to make it transparent
	SDL_SetRenderDrawColor(gRenderer, 0xCB, 0xBD, 0x93, 0x66);
	//giving the order to do it

	SDL_RenderFillRect(gRenderer, &fillRect);

}