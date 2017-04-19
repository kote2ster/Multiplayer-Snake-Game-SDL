#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <vector>

#include "SDL.h"
#include "GlobalTypes.h"

typedef struct {
    SDL_Rect screenPos;
    Pos pos;
    Dir dir;
    int levelPosX;
    int levelPosY;
    bool markForDelete;
} Bullet;

extern SDL_Surface* bmp_bullet;

extern const float bulletSpeed;
extern std::vector<Bullet> bullets;
extern bool fireBullet;

void UpdateBulletLevel(Bullet *bullet);
void UpdateBulletMove();

void DrawBullets();

#endif // BULLET_H_INCLUDED
