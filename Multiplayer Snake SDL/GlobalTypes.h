#ifndef GLOBALTYPES_H_INCLUDED
#define GLOBALTYPES_H_INCLUDED

enum {__EMPTY, SNK_POS, LVL_BND};
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
