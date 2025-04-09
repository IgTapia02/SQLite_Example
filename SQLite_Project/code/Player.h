#pragma warning(disable:4996)

#pragma once
#include <iostream>
#include <SDL.h>
using namespace std;

class Player
{
    //cambiar a get set
private:

    short x = 0;
    short y = 0;
    char r = 0;

    float gravity = 0.5;
    char  velY = 100;

    bool serve = false;
    bool canJump = true;

    unsigned int id = 0;
    string name;
    unsigned int matches = 0;
    unsigned int wins = 0;

public:

    Player();
    Player(string, unsigned int);
    string writeName();

    string getName(); void setName(string);
    unsigned int getID(); void setID(unsigned int);
    unsigned int getMatches(); void setMatches(unsigned int);
    unsigned int getWins(); void setWins(unsigned int);

    short getX();
    short getY();
    char  getR();
    void setPlayerPosition(int, int);
    void setPlayerRadius();

    bool getServe(); void setServe(bool);
    bool getCanJump();void setCanJump(bool);
   
    void drawPlayers(SDL_Renderer*);
    void jumpUpdate();

    void addX(int);
    void substractX(int);
    void substractY(int);

    void showStats();
};

