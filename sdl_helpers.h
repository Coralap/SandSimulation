

#include <SDL.h>
#include <stdbool.h>

#include "globals.h"

// Initializes SDL, window, and renderer
bool init(SDL_Window** window, SDL_Renderer** renderer);

// Shuts down SDL and cleans up
void close(SDL_Window* window, SDL_Renderer* renderer);

// Loads an image as a texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

