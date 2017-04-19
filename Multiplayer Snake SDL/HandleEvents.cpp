#include <map>
#include "SDL.h"
#include "level.h"

extern bool done;
extern Snake_Dir nextSnakeDir;
extern bool fireBullet;

std::map<SDLKey, bool> pressedKey;

void HandleEvents()
{
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
        // exit if the window is closed
        case SDL_QUIT:
            done = true;
            break;

        // check for keypresses
        case SDL_KEYDOWN:
        {
            // exit if ESCAPE is pressed
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = true;
                break;
            case SDLK_LEFT:
                pressedKey[SDLK_LEFT] = true;
                nextSnakeDir = DIR_LEFT;
                break;
            case SDLK_UP:
                pressedKey[SDLK_UP] = true;
                nextSnakeDir = DIR_UP;
                break;
            case SDLK_RIGHT:
                pressedKey[SDLK_RIGHT] = true;
                nextSnakeDir = DIR_RIGHT;
                break;
            case SDLK_DOWN:
                pressedKey[SDLK_DOWN] = true;
                nextSnakeDir = DIR_DOWN;
                break;
            case SDLK_SPACE:
                pressedKey[SDLK_SPACE] = true;
                app->SendMsg();
                fireBullet = true;
                break;
            default:
                break;
            }

            break;
        }
        case SDL_KEYUP:
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_LEFT:
                pressedKey[SDLK_LEFT] = false;
                break;
            case SDLK_UP:
                pressedKey[SDLK_UP] = false;
                break;
            case SDLK_RIGHT:
                pressedKey[SDLK_RIGHT] = false;
                break;
            case SDLK_DOWN:
                pressedKey[SDLK_DOWN] = false;
                break;
            default:
                break;
            }

            break;
        }
        } // end switch
    } // end of message processing
}

/*void UpdateManualMove()
{
    if (pressedKey[SDLK_LEFT])
        snakePos.x -= deltaTime*snakeSpeed;
    if (pressedKey[SDLK_RIGHT])
        snakePos.x += deltaTime*snakeSpeed;
    if (pressedKey[SDLK_UP])
        snakePos.y -= deltaTime*snakeSpeed;
    if (pressedKey[SDLK_DOWN])
        snakePos.y += deltaTime*snakeSpeed;

    snakeScreenPos.x = roundf(snakePos.x);
    snakeScreenPos.y = roundf(snakePos.y);
}*/
