#include <math.h>
#include "level.h"

Snake_Dir nextSnakeDir = DIR_UP;
Snake_Dir currSnakeDir = DIR_UP;

const float snakeSpeed = 0;//50;

SDL_Rect snakeScreenPos;
Snake_Pos snakePos;
int snakeLevelPosX = 0;
int snakeLevelPosY = 0;

SDL_Surface* bmp_snake;
SDL_Surface* bmp_snake_dead;

void UpdateSnakeLevel()
{
    switch( Level::map[snakeLevelPosX][snakeLevelPosY] ) {
        case __EMPTY:
            Level::map[snakeLevelPosX][snakeLevelPosY] = SNK_POS;
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
    if       ( snakeScreenPos.x >= (snakeLevelPosX + 1) * discreteBlocks  + (discreteBlocks - bmp_snake->w)/2 ) {
        snakeLevelPosX++;
        UpdateSnakeLevel();

    } else if( snakeScreenPos.x <= (snakeLevelPosX - 1) * discreteBlocks - (discreteBlocks - bmp_snake->w)/2 ) {
        snakeLevelPosX--;
        UpdateSnakeLevel();
    } else if( snakeScreenPos.y >= (snakeLevelPosY + 1) * discreteBlocks + (discreteBlocks - bmp_snake->h)/2 ) {
        snakeLevelPosY++;
        UpdateSnakeLevel();
    } else if( snakeScreenPos.y <= (snakeLevelPosY - 1) * discreteBlocks - (discreteBlocks - bmp_snake->h)/2 ) {
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

void DrawSnakeDead() {
    SDL_Rect* rect = new SDL_Rect();
    rect->x = snakeLevelPosX*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
    rect->y = snakeLevelPosY*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
    SDL_BlitSurface(bmp_snake_dead, 0, screen, rect);
}

void DrawSnake() {
    // draw snake trace bitmap
    for(int i=0; i<Level::size; i++) {
        for(int j=0; j<Level::size; j++) {
            if(Level::map[i][j] == SNK_POS) {
                static SDL_Rect* rect = new SDL_Rect();
                rect->x = i*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
                rect->y = j*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
                SDL_BlitSurface(bmp_snake, 0, screen, rect);
            }
        }
    }

    // draw moving snake bitmap
    SDL_BlitSurface(bmp_snake, 0, screen, &snakeScreenPos);
}
