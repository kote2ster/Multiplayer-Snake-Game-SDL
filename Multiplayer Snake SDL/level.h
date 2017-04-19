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

void DrawLevel();

class Level {
public:
    static const int size = 11;
    static char map[size][size];
};

#endif // LEVEL_H_INCLUDED
