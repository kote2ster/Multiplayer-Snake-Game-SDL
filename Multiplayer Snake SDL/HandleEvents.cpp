#include <map>
#include "SDL.h"
#include "level.h"
#include "snake.h"

extern bool restart;
extern bool done;
extern Snake_Dir nextSnakeDir;
extern bool fireBullet;
extern Uint32 bulletFireReload;

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
                if(playerColor!=SNK_SPECTATE) {
                    snakes[playerColor].nextDir = DIR_LEFT;
                    app->SendMsg(playerColor);
                }
                break;
            case SDLK_UP:
                pressedKey[SDLK_UP] = true;
                if(playerColor!=SNK_SPECTATE) {
                    snakes[playerColor].nextDir = DIR_UP;
                    app->SendMsg(playerColor);
                }
                break;
            case SDLK_RIGHT:
                pressedKey[SDLK_RIGHT] = true;
                if(playerColor!=SNK_SPECTATE) {
                    snakes[playerColor].nextDir = DIR_RIGHT;
                    app->SendMsg(playerColor);
                }
                break;
            case SDLK_DOWN:
                pressedKey[SDLK_DOWN] = true;
                if(playerColor!=SNK_SPECTATE) {
                    snakes[playerColor].nextDir = DIR_DOWN;
                    app->SendMsg(playerColor);
                }
                break;
            case SDLK_SPACE:
                pressedKey[SDLK_SPACE] = true;
                if(playerColor!=SNK_SPECTATE && !snakes[playerColor].dead) {
                    static Uint32 reloadCurrTime = 0;
                    Uint32 reloadNextTime = SDL_GetTicks();
                    if(reloadNextTime > reloadCurrTime) {
                        reloadCurrTime = reloadNextTime + bulletFireReload;
                        fireBullet = true;
                        app->SendMsg(playerColor);
                    }
                }
                break;
            case SDLK_r:
                pressedKey[SDLK_r] = true;
                if(playerColor == serverColor) {// Only server can restart
                    restart = true;
                    app->SendMsg(playerColor);
                }
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

bool HandleWaitForPlayersEvents()
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
            case SDLK_RETURN:
                return true;
                break;
            default:
                break;
            }

            break;
        }
        }
    }
    return false;
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
