#include "Match.h"

Match :: Match()
{
    player1 = new Player();
    player2 = new Player();

    points[0] = 0;
    points[1] = 0;

    matchTime = 0;
    winner = "";

    ended = 0;
}

Match::Match(Player *_player1, Player *_player2, unsigned int _id) : player1(_player1), player2(_player2)
{
    id = _id;
    points[0] = 0;
    points[1] = 0;

    matchTime = 0;
    winner = "";

    ended = 0;
}

void Match :: simGame()
{
    matchTime = 100;
    matchTime += rand() % 3501;
    while (points[0] < 5 && points[1] < 5)
    {
        int point = rand() % 2;
            if (point == 0)
            {
                points[0]++;
            }
            else 
            {
                points[1]++;
            }
    }
    setWinner(false);
}

void Match:: setWinner(bool _loaded) {

    cout << player1->getName() << ": " << points[0] << endl;
    cout << player2->getName() << ": " << points[1] << endl;

    if (points[0] >= 5 || points[1] >= 5)
    {
        if (points[0] > points[1])
        {
            cout << "ha ganado el jugador: " << player1->getName() << endl;
            player1->setWins(player1->getWins() + 1);
            winner = player1->getName();
        }
        else if (points[0] < points[1])
        {
            cout << "ha ganado el jugador: " << player2->getName() << endl;
            player2->setWins(player2->getWins() + 1);
            winner = player2->getName();
        }
        else if (points[0] == points[1])
        {
            cout << "han quedado empate" << endl;
        }      

        cout << "El partido ha durado " << matchTime << " segundos" << endl;

        ended = 1;
    }
    if (!_loaded)
    {
        player1->setMatches(player1->getMatches() + 1);
        player2->setMatches(player2->getMatches() + 1);
    }

    showStats();
    
}
void Match::showStats()
{
    cout << "id: " << id << " || Name1: " << player1->getName() << " || Points1: " << points[0] << " || Name2: " << player2->getName()<< " || Points2: "
        << points[1] << " || Time: " << matchTime << endl;
}
unsigned int Match::getID() { return id; } void Match::setID(unsigned int _id) { this->id = _id; }

