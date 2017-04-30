#include <iostream>
#include "SDL.h"
#include "level.h"
#include "snake.h"
#include "Bullet.h"
#include "NetApp.h"
#include "setup.h"

extern bool restart;

int InitSDL() {
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    screen = SDL_SetVideoMode(Level::sizeX*discreteBlocks+1, Level::sizeY*discreteBlocks+1, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Multiplayer Snake game", NULL);

    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }

    // load images
    bmp_snake_red = SDL_LoadBMP("snake_red.bmp");
    bmp_snake = bmp_snake_red;
    bmp_snake_red_dead = SDL_LoadBMP("snake_red_dead.bmp");
    bmp_snake_yellow = SDL_LoadBMP("snake_yellow.bmp");
    bmp_snake_yellow_dead = SDL_LoadBMP("snake_yellow_dead.bmp");
    bmp_snake_blue = SDL_LoadBMP("snake_blue.bmp");
    bmp_snake_blue_dead = SDL_LoadBMP("snake_blue_dead.bmp");
    bmp_snake_green = SDL_LoadBMP("snake_green.bmp");
    bmp_snake_green_dead = SDL_LoadBMP("snake_green_dead.bmp");
    bmp_bound = SDL_LoadBMP("boundary.bmp");
    bmp_bullet = SDL_LoadBMP("bullet.bmp");
    if (!bmp_snake_red || !bmp_snake_red_dead ||
        !bmp_snake_yellow || !bmp_snake_yellow_dead ||
        !bmp_snake_blue || !bmp_snake_blue_dead ||
        !bmp_snake_green || !bmp_snake_green_dead || !bmp_bound || !bmp_bullet)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int SetupGame() {
    std::cout << "Setup as: " << std::endl << " 1., Server" << std::endl << " 2., Client" << std::endl;
    int choice;
    bool valid = false;
    do {
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        if(choice == 1 || choice == 2)
            valid = true;
        else
            std::cout << "Invalid input!" << std::endl;
    } while(!valid);

    if(choice == 1) {
        std::cout << "Setting up as server!" << std::endl;
        std::cout << "Your color is red!" << std::endl;
        playerColor = SNK_RED;
        app = new CNetHost();
        std::cout << "Press enter on the game window to start the game!" << std::endl;
        std::cout << "Waiting for players..." << std::endl;
        // init
        if( InitSDL() ) return 1;
        bool startGame = false;
        while(!startGame) {
            startGame = app->WaitForOtherPlayers();
        }
        std::cout << "Game started!" << std::endl;
    } else {
        std::cout << "Setting up as client!" << std::endl;
        app = new CNetClient();
        playerColor = SNK_SPECTATE;
        playerColor = ((CNetClient*)app)->GetColor();
        std::cout << "Waiting for players..." << std::endl;
        bool startGame = false;
        while(!startGame) {
            startGame = app->WaitForOtherPlayers();
        }
        std::cout << "Game started!" << std::endl;

        // init
        if( InitSDL() ) return 1;
    }

    RestartLevel();

    return 0;
}

void RestartLevel()
{
    for(int i=1; i<Level::sizeX-1; i++) {
        for(int j=1; j<Level::sizeY-1; j++) {
            Level::map[i][j] = __EMPTY;
        }
    }

    // Init Red snake
    Level::map[redSnakeDefaultPos.x][redSnakeDefaultPos.y] = SNK_RED;
    snakes.clear();
    bullets.clear();
    Snake initSnake;
    initSnake.color = SNK_RED;
    initSnake.nextDir = DIR_UP;
    initSnake.currDir = DIR_UP;
    initSnake.dead = false;
    initSnake.pos.x = initSnake.screenPos.x = redSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
    initSnake.pos.y = initSnake.screenPos.y = redSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
    initSnake.levelPosX = redSnakeDefaultPos.x;
    initSnake.levelPosY = redSnakeDefaultPos.y;

    snakes.push_back(initSnake);

    // Init Yellow snake
    Level::map[yelSnakeDefaultPos.x][yelSnakeDefaultPos.y] = SNK_YEL;
    initSnake.color = SNK_YEL;
    initSnake.pos.x = initSnake.screenPos.x = yelSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
    initSnake.pos.y = initSnake.screenPos.y = yelSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
    initSnake.levelPosX = yelSnakeDefaultPos.x;
    initSnake.levelPosY = yelSnakeDefaultPos.y;

    snakes.push_back(initSnake);

    // Init Blue snake
    if(numOfPlayers >= 3) {
        Level::map[bluSnakeDefaultPos.x][bluSnakeDefaultPos.y] = SNK_BLU;
        initSnake.color = SNK_BLU;
        initSnake.pos.x = initSnake.screenPos.x = bluSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
        initSnake.pos.y = initSnake.screenPos.y = bluSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
        initSnake.levelPosX = bluSnakeDefaultPos.x;
        initSnake.levelPosY = bluSnakeDefaultPos.y;

        snakes.push_back(initSnake);
    }

    // Init Green snake
    if(numOfPlayers >= 4) {
        Level::map[grnSnakeDefaultPos.x][grnSnakeDefaultPos.y] = SNK_GRN;
        initSnake.color = SNK_GRN;
        initSnake.pos.x = initSnake.screenPos.x = grnSnakeDefaultPos.x*discreteBlocks + (discreteBlocks - bmp_snake->w)/2;
        initSnake.pos.y = initSnake.screenPos.y = grnSnakeDefaultPos.y*discreteBlocks + (discreteBlocks - bmp_snake->h)/2;
        initSnake.levelPosX = grnSnakeDefaultPos.x;
        initSnake.levelPosY = grnSnakeDefaultPos.y;

        snakes.push_back(initSnake);
    }

    gameState = GAME_RUN;
    restart = false;
}
