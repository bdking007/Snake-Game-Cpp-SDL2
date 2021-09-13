#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_DIM 12
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#include "Game.h"


class Snake
{
    public:
        Snake();
        virtual ~Snake();
        bool loadSnake();
        void moveSnake();
        void drawSnake();
        void drawFood();
        bool isBoundryCollision();
        bool isFoodCollision();
        bool isSelfCollision();
        void increaseSnakeSize();

        SDL_Texture *snakeTexture;
        SDL_Texture *foodTexture;
        SDL_Rect initialPosition;
        int length;
        int SNAKE_SPEED;
        int snakeSpeedX,snakeSpeedY;
        bool isFoodOnScreen;
        SDL_Rect food;

    protected:

    private:
};

#endif // SNAKE_H
