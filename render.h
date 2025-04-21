#pragma once
#pragma once
#include "particle.h"
#include <SDL.h>

void renderRectangles(SDL_Renderer* renderer, SandParticle rects[]);
void alphaCursorRender(SDL_Renderer* renderer, int x, int y);
