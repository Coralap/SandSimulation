#include <SDL.h>
#include <stdlib.h>
#include <time.h>

#include "globals.h"
#include "particle.h"
#include "render.h"
#include "sdl_helpers.h"

int main(int argc, char* args[])
{
    // Setup the random value generator
    srand(time(NULL));

    // Setup SDL window and renderer
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Setting up a timer
    Uint64 lastUpdateTime = SDL_GetTicks64();
    const Uint64 updateInterval = 16; // Approx 60 times per second (1000ms / 60)

    // Start up SDL and create window
    if (!init(&window, &renderer)) {
        printf("Failed to initialize!\n");
        return 1;
    }

    // Load media
    if (!loadMedia()) {
        printf("Failed to load media!\n");
        return 1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Is a mouse button down
    bool mouseDown = false;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // If a mouse button is pressed down set the value to true
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                mouseDown = true;
            }
            // If released, set to false
            else if (e.type == SDL_MOUSEBUTTONUP) {
                mouseDown = false;
            }
        }

        // Get the position of the mouse
        int x, y;
        SDL_GetMouseState(&x, &y);

        // If the mouse is down, try to place a particle there
        if (mouseDown) {
            addParticle(x, y);
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render all particles and the cursor highlight
        renderRectangles(renderer, sandParticles);
        alphaCursorRender(renderer, x, y);

        // Update screen
        SDL_RenderPresent(renderer);

        // Get the current time
        Uint64 currentTime = SDL_GetTicks64();
        // Compare the times between the last and now to see if update time has passed
        // This is to keep the same speed on multiple framerates
        if (currentTime - lastUpdateTime >= updateInterval) {
            // Reset the timer and call the gravity function
            Gravity();
            lastUpdateTime = currentTime;
        }
    }

    // Free resources and close SDL
    close(window, renderer);
    return 0;
}
