#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

enum {__EMPTY, SNK_POS, LVL_BND};
typedef enum state {GAME_RUN, GAME_PAUSE, GAME_OVER} state;

state gameState = GAME_RUN;

const float snakeSpeed = 50;
const float bulletSpeed = 100;
const float discreteBlocks = 30;

typedef enum Snake_Dir
{
    DIR_LEFT,
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN
} Snake_Dir, Dir;

typedef struct Snake_Pos
{
    float x, y;
} Snake_Pos, Pos;

int snakeLevelPosX = 0;
int snakeLevelPosY = 0;

const int levelSize = 11;
char level[levelSize][levelSize] = {
        {LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, SNK_POS, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, __EMPTY, LVL_BND},
        {LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND, LVL_BND}
    };

#endif // LEVEL_H_INCLUDED
