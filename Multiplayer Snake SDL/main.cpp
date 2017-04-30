#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#include <map>
#include <math.h>
#else
#include <stdlib.h>
#endif

#include "SDL.h"
#include "level.h"
#include "snake.h"
#include "Bullet.h"
#include "NetApp.h"
#include "setup.h"

Uint32 currentTime, previousTime;
float deltaTime;

bool restart = false;
bool done = false;
state gameState = GAME_RUN;

CNetInterface* app;

//void UpdateManualMove();
int InitSDL();
void RestartLevel();
void UpdateMove();
void Drawing();
extern void HandleEvents();

int main ( int argc, char** argv )
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    if( SetupGame() ) return 1;

    // program main loop
    previousTime = SDL_GetTicks();
    while (!done)
    {
        // timing
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - previousTime) / 1000.f;
        previousTime = currentTime;

        HandleEvents();

        app->HandleConnection();
        if(restart) RestartLevel();

        switch(gameState) {
            case GAME_RUN:
                app->CheckForWinner();
                //UpdateManualMove(&snakePos, &screenPos);
                UpdateMove();

                // DRAWING STARTS HERE
                Drawing();
                DrawDeadSnake();

                // finally, update the screen
                SDL_Flip(screen);
                break;
            case GAME_PAUSE:
                break;
        }
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp_snake_red);
    SDL_FreeSurface(bmp_snake_red_dead);
    SDL_FreeSurface(bmp_snake_yellow);
    SDL_FreeSurface(bmp_snake_yellow_dead);
    SDL_FreeSurface(bmp_snake_blue);
    SDL_FreeSurface(bmp_snake_blue_dead);
    SDL_FreeSurface(bmp_snake_green);
    SDL_FreeSurface(bmp_snake_green_dead);
    SDL_FreeSurface(bmp_bound);
    SDL_FreeSurface(bmp_bullet);

    std::cout << "Exited cleanly" << std::endl;
    return 0;
}

void UpdateMove() {
    UpdateBulletMove();
    UpdateSnakeMove();
}

void Drawing() {
    // clear screen
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

    DrawLevel();
    DrawSnake();
    DrawBullets();
}
