#pragma once
#include <SDL.h>

class Ball
{
private:
    //Atributos:
    short x = 0;
    short y = 0;
    char r = 0;
public:
    //Funciones
    short getX();
    short getY();
    char  getR();
    void drawBall(SDL_Renderer*);
    void setBallPosition(SDL_Window*);
    void setBallRadius();
    void addBallX(int, int);
};


