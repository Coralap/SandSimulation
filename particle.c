#include "particle.h"
#include "globals.h"
#include <stdlib.h>

//array with the size of the screen divided by the size of the pixel to keep the space cost to a minimum.
SandParticle sandParticles[(SCREEN_WIDTH * SCREEN_HEIGHT) / particleSize];

//a counter to the number of sand particles currently on screen.
int numOfParticles = 0;

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

    //offseting the color by a random value.
    double randomColorOffset = rand() % colorSpec - colorSpec / 2;
    sandParticles[numOfParticles].r = 0xCB + randomColorOffset;
    sandParticles[numOfParticles].g = 0xBD + randomColorOffset;
    sandParticles[numOfParticles].b = 0x93 + randomColorOffset;




    numOfParticles++;
}
void Gravity() {
	//looping through every particle
	for (int i = 0; i < numOfParticles; i++) {

		//check if the particle is touching the ground
		if (sandParticles[i].y >= SCREEN_HEIGHT - particleSize) {
			sandParticles[i].y = SCREEN_HEIGHT - particleSize;
			sandParticles[i].velocity = 0;
			continue;
		}
		//check if the particle is touching another particle on the bottom
		SandParticle bottomColl = bottomCollision(sandParticles[i].x, sandParticles[i].y);
		if (bottomColl.x!=-1) {
			//fix its position
			sandParticles[i].y = (sandParticles[i].y / particleSize) * particleSize;
			
			sandParticles[i].velocity = bottomColl.velocity;
			sandParticles[i].y = bottomColl.y - particleSize;
			

			//if there isnt a particle on the bottom left corner go there
			SandParticle coll = bottomCollision(sandParticles[i].x - particleSize, sandParticles[i].y);

			if (sandParticles[i].x != 0 && (coll.x == -1) && bottomColl.velocity == 0) {
				sandParticles[i].x -= particleSize;
				sandParticles[i].y += particleSize;

				continue;
			}

			//same for the right
			coll = bottomCollision(sandParticles[i].x + particleSize, sandParticles[i].y);
			if (sandParticles[i].x != SCREEN_WIDTH - particleSize && (coll.x == -1)&& bottomColl.velocity==0) {

				sandParticles[i].x += particleSize;
				sandParticles[i].y += particleSize;
				continue;
			}
			sandParticles[i].velocity = bottomColl.velocity;
			sandParticles[i].y = bottomColl.y - particleSize;
			continue;
		}

		//apply velocity
		if (bottomColl.velocity == 0) {
			sandParticles[i].y += sandParticles[i].velocity;
		}
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


