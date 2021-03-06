#include <iostream>
#include "level.h"
#include "GlobalTypes.h"
#include "snake.h"

SDL_Surface* screen;
SDL_Surface* bmp_bound;

const float discreteBlocks = 30;

const SDL_Rect redSnakeDefaultPos = { 10, 5 };
const SDL_Rect yelSnakeDefaultPos = { 30, 5 };
const SDL_Rect bluSnakeDefaultPos = { 10, 15 };
const SDL_Rect grnSnakeDefaultPos = { 30, 15 };

char Level::map[sizeX][sizeY] = {
        {LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND}
    };

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
    for(int i=0; i<Level::sizeX; i++) {
        for(int j=0; j<Level::sizeY; j++) {
            if(Level::map[i][j] == LVL_BND) {
                static SDL_Rect* rect = new SDL_Rect();
                rect->x = i*discreteBlocks + (discreteBlocks - bmp_bound->w)/2;
                rect->y = j*discreteBlocks + (discreteBlocks - bmp_bound->h)/2;
                SDL_BlitSurface(bmp_bound, 0, screen, rect);
            }
        }
    }
}
