#pragma once
#include "Player.h"
#include "Ball.h"
#include <random>
#pragma warning(disable:4996)

using namespace std;

class Match
{
    //cambiar a get set
public:
    Player* player1 = NULL;
    Player* player2 = NULL;

    unsigned int matchTime = 0;
    string winner = "";
    int points[2] = {0,0};

    unsigned int id = 0;

    int ended = 0;

public:
    Match();
    Match(Player*,Player*,unsigned int);
    unsigned int getID(); void setID(unsigned int);
    void simGame();
    void setWinner(bool);
    void showStats();
};

