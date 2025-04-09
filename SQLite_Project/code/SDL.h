#pragma once
#include "Ball.h"
#include "Match.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>

#define FONT_SIZE 32



void write(SDL_Renderer*, string, int, int);

//Apartado visual del juego implementando los datos de la partida:
void gameVisualsController(Match&, bool);

