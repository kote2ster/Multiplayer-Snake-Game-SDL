#include <iostream>
#include <math.h>
#include <vector>
#include "level.h"
#include "snake.h"

const float snakeSpeed = 100;

char playerColor = SNK_RED;
char serverColor = SNK_RED;

std::vector<Snake> snakes;

SDL_Surface* bmp_snake;
SDL_Surface* bmp_snake_dead;

void UpdateSnakeLevel(Snake *snake)
{
    switch( Level::map[snake->levelPosX][snake->levelPosY] ) {
        case __EMPTY:
            Level::map[snake->levelPosX][snake->levelPosY] = snake->color;
            break;
        //case SNK_POS:
        //case LVL_BND:
        default:
            snake->dead = true;
            //gameState = GAME_OVER;
            break;
    }

    switch(snake->currDir) {
        case DIR_LEFT:
            if(snake->nextDir != DIR_RIGHT) snake->currDir = snake->nextDir;
        break;
        case DIR_RIGHT:
            if(snake->nextDir != DIR_LEFT)  snake->currDir = snake->nextDir;
        break;
        case DIR_UP:
            if(snake->nextDir != DIR_DOWN)  snake->currDir = snake->nextDir;
        break;
        case DIR_DOWN:
            if(snake->nextDir != DIR_UP)    snake->currDir = snake->nextDir;
        break;
    }

    if(playerColor==serverColor) { // Server updates everything
        app->SendMsg(snake->color);
    }
}

void UpdateAliveSnakeMove(char i)
{
    if       ( snakes[i].screenPos.x >= (snakes[i].levelPosX + 1) * discreteBlocks + (discreteBlocks - bmp_snake->w)/2 ) {
        snakes[i].screenPos.x = snakes[i].pos.x = (snakes[i].levelPosX + 1) * discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
        snakes[i].levelPosX++;
        UpdateSnakeLevel(&snakes[i]);
    } else if( snakes[i].screenPos.x <= (snakes[i].levelPosX - 1) * discreteBlocks - (discreteBlocks - bmp_snake->w)/2 ) {
        snakes[i].screenPos.x = snakes[i].pos.x = (snakes[i].levelPosX - 1) * discreteBlocks - (discreteBlocks - bmp_snake->w)/2;
        snakes[i].levelPosX--;
        UpdateSnakeLevel(&snakes[i]);
    } else if( snakes[i].screenPos.y >= (snakes[i].levelPosY + 1) * discreteBlocks + (discreteBlocks - bmp_snake->h)/2 ) {
        snakes[i].screenPos.y = snakes[i].pos.y = (snakes[i].levelPosY + 1) * discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
        snakes[i].levelPosY++;
        UpdateSnakeLevel(&snakes[i]);
    } else if( snakes[i].screenPos.y <= (snakes[i].levelPosY - 1) * discreteBlocks - (discreteBlocks - bmp_snake->h)/2 ) {
        snakes[i].screenPos.y = snakes[i].pos.y = (snakes[i].levelPosY - 1) * discreteBlocks - (discreteBlocks - bmp_snake->h)/2;
        snakes[i].levelPosY--;
        UpdateSnakeLevel(&snakes[i]);
    }

    switch(snakes[i].currDir)
    {
    case DIR_LEFT:
        snakes[i].pos.x -= deltaTime*snakeSpeed;
        break;
    case DIR_RIGHT:
        snakes[i].pos.x += deltaTime*snakeSpeed;
        break;
    case DIR_UP:
        snakes[i].pos.y -= deltaTime*snakeSpeed;
        break;
    case DIR_DOWN:
        snakes[i].pos.y += deltaTime*snakeSpeed;
        break;
    default:
        break;
    }

    snakes[i].screenPos.x = roundf(snakes[i].pos.x);
    snakes[i].screenPos.y = roundf(snakes[i].pos.y);
}

void UpdateSnakeMove()
{
    for(unsigned int i=0; i<snakes.size();i++) {
        if(!snakes[i].dead) {
            UpdateAliveSnakeMove(i);
        }
    }
}

void DrawDeadSnake() {
    static SDL_Rect* rect = new SDL_Rect();
    static bool noticePlayerDead = true;
    if(noticePlayerDead && snakes[playerColor].dead) {
        std::cout << "You are dead!" << std::endl;
        noticePlayerDead=false;
    }
    for(unsigned int i=0; i<snakes.size();i++) {
        if(snakes[i].dead) {
            rect->x = snakes[i].levelPosX*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
            rect->y = snakes[i].levelPosY*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
            SDL_BlitSurface(bmp_snake_dead, 0, screen, rect);
        }
    }
}

void DrawSnake() {
    // draw snake trace bitmap
    for(unsigned int k=0; k<snakes.size();k++) {
        for(int i=0; i<Level::sizeX; i++) {
            for(int j=0; j<Level::sizeY; j++) {
                if(Level::map[i][j] == snakes[k].color) {
                    static SDL_Rect* rect = new SDL_Rect();
                    rect->x = i*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
                    rect->y = j*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
                    SDL_BlitSurface(bmp_snake, 0, screen, rect);
                }
            }
        }

        // draw moving snake bitmap
        SDL_BlitSurface(bmp_snake, 0, screen, &snakes[k].screenPos);
    }
}
