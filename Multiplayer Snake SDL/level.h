#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <map>
#include "SDL.h"
#include "GlobalTypes.h"
#include "NetApp.h"

extern float deltaTime;
extern state gameState;

extern CNetInterface* app;

extern SDL_Surface* screen;
extern SDL_Surface* bmp_bound;

extern const float discreteBlocks;

extern const SDL_Rect redSnakeDefaultPos;
extern const SDL_Rect yelSnakeDefaultPos;

void DrawLevel();

class Level {
public:
    static const int sizeX = 39;
    static const int sizeY = 19;
    static char map[sizeX][sizeY];
};

#endif // LEVEL_H_INCLUDED
