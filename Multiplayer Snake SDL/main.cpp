#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#else
#include <stdlib.h>
#endif

#include "SDL.h"
#include "level.h"

std::map<SDLKey, bool> pressedKey;
Uint32 currentTime, previousTime;
float deltaTime;

bool done = false;
Snake_Dir nextSnakeDir = DIR_UP;
Snake_Dir currSnakeDir = DIR_UP;

// centre the bitmap on screen
SDL_Rect snakeScreenPos;
Snake_Pos snakePos;
typedef struct {
    SDL_Rect screenPos;
    Pos pos;
    Dir dir;
    int levelPosX;
    int levelPosY;
    bool markForDelete;
} Bullet;
std::vector<Bullet> bullets;
bool fireBullet = false;

SDL_Surface* screen;
SDL_Surface* bmp;
SDL_Surface* bmp_bound;
SDL_Surface* bmp_snake_dead;
SDL_Surface* bmp_bullet;

//void UpdateManualMove();
void UpdateMove();
void HandleEvents();
void Drawing();
void DrawSnakeDead();

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(levelSize*discreteBlocks+1, levelSize*discreteBlocks+1, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);

    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    bmp = SDL_LoadBMP("cb.bmp");
    bmp_bound = SDL_LoadBMP("boundary.bmp");
    bmp_snake_dead = SDL_LoadBMP("snake_dead.bmp");
    bmp_bullet = SDL_LoadBMP("bullet.bmp");
    if (!bmp || !bmp_bound || !bmp_snake_dead || !bmp_bullet)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // Init
    for(int i=0; i<levelSize; i++) {
        for(int j=0; j<levelSize; j++) {
            if(level[i][j] == SNK_POS) {
                snakePos.x = snakeScreenPos.x = i*discreteBlocks + (discreteBlocks - bmp->w)/2;
                snakePos.y = snakeScreenPos.y = j*discreteBlocks + (discreteBlocks - bmp->h)/2;
                snakeLevelPosX = i;
                snakeLevelPosY = j;
            }
        }
    }

    // program main loop
    previousTime = SDL_GetTicks();
    while (!done)
    {
        // timing
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - previousTime) / 1000.f;
        previousTime = currentTime;

        HandleEvents();

        switch(gameState) {
            case GAME_RUN:
                //UpdateManualMove(&snakePos, &screenPos);
                UpdateMove();

                // DRAWING STARTS HERE
                Drawing();

                // finally, update the screen
                SDL_Flip(screen);
                break;
            case GAME_OVER:
                std::cout << "Game Over!" << std::endl;
                DrawSnakeDead();
                SDL_Flip(screen);
                gameState = GAME_PAUSE;
                break;
            case GAME_PAUSE:
                break;
        }
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp);
    SDL_FreeSurface(bmp_bound);
    SDL_FreeSurface(bmp_snake_dead);
    SDL_FreeSurface(bmp_bullet);

    std::cout << "Exited cleanly" << std::endl;
    return 0;
}

void UpdateManualMove()
{
    if (pressedKey[SDLK_LEFT])
        snakePos.x -= deltaTime*snakeSpeed;
    if (pressedKey[SDLK_RIGHT])
        snakePos.x += deltaTime*snakeSpeed;
    if (pressedKey[SDLK_UP])
        snakePos.y -= deltaTime*snakeSpeed;
    if (pressedKey[SDLK_DOWN])
        snakePos.y += deltaTime*snakeSpeed;

    snakeScreenPos.x = roundf(snakePos.x);
    snakeScreenPos.y = roundf(snakePos.y);
}

void UpdateSnakeLevel()
{
    switch( level[snakeLevelPosX][snakeLevelPosY] ) {
        case __EMPTY:
            level[snakeLevelPosX][snakeLevelPosY] = SNK_POS;
            break;
        case SNK_POS:
        case LVL_BND:
            gameState = GAME_OVER;
            break;
    }

    switch(currSnakeDir) {
        case DIR_LEFT:
            if(nextSnakeDir != DIR_RIGHT) currSnakeDir = nextSnakeDir;
        break;
        case DIR_RIGHT:
            if(nextSnakeDir != DIR_LEFT)  currSnakeDir = nextSnakeDir;
        break;
        case DIR_UP:
            if(nextSnakeDir != DIR_DOWN)  currSnakeDir = nextSnakeDir;
        break;
        case DIR_DOWN:
            if(nextSnakeDir != DIR_UP)    currSnakeDir = nextSnakeDir;
        break;
    }
}

void UpdateSnakeMove()
{
    if       ( snakeScreenPos.x >= (snakeLevelPosX + 1) * discreteBlocks  + (discreteBlocks - bmp->w)/2 ) {
        snakeLevelPosX++;
        UpdateSnakeLevel();

    } else if( snakeScreenPos.x <= (snakeLevelPosX - 1) * discreteBlocks - (discreteBlocks - bmp->w)/2 ) {
        snakeLevelPosX--;
        UpdateSnakeLevel();
    } else if( snakeScreenPos.y >= (snakeLevelPosY + 1) * discreteBlocks + (discreteBlocks - bmp->h)/2 ) {
        snakeLevelPosY++;
        UpdateSnakeLevel();
    } else if( snakeScreenPos.y <= (snakeLevelPosY - 1) * discreteBlocks - (discreteBlocks - bmp->h)/2 ) {
        snakeLevelPosY--;
        UpdateSnakeLevel();
    }

    switch(currSnakeDir)
    {
    case DIR_LEFT:
        snakePos.x -= deltaTime*snakeSpeed;
        break;
    case DIR_RIGHT:
        snakePos.x += deltaTime*snakeSpeed;
        break;
    case DIR_UP:
        snakePos.y -= deltaTime*snakeSpeed;
        break;
    case DIR_DOWN:
        snakePos.y += deltaTime*snakeSpeed;
        break;
    default:
        break;
    }

    snakeScreenPos.x = roundf(snakePos.x);
    snakeScreenPos.y = roundf(snakePos.y);
}

void UpdateBulletLevel(Bullet *bullet)
{
    switch( level[bullet->levelPosX][bullet->levelPosY] ) {
        case __EMPTY:
            break;
        case SNK_POS:
        case LVL_BND:
            bullet->markForDelete = true;
            break;
    }
}

bool NeedsToBeDeleted(Bullet bullet){ return bullet.markForDelete; }

void UpdateBulletMove() {
    if(fireBullet) {
        Bullet bullet;
        bullet.dir = currSnakeDir;
        bullet.pos = snakePos;
        bullet.screenPos = snakeScreenPos;
        bullet.levelPosX = snakeLevelPosX;
        bullet.levelPosY = snakeLevelPosY;
        bullet.markForDelete = false;
        bullets.push_back(bullet);
        fireBullet = false;
    }

    for(unsigned int i=0; i<bullets.size(); i++) {
        if       ( bullets[i].screenPos.x >= (bullets[i].levelPosX + 1) * discreteBlocks  + (discreteBlocks - bmp->w)/2 ) {
            bullets[i].levelPosX++;
            UpdateBulletLevel(&bullets[i]);

        } else if( bullets[i].screenPos.x <= (bullets[i].levelPosX - 1) * discreteBlocks - (discreteBlocks - bmp->w)/2 ) {
            bullets[i].levelPosX--;
            UpdateBulletLevel(&bullets[i]);
        } else if( bullets[i].screenPos.y >= (bullets[i].levelPosY + 1) * discreteBlocks + (discreteBlocks - bmp->h)/2 ) {
            bullets[i].levelPosY++;
            UpdateBulletLevel(&bullets[i]);
        } else if( bullets[i].screenPos.y <= (bullets[i].levelPosY - 1) * discreteBlocks - (discreteBlocks - bmp->h)/2 ) {
            bullets[i].levelPosY--;
            UpdateBulletLevel(&bullets[i]);
        }

        switch(bullets[i].dir)
        {
        case DIR_LEFT:
            bullets[i].pos.x -= deltaTime*bulletSpeed;
            break;
        case DIR_RIGHT:
            bullets[i].pos.x += deltaTime*bulletSpeed;
            break;
        case DIR_UP:
            bullets[i].pos.y -= deltaTime*bulletSpeed;
            break;
        case DIR_DOWN:
            bullets[i].pos.y += deltaTime*bulletSpeed;
            break;
        default:
            break;
        }

        bullets[i].screenPos.x = roundf(bullets[i].pos.x);
        bullets[i].screenPos.y = roundf(bullets[i].pos.y);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), NeedsToBeDeleted), bullets.end());
}

void UpdateMove() {
    UpdateSnakeMove();
    UpdateBulletMove();
}

void HandleEvents()
{
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
        // exit if the window is closed
        case SDL_QUIT:
            done = true;
            break;

        // check for keypresses
        case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = true;
                break;
            case SDLK_LEFT:
                pressedKey[SDLK_LEFT] = true;
                nextSnakeDir = DIR_LEFT;
                break;
            case SDLK_UP:
                pressedKey[SDLK_UP] = true;
                nextSnakeDir = DIR_UP;
                break;
            case SDLK_RIGHT:
                pressedKey[SDLK_RIGHT] = true;
                nextSnakeDir = DIR_RIGHT;
                break;
            case SDLK_DOWN:
                pressedKey[SDLK_DOWN] = true;
                nextSnakeDir = DIR_DOWN;
                break;
            case SDLK_SPACE:
                pressedKey[SDLK_SPACE] = true;
                fireBullet = true;
                break;
            default:
                break;
            }

            break;
        }
        case SDL_KEYUP:
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_LEFT:
                pressedKey[SDLK_LEFT] = false;
                break;
            case SDLK_UP:
                pressedKey[SDLK_UP] = false;
                break;
            case SDLK_RIGHT:
                pressedKey[SDLK_RIGHT] = false;
                break;
            case SDLK_DOWN:
                pressedKey[SDLK_DOWN] = false;
                break;
            default:
                break;
            }

            break;
        }
        } // end switch
    } // end of message processing
}

void DrawSnakeDead() {
    SDL_Rect* rect = new SDL_Rect();
    rect->x = snakeLevelPosX*discreteBlocks + (discreteBlocks - bmp->w)/2;
    rect->y = snakeLevelPosY*discreteBlocks + (discreteBlocks - bmp->h)/2;
    SDL_BlitSurface(bmp_snake_dead, 0, screen, rect);
}

void DrawLevel() {
    // Draw grid
    for(int i=0; i<screen->w/discreteBlocks; i++) {
        static SDL_Rect *gridRect = new SDL_Rect();
        gridRect->w = 1;
        gridRect->h = screen->h;
        gridRect->x = i*discreteBlocks;
        gridRect->y = 0;
        SDL_FillRect(screen, gridRect, SDL_MapRGB(screen->format, 100, 100, 100));
        gridRect->w = screen->w;
        gridRect->h = 1;
        gridRect->x = 0;
        gridRect->y = i*discreteBlocks;
        SDL_FillRect(screen, gridRect, SDL_MapRGB(screen->format, 100, 100, 100));
    }

    // draw level boundary
    for(int i=0; i<levelSize; i++) {
        for(int j=0; j<levelSize; j++) {
            if(level[i][j] == LVL_BND) {
                static SDL_Rect* rect = new SDL_Rect();
                rect->x = i*discreteBlocks + (discreteBlocks - bmp_bound->w)/2;
                rect->y = j*discreteBlocks + (discreteBlocks - bmp_bound->h)/2;
                SDL_BlitSurface(bmp_bound, 0, screen, rect);
            }
        }
    }
}

void DrawSnake() {
    // draw snake trace bitmap
    for(int i=0; i<levelSize; i++) {
        for(int j=0; j<levelSize; j++) {
            if(level[i][j] == SNK_POS) {
                static SDL_Rect* rect = new SDL_Rect();
                rect->x = i*discreteBlocks + (discreteBlocks - bmp->w)/2;
                rect->y = j*discreteBlocks + (discreteBlocks - bmp->h)/2;
                SDL_BlitSurface(bmp, 0, screen, rect);
            }
        }
    }

    // draw moving snake bitmap
    SDL_BlitSurface(bmp, 0, screen, &snakeScreenPos);
}

void DrawBullets() {
    for(unsigned int i=0; i<bullets.size(); i++) {
        SDL_BlitSurface(bmp_bullet, 0, screen, &bullets[i].screenPos);
    }
}

void Drawing() {
    // clear screen
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

    DrawLevel();
    DrawSnake();
    DrawBullets();
}
