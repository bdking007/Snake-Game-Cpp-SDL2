#include "Game.h"

Snake snake;

SDL_Rect Game::gameBoundry={2*SCALE,4*SCALE,SCREEN_DIM-4*SCALE,SCREEN_DIM-6*SCALE};

Game::Game()
{
    //ctor
    gameSurface=nullptr;   gameWindow=nullptr;
    playStart=false;   score=0;  font=nullptr;
    textTexture=nullptr;
    foodEatenEffect=nullptr;  outEffect=nullptr;  gameOverEffect=nullptr;
}

Game::~Game()
{
    //dtor
    gameSurface=nullptr;    gameRenderer=nullptr;
    gameWindow=nullptr;     font=nullptr;
    textTexture=nullptr;
    foodEatenEffect=nullptr;  outEffect=nullptr;  gameOverEffect=nullptr;
}

SDL_Renderer *Game::gameRenderer=nullptr;

SDL_Texture* Game::loadTexture(string path,int r,int g,int b)
{
    SDL_Surface *tempImgSurface=IMG_Load(path.c_str());
    SDL_Texture *retTexture=nullptr;
    if(tempImgSurface==nullptr)
    {
        cout<<"Image not found : "<<path<<endl;
    }
    else
    {
        SDL_SetColorKey(tempImgSurface,SDL_TRUE,SDL_MapRGB(tempImgSurface->format,r,g,b));
        retTexture=SDL_CreateTextureFromSurface(gameRenderer,tempImgSurface);
        SDL_FreeSurface(tempImgSurface);   tempImgSurface=nullptr;
    }
    return retTexture;
}

void Game::run()
{
    if(!initSystem())
    {
        cout<<"Error in INITIALIZING System "<<endl; cin.get(); return;
    }
    if(!loadFiles())
    {
        cout<<"Error in Loading Files "<<endl;  cin.get();  return;
    }

    gameLoop();

    cleanSystem();
}

bool Game::initSystem()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

    gameWindow=SDL_CreateWindow("The Snake Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_DIM,SCREEN_DIM,SDL_WINDOW_SHOWN);
    gameSurface=SDL_GetWindowSurface(gameWindow);
    gameRenderer=SDL_CreateRenderer(gameWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(gameRenderer,255,255,255,255);

    return true;
}

void Game::cleanSystem()
{
    SDL_FreeSurface(gameSurface);
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyWindow(gameWindow);
    Mix_FreeChunk(foodEatenEffect);   Mix_FreeChunk(outEffect);  Mix_FreeChunk(gameOverEffect);

    Mix_Quit();  TTF_Quit();  IMG_Quit();  SDL_Quit();
}

bool Game::loadFiles()
{
    gameOverTexture=loadTexture("assets/images/gameOver.png",255,255,255);
    font=TTF_OpenFont("assets/fonts/neuropol.ttf",50);
    gameOverEffect=Mix_LoadWAV("assets/music/gameOver.wav");
    outEffect=Mix_LoadWAV("assets/music/out.wav");
    foodEatenEffect=Mix_LoadWAV("assets/music/foodEaten.wav");
    if(!snake.loadSnake() || gameOverTexture==nullptr || font==nullptr || gameOverEffect==nullptr
        || outEffect==nullptr || foodEatenEffect==nullptr)
    {
        return false;
    }
    else
        return true;
}

void Game::handleGameEvents()
{
    while(SDL_PollEvent(&gameEvents))
    {
        switch(gameEvents.type)
        {
            case SDL_QUIT :
                cleanSystem();
                break;
            case SDL_KEYDOWN :
                if(gameEvents.key.keysym.sym==SDLK_UP)
                {
                    if(snake.snakeSpeedY==0)
                    {
                        snake.snakeSpeedX=0;  snake.snakeSpeedY=-snake.SNAKE_SPEED;
                    }
                }
                else if(gameEvents.key.keysym.sym==SDLK_DOWN)
                {
                    if(snake.snakeSpeedY==0)
                    {
                        snake.snakeSpeedX=0;  snake.snakeSpeedY=snake.SNAKE_SPEED;
                    }
                }
                else if(gameEvents.key.keysym.sym==SDLK_LEFT)
                {
                    if(snake.snakeSpeedX==0)
                    {
                        snake.snakeSpeedY=0;  snake.snakeSpeedX=-snake.SNAKE_SPEED;
                    }
                }
                else if(gameEvents.key.keysym.sym==SDLK_RIGHT)
                {
                    if(snake.snakeSpeedX==0)
                    {
                        snake.snakeSpeedY=0;  snake.snakeSpeedX=+snake.SNAKE_SPEED;
                    }
                }

                break;
        }
    }
}
void Game::updateScreen()
{
    SDL_SetRenderDrawColor(gameRenderer,255,255,255,255);
    SDL_RenderClear(gameRenderer);

    SDL_SetRenderDrawColor(gameRenderer,204, 204, 179,255);
    SDL_RenderFillRect(gameRenderer,&gameBoundry);

    snake.drawFood();
    snake.moveSnake();
    if(snake.isFoodCollision())
    {
        snake.increaseSnakeSize();  score+=10;
        if(score%200==0)
        {
            snake.SNAKE_SPEED++;  score+=5;
        }
        Mix_PlayChannel(-1,foodEatenEffect,0);
        snake.isFoodOnScreen=false;
    }
    snake.drawSnake();

    if(snake.isBoundryCollision() || snake.isSelfCollision())
    {
        playStart=false;   Mix_PlayChannel(-1,outEffect,0);
        SDL_Delay(1500);
    }

    renderScore();
}

void Game::renderChanges()
{
    SDL_RenderPresent(gameRenderer);
}

void Game::gameLoop()
{
    playStart=true;
    while(playStart)
    {
        Uint32 startTime=SDL_GetTicks();
        handleGameEvents();
        updateScreen();
        renderChanges();

        Uint32 endTime=SDL_GetTicks()-startTime;
        if(endTime<FPS/1000)
            SDL_Delay(endTime-FPS/1000);
    }
    playStart=false;
    stringstream finalSc("");
    finalSc<<"Your Score : "<<score;
    textTexture=loadText(finalSc.str(),255,255,255);
    SDL_Rect r={170,460,420-170,540-460};
    SDL_RenderCopy(gameRenderer,gameOverTexture,NULL,NULL);
    SDL_RenderCopy(gameRenderer,textTexture,NULL,&r);
    SDL_RenderPresent(gameRenderer);
    Mix_PlayChannel(-1,gameOverEffect,0);
    while(true)
    {
        while(SDL_PollEvent(&gameEvents))
        {
            if(gameEvents.type==SDL_QUIT)
                cleanSystem();
        }
    }

}

SDL_Texture* Game::loadText(string text,Uint8 r,Uint8 g,Uint8 b)
{
    SDL_Color colorT={r,g,b};
    SDL_Surface *textSurface=TTF_RenderText_Solid(font,text.c_str(),colorT);
    SDL_Texture *retTexture=SDL_CreateTextureFromSurface(gameRenderer,textSurface);
    SDL_FreeSurface(textSurface);   textSurface=nullptr;
    return retTexture;
}
void Game::renderScore()
{
    SDL_SetRenderDrawColor(gameRenderer,173, 51, 255,255);
    SDL_Rect scoreRect={gameBoundry.x,2,gameBoundry.w,gameBoundry.y-3};
    SDL_RenderFillRect(gameRenderer,&scoreRect);

    stringstream finalSc("");
    finalSc<<" Score : "<<score<<" ";
    textTexture=loadText(finalSc.str(),0,0,0);
    SDL_Rect r={gameBoundry.x+13*SCALE,5,200,gameBoundry.y-SCALE};
    SDL_RenderCopy(gameRenderer,textTexture,NULL,&r);
}
