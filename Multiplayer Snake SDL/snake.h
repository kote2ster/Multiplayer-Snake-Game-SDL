#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include <vector>

typedef struct Snake {
    char color;
    SDL_Rect screenPos;
    Snake_Pos pos;
    int levelPosX;
    int levelPosY;
    Snake_Dir nextDir;
    Snake_Dir currDir;
    bool fireBullet;
    bool dead;
} Snake;

extern char playerColor;
extern char serverColor;
extern std::vector<Snake> snakes;

extern SDL_Surface* bmp_snake;
extern SDL_Surface* bmp_snake_dead;

void UpdateSnakeLevel();
void UpdateAliveSnakeMove(char i);
void UpdateSnakeMove();
void DrawDeadSnake();
void DrawSnake();


#endif // SNAKE_H_INCLUDED
