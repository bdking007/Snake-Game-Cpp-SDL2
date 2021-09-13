#include "Snake.h"

vector <SDL_Point> snakePositions;

Snake::Snake()
{
    //ctor
    snakeTexture=nullptr;  foodTexture=nullptr;
    length=5;   SNAKE_SPEED=3;

    isFoodOnScreen=false;   srand(time(NULL));
    food.x=food.y=0;   food.h=food.w=SNAKE_DIM;

    snakeSpeedX=SNAKE_SPEED;   snakeSpeedY=0;

    snakePositions.reserve(length);
    snakePositions[0]={10*SNAKE_DIM,8*SNAKE_DIM};
    snakePositions[1]={9*SNAKE_DIM,8*SNAKE_DIM};
    snakePositions[2]={8*SNAKE_DIM,8*SNAKE_DIM};

}

Snake::~Snake()
{
    //dtor
    SDL_DestroyTexture(snakeTexture);   snakeTexture=nullptr;
    SDL_DestroyTexture(foodTexture);  foodTexture=nullptr;

}

bool Snake::loadSnake()
{
    snakeTexture=Game::loadTexture("assets/images/snake.png",255,255,255);
    foodTexture=Game::loadTexture("assets/images/food.png",255,255,255);
    if(snakeTexture==nullptr || foodTexture==nullptr)
        return false;
    else
        return true;
}

void Snake::moveSnake()
{

   for(int i=length-1;i>=1;i--)
   {
       snakePositions[i].x=snakePositions[i-1].x;
       snakePositions[i].y=snakePositions[i-1].y;
   }
   snakePositions[0].x+=snakeSpeedX;   snakePositions[0].y+=snakeSpeedY;
}

void Snake::drawSnake()
{
    SDL_Rect pos;   pos.h=pos.w=SNAKE_DIM;
    for(int i=0;i<length;i++)
    {
       pos.x=snakePositions[i].x;   pos.y=snakePositions[i].y;
       SDL_RenderCopy(Game::gameRenderer,snakeTexture,NULL,&pos);
    }
}

void Snake::drawFood()
{
    if(!isFoodOnScreen)
    {
        food.x=rand()%(Game::gameBoundry.x+Game::gameBoundry.w-SCALE-2-Game::gameBoundry.x+2+1)+Game::gameBoundry.x+2;
        food.y=rand()%(Game::gameBoundry.y+Game::gameBoundry.h-SCALE-2-Game::gameBoundry.y+2+1)+Game::gameBoundry.y+2;
        food.h=food.w=SCALE;
        isFoodOnScreen=true;
    }
    SDL_RenderCopy(Game::gameRenderer,foodTexture,NULL,&food);
}

bool Snake::isBoundryCollision()
{
    if(snakePositions[0].x<=Game::gameBoundry.x)
        return true;
    else if(snakePositions[0].y<=Game::gameBoundry.y)
        return true;
    else if(snakePositions[0].x+SNAKE_DIM>=Game::gameBoundry.x+Game::gameBoundry.w)
        return true;
    else if(snakePositions[0].y+SNAKE_DIM>=Game::gameBoundry.y+Game::gameBoundry.h)
        return true;
    else
        return false;
}
bool Snake::isFoodCollision()
{
    if(food.x>snakePositions[0].x+SCALE)
        return false;
    else if(food.y>snakePositions[0].y+SCALE)
        return false;
    else if(food.x+SCALE<snakePositions[0].x)
        return false;
    else if(food.y+SCALE<snakePositions[0].y)
        return false;
    else
        return true;
}

bool Snake::isSelfCollision()
{
    for(int i=1;i<length;i++)
    {
        if(snakePositions[0].x==snakePositions[i].x && snakePositions[0].y==snakePositions[i].y)
            return true;
    }
    return false;
}

void Snake::increaseSnakeSize()
{
    length+=3;
    snakePositions[length].x=snakePositions[length].y=-100;
}

