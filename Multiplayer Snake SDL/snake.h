#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED


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
void DrawSnakeDead();
void DrawSnake();


#endif // SNAKE_H_INCLUDED
