#include <math.h>
#include <algorithm>
#include "Bullet.h"
#include "level.h"
#include "snake.h"

const float bulletSpeed = 100;

std::vector<Bullet> bullets;
bool fireBullet = false;

SDL_Surface* bmp_bullet;

void UpdateBulletLevel(Bullet *bullet)
{
    switch( Level::map[bullet->levelPosX][bullet->levelPosY] ) {
        case __EMPTY:
            break;
        //case SNK_POS:
        case LVL_BND:
            bullet->markForDelete = true;
            break;
        default:
            bullet->markForDelete = true;
            Level::map[bullet->levelPosX][bullet->levelPosY] = __EMPTY;
            for(unsigned int i=0;i<snakes.size();i++) {
                if(snakes[i].levelPosX==bullet->levelPosX&&snakes[i].levelPosY==bullet->levelPosY) {
                    Level::map[bullet->levelPosX][bullet->levelPosY] = snakes[i].color;
                    snakes[i].dead = true;
                }
            }
            break;
    }
}

bool NeedsToBeDeleted(Bullet bullet){ return bullet.markForDelete; }

void UpdateBulletMove() {
    if(fireBullet) {
        Bullet bullet;
        bullet.dir = snakes[playerColor].currDir;
        bullet.pos = snakes[playerColor].pos;
        bullet.screenPos = snakes[playerColor].screenPos;
        bullet.levelPosX = snakes[playerColor].levelPosX;
        bullet.levelPosY = snakes[playerColor].levelPosY;
        bullet.markForDelete = false;
        bullets.push_back(bullet);
        fireBullet = false;
    }

    for(unsigned int i=0; i<bullets.size(); i++) {
        if       ( bullets[i].screenPos.x >= (bullets[i].levelPosX + 1) * discreteBlocks  + (discreteBlocks - bmp_snake->w)/2 ) {
            bullets[i].levelPosX++;
            UpdateBulletLevel(&bullets[i]);

        } else if( bullets[i].screenPos.x <= (bullets[i].levelPosX - 1) * discreteBlocks - (discreteBlocks - bmp_snake->w)/2 ) {
            bullets[i].levelPosX--;
            UpdateBulletLevel(&bullets[i]);
        } else if( bullets[i].screenPos.y >= (bullets[i].levelPosY + 1) * discreteBlocks + (discreteBlocks - bmp_snake->h)/2 ) {
            bullets[i].levelPosY++;
            UpdateBulletLevel(&bullets[i]);
        } else if( bullets[i].screenPos.y <= (bullets[i].levelPosY - 1) * discreteBlocks - (discreteBlocks - bmp_snake->h)/2 ) {
            bullets[i].levelPosY--;
            UpdateBulletLevel(&bullets[i]);
        }

        switch(bullets[i].dir)
        {
        case DIR_LEFT:
            bullets[i].pos.x -= deltaTime*bulletSpeed;
            break;
        case DIR_RIGHT:
            bullets[i].pos.x += deltaTime*bulletSpeed;
            break;
        case DIR_UP:
            bullets[i].pos.y -= deltaTime*bulletSpeed;
            break;
        case DIR_DOWN:
            bullets[i].pos.y += deltaTime*bulletSpeed;
            break;
        default:
            break;
        }

        bullets[i].screenPos.x = roundf(bullets[i].pos.x);
        bullets[i].screenPos.y = roundf(bullets[i].pos.y);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), NeedsToBeDeleted), bullets.end());
}

void DrawBullets() {
    for(unsigned int i=0; i<bullets.size(); i++) {
        SDL_BlitSurface(bmp_bullet, 0, screen, &bullets[i].screenPos);
    }
}
