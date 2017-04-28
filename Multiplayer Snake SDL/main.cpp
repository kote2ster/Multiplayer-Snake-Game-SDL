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

    std::cout << "Setup as: " << std::endl << " 1., Server" << std::endl << " 2., Client" << std::endl;
    int choice;
    bool valid = false;
    do {
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        if(choice == 1 || choice == 2)
            valid = true;
        else
            std::cout << "Invalid input!" << std::endl;
    } while(!valid);

    if(choice == 1) {
        std::cout << "Setting up as server!" << std::endl;
        std::cout << "Your color is red!" << std::endl;
        playerColor = SNK_RED;
        app = new CNetHost();
    } else {
        std::cout << "Setting up as client!" << std::endl;
        std::cout << "Choose your color: " << std::endl << " 1., Yellow" << std::endl;
        valid = false;
        do {
            std::cin >> choice;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            if(choice == 1)
                valid = true;
            else
                std::cout << "Invalid input!" << std::endl;
        } while(!valid);
        if(choice == 1)
            playerColor = SNK_YEL;
        app = new CNetClient();
    }

    // init
    if( InitSDL() ) return 1;

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

        switch(gameState) {
            case GAME_RUN:
                if(restart) RestartLevel();
                //UpdateManualMove(&snakePos, &screenPos);
                UpdateMove();

                // DRAWING STARTS HERE
                Drawing();
                DrawDeadSnake();

                // finally, update the screen
                SDL_Flip(screen);
                break;
            case GAME_OVER:
                std::cout << "Game Over!" << std::endl;
                SDL_Flip(screen);
                gameState = GAME_PAUSE;
                break;
            case GAME_PAUSE:
                break;
        }
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp_snake);
    SDL_FreeSurface(bmp_bound);
    SDL_FreeSurface(bmp_snake_dead);
    SDL_FreeSurface(bmp_bullet);

    std::cout << "Exited cleanly" << std::endl;
    return 0;
}

void UpdateMove() {
    UpdateSnakeMove();
    UpdateBulletMove();
}

void Drawing() {
    // clear screen
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

    DrawLevel();
    DrawSnake();
    DrawBullets();
}

int InitSDL() {
    // initialize SDL video
    //if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    //{
       // printf( "Unable to init SDL: %s\n", SDL_GetError() );
       // return 1;
    //}

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(Level::sizeX*discreteBlocks+1, Level::sizeY*discreteBlocks+1, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    // load images
    bmp_snake = SDL_LoadBMP("cb.bmp");
    bmp_bound = SDL_LoadBMP("boundary.bmp");
    bmp_snake_dead = SDL_LoadBMP("snake_dead.bmp");
    bmp_bullet = SDL_LoadBMP("bullet.bmp");
    if (!bmp_snake || !bmp_bound || !bmp_snake_dead || !bmp_bullet)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    RestartLevel();

    return 0;
}

void RestartLevel()
 {
     for(int i=1; i<Level::sizeX-1; i++) {
        for(int j=1; j<Level::sizeY-1; j++) {
            Level::map[i][j] = __EMPTY;
        }
    }

    Level::map[redSnakeDefaultPos.x][redSnakeDefaultPos.y] = SNK_RED;
    Level::map[redSnakeDefaultPos.x][redSnakeDefaultPos.y] = SNK_RED;
    Level::map[yelSnakeDefaultPos.x][yelSnakeDefaultPos.y] = SNK_YEL;
    Level::map[yelSnakeDefaultPos.x][yelSnakeDefaultPos.y] = SNK_YEL;

     // Init
    snakes.clear();
    Snake initSnake;
    initSnake.color = SNK_RED;
    initSnake.nextDir = DIR_UP;
    initSnake.currDir = DIR_UP;
    initSnake.dead = false;
    initSnake.pos.x = initSnake.screenPos.x = redSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
    initSnake.pos.y = initSnake.screenPos.y = redSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
    initSnake.levelPosX = redSnakeDefaultPos.x;
    initSnake.levelPosY = redSnakeDefaultPos.y;

    snakes.push_back(initSnake);

    initSnake.color = SNK_YEL;
    initSnake.nextDir = DIR_UP;
    initSnake.currDir = DIR_UP;
    initSnake.pos.x = initSnake.screenPos.x = yelSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
    initSnake.pos.y = initSnake.screenPos.y = yelSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
    initSnake.levelPosX = yelSnakeDefaultPos.x;
    initSnake.levelPosY = yelSnakeDefaultPos.y;

    snakes.push_back(initSnake);

    restart = false;
 }
