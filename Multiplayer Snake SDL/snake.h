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
    bool dead;
} Snake;

extern char playerColor;
extern std::vector<Snake> snakes;

extern Snake_Dir nextSnakeDir;
extern Snake_Dir currSnakeDir;

extern SDL_Rect snakeScreenPos;
extern Snake_Pos snakePos;
extern int snakeLevelPosX;
extern int snakeLevelPosY;

extern SDL_Surface* bmp_snake;
extern SDL_Surface* bmp_snake_dead;

void UpdateSnakeLevel();
void UpdateSnakeMove();
void DrawDeadSnake();
void DrawSnake();


#endif // SNAKE_H_INCLUDED
