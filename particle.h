#pragma once

#include <stdbool.h>
#include <SDL.h>

//defining a struct for the sand particle
typedef struct {
	//positions
	int x,y;

	//speed downwards
	double velocity;

	//colors
	int r,g,b;
} SandParticle;

extern SandParticle sandParticles[];
extern int numOfParticles;

void addParticle(int x, int y);
bool isOccupied(int x, int y);
void Gravity(void);