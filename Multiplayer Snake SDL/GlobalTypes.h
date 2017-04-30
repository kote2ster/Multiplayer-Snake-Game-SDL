#ifndef GLOBALTYPES_H_INCLUDED
#define GLOBALTYPES_H_INCLUDED

enum {SNK_RED=0, SNK_YEL=1, SNK_BLU=2, SNK_GRN=3, SNK_SPECTATE, __EMPTY, LVL_BND};
typedef enum state {GAME_RUN, GAME_PAUSE, GAME_OVER} state;
typedef enum NetType {SERVER, CLIENT} NetType;

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

#endif // GLOBALTYPES_H_INCLUDED
