#include "Ball.h"

short Ball::getX() {
    return x;
}
short Ball::getY() {
    return y;
}
char Ball::getR() {
    return r;
}
void Ball::drawBall(SDL_Renderer* _renderer)
{
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w;
            int dy = r - h;
            if ((dx * dx + dy * dy) <= (r * r))
            {
                SDL_RenderDrawPoint(_renderer, x + dx, y + dy);
            }
        }
    }
}
void Ball::setBallPosition(SDL_Window* _window) {
    int width = SDL_GetWindowSurface(_window)->w;
    int height = SDL_GetWindowSurface(_window)->h;
    x = width / 2;
    y = height / 4;
}
void Ball::setBallRadius() {
    r = 30;
}
void Ball::addBallX(int _dX, int _dY) {
    x += _dX;
    y += _dY;
}

