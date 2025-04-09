#include "Player.h"


Player::Player()
{
    x = 0;
    id = 0;
    y = 0;
    r = 0;
    gravity = 0.5;
    velY = 100;
    serve = false;
    canJump = true;
    name = "";
    matches = 0;
    wins = 0;
}
Player::Player(string _name, unsigned int _id)
{
    name = _name; 
    id = _id;
}
string Player::writeName()
{
    string name;
    cout << "Player Name: ";
    cin >> name;
    name = name.substr(0, 4);

    return name;
}
void Player::showStats()
{
    cout << "id: " << id << " || Name: " << name << " || Matches: " << matches << " || Wins: " << wins << endl;
}

string Player::getName() { return name; } void Player::setName(string _name) { this-> name = _name; }
unsigned int Player::getID() { return id; } void Player::setID(unsigned int _id) { this-> id = _id; }
unsigned int Player::getMatches(){ return matches; } void Player::setMatches(unsigned int _matches) { this-> matches = _matches; }
unsigned int Player::getWins(){ return wins; } void Player::setWins(unsigned int _wins) { this-> wins = _wins; }

short Player::getX() { return x; }
short Player::getY() { return y; }
char  Player::getR() { return r; }
bool Player::getServe() { return serve; } void Player::setServe(bool _check) { serve = _check; }
bool Player::getCanJump() { return canJump; } void Player::setCanJump(bool _check) { canJump = _check; }

void Player::setPlayerPosition(int _positionX, int _positionY) {
    x = _positionX;
    y = _positionY;
}
void Player::setPlayerRadius() {
    r = 75;
}

void Player::addX(int _n) {
    x += _n;
}
void Player::substractX(int _n) {
    x -= _n;
}
void Player::substractY(int _n) {
    y -= _n;
}

void Player::drawPlayers(SDL_Renderer* _renderer)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
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
void Player::jumpUpdate() {
    y += 2;
    setCanJump(true);
}
