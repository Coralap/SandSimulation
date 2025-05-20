#include "particle.h"
#include "globals.h"
#include <stdlib.h>

//array with the size of the screen divided by the size of the pixel to keep the space cost to a minimum.
SandParticle sandParticles[(SCREEN_WIDTH * SCREEN_HEIGHT) / particleSize];
SandParticle particlesToUpdate[(SCREEN_WIDTH * SCREEN_HEIGHT) / particleSize];
//a counter to the number of sand particles currently on screen.
int numOfParticles = 0;
const double epsilon = 1e-9;
//if a particle is in the place you enter return true
bool isOccupied(int x, int y) {
	
	y = (y / particleSize) * particleSize;
    //go through each particle on the list
    for (int i = 0; i < numOfParticles; i++) {
        //the y is divided by the particle size and multiplied again is to make sure its an int so checking positions is easier.
        if (sandParticles[i].x == x && (sandParticles[i].y / particleSize) * particleSize == y) {
            return true; // Already a particle here
        }
    }
    return false;
}


//check if there is another particle below a point
SandParticle bottomCollision(int x, int y) {
    //looping through every particle
    for (int i = 0; i < numOfParticles; i++) {
        //if the particle is not in the exact same position(meaning the same one), the particle is bellow them and the space between them is lower then the particleSize(indicating collision) then return true
        if (sandParticles[i].y != y && (double)sandParticles[i].y - y > 0 && (double)sandParticles[i].y - y <= particleSize && sandParticles[i].x == x
			) {

            return sandParticles[i];
        }

    }
	SandParticle nothing = { -1,0,0,0,0,0 };
    return nothing;



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
    sandParticles[numOfParticles].velocity = 0.1;

	sandParticles[numOfParticles].index = numOfParticles;
	sandParticles[numOfParticles].stayedInPlace = false;

    //offseting the color by a random value.
    double randomColorOffset = rand() % colorSpec - colorSpec / 2;
    sandParticles[numOfParticles].r = 0xCB + randomColorOffset;
    sandParticles[numOfParticles].g = 0xBD + randomColorOffset;
    sandParticles[numOfParticles].b = 0x93 + randomColorOffset;



	//set the values for the updatelist
	particlesToUpdate[numOfParticles].x = x;
	particlesToUpdate[numOfParticles].y = y;
	particlesToUpdate[numOfParticles].velocity = 0.1;

	particlesToUpdate[numOfParticles].r =0;
	particlesToUpdate[numOfParticles].g =0;
	particlesToUpdate[numOfParticles].b =0;

	particlesToUpdate[numOfParticles].stayedInPlace = false;


	particlesToUpdate[numOfParticles].index = numOfParticles;



    numOfParticles++;
}
void Step() {
	//looping through every particle
	for (int i = 0; i < numOfParticles; i++) {

		StepParticle(&particlesToUpdate[i]);
		
		//printf("particle: x= %d   y=%d   vel=%f   \n", sandParticles[i].x, sandParticles[i].y, sandParticles[i].velocity);
	}


	return;
}

//update the renderlist from the physics list
void updateParticles() {
	for (int i = 0; i < numOfParticles; i++) {

		if (particlesToUpdate[i].x != -1) {
			sandParticles[particlesToUpdate[i].index].y = particlesToUpdate[i].y;
			sandParticles[particlesToUpdate[i].index].x = particlesToUpdate[i].x;
			sandParticles[particlesToUpdate[i].index].velocity = particlesToUpdate[i].velocity;
		}

	}
	return;
}

//check if there is something on the bottom right or left of a position
bool CheckSides(int x,int y) {
	//same for the right
	if (bottomCollision(x - particleSize, y).x != -1 && bottomCollision(x + particleSize, y).x != -1) {
		return true;
	}
	


	return false;

}

void StepParticle(SandParticle *particle) {
	//update the value of the update list if its in there
	if (particle->x == -1) {
		return;
	}
	



	//check if the particle is touching the ground
	if (particle->y >= SCREEN_HEIGHT - particleSize) {
		particle->y = SCREEN_HEIGHT - particleSize;
		particle->velocity = 0;
		updateParticles();

		particle->x = -1;

		return;
	}
	//check if the particle is touching another particle on the bottom
	SandParticle bottomColl = bottomCollision(particle->x, particle->y);
	if (bottomColl.x != -1) {
		//fix its position
		particle->y = (particle->y / particleSize) * particleSize;

		particle->velocity = bottomColl.velocity;
		particle->y = bottomColl.y - particleSize;

		updateParticles();
		//if there isnt a particle on the bottom left corner go there
		SandParticle coll = bottomCollision(particle->x - particleSize, particle->y);
		if (particle->x != 0 && (coll.x == -1) && fabs(bottomColl.velocity) < epsilon) {
			particle->x -= particleSize;
			particle->y += particleSize;
			particle->stayedInPlace = false;
			updateParticles();
			return;
		}

		//same for the right
		coll = bottomCollision(particle->x + particleSize, particle->y);
		if (particle->x != SCREEN_WIDTH - particleSize && (coll.x == -1)&& fabs(bottomColl.velocity) < epsilon) {

			particle->x += particleSize;
			particle->y += particleSize;
			particle->stayedInPlace = false;
			updateParticles();
			return;
		}
		particle->velocity = bottomColl.velocity;
		particle->y = bottomColl.y - particleSize;


		updateParticles();
		//if there are particles on both sides  and the bottom is not moving stop updating this particle because it wont go anywhere also check if it was there for two frames to prevent freezing partices
		if (fabs(bottomColl.velocity) < epsilon&&CheckSides(particle->x,particle->y)&&fabs(particle->velocity)<epsilon) {
			if (particle->stayedInPlace) {
				particle->x = -1;
			}
			else {
				particle->stayedInPlace = true;
			}
			
		}
		return;
	}

	//apply velocity
	if (fabs(bottomColl.velocity) < epsilon) {
		particle->y += particle->velocity;
	}
	//make sure the velocity is not too big
	if (particle->velocity >= particleSize) {
		particle->velocity = particleSize;
	}
	else {
		//increase it

		particle->velocity += 0.5f;

	}


	updateParticles();
}




