#ifndef GAME_H
#define GAME_H

#define SCREEN_DIM 600
#define SCALE 12
#define OFFSET 50

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Snake.h"

using namespace std;


class Game
{
    public:

        Game();
        virtual ~Game();
        void run();

        static SDL_Renderer *gameRenderer;
        static SDL_Rect gameBoundry;
        static SDL_Texture *loadTexture(string,int,int,int);

    protected:

    private:

        bool initSystem();
        void cleanSystem();
        bool loadFiles();
        void updateScreen();
        void renderChanges();
        void gameLoop();
        void handleGameEvents();
        SDL_Texture* loadText(string,Uint8,Uint8,Uint8);
        void renderScore();

        static const int FPS=60;
        int score;
        SDL_Window *gameWindow;
        SDL_Surface *gameSurface;
        SDL_Texture *gameOverTexture;
        Mix_Chunk *gameOverEffect,*outEffect,*foodEatenEffect;
        TTF_Font *font;
        SDL_Texture *textTexture;
        SDL_Event gameEvents;
        bool playStart;
};

#endif // GAME_H
