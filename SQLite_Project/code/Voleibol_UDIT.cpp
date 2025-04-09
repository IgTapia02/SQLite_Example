#include <iostream>
#include"Player.h"
#include "Match.h"
#include "Sqlite_Lib.h"
#include "Winsock.h"
#include <SDL.h>
#include <stdio.h>
#include "SDL.h"

#pragma warning(disable:4996)

#include <iostream>
#include <sstream>
#include "sqlite3.h"


using namespace std;

int menu();
bool deleteMatch();
bool selectPlayers(Match &_match, Player _playerBuffer[], Match _matchBuffer[], int _modeOption);
bool loadMatch(Match _matchBuffer[], Player _playerBuffer[]);

int main(int argc, char* args[])
{
    Player playerBuffer[100];
    Match matchBuffer[100];

    Match match;
    Player player;

    int option = 0;

    openTable();
    tableExist("Players");
    tableExist("Matchs");
    updatePlayerBuffer(playerBuffer);
    updateMatchBuffer(matchBuffer);

    while (option != 8) {

        option = menu();

        switch (option) {

        case 1:
            system("cls");
            player = Player();
            player.setName(player.writeName());
            player.setID(readLastID("Players") + 1);
            playerBuffer[numberOfLines("Players")] = player;
            insertPlayers(player);
            system("pause");
            break;
        case 2:
            system("cls");
            printTableData("Players");
            system("pause");
            break;
        case 3:
            system("cls");
            printTableData("Matchs");
            system("pause");
            break;
        case 4:
            system("cls");
            selectPlayers(match, playerBuffer,matchBuffer,0);
            system("pause");
            break;
        case 5:
            system("cls");
            loadMatch(matchBuffer,playerBuffer);
            system("pause");
            break;
        case 6:
            system("cls");
            selectPlayers(match, playerBuffer, matchBuffer, 1);
            system("pause");
            break;
        case 7:
            system("cls");
            deleteMatch();
            updateMatchBuffer(matchBuffer);
            system("pause");
            break;
        default:
            break;
        }
    }
    free(playerBuffer);
    free(matchBuffer);
    closeSqlite();
    return 0;
}

int menu()
{
    system("cls");
    int option = 0;
    printf("UDIT VOLLEY\n");
    printf("############\n");
    printf("1: Crear jugador\n");
    printf("2: Ranking\n");
    printf("3: Mostrar Juegos\n");
    printf("4: Jugar partida\n");
    printf("5: Cargar partida\n");
    printf("6: Simular Partida\n");
    printf("7: Borrar Partida\n");
    printf("8: Salir\n");

    cin >> option;
    return option;
}

bool deleteMatch()
{
    int number = 0;

    printTableData("Matchs");

    cout << "ID del partido que deseas eliminar:  ";
    cin >> number;

    if (number <= 0 || number > readLastID("Matchs")) {
        cout << "Valor de numero incorrecto"<< endl;
        return false;
    }

    cout << "Se elimino el partido " << number << " correctamente" <<endl;

    deleteLine(number, "Matchs");

    return true;
}

bool selectPlayers(Match &_match, Player _playerBuffer[], Match _matchBuffer[], int _modeOption)
{
    for (int i = 0; i < numberOfLines("Players"); i++)
    {
        cout << i + 1 <<": ";
        _playerBuffer[i].showStats();
    }

    if (readLastID("Players") < 2)
    {
        cout << "No hay suficientes jugadores"<< endl;
        return false;
    }
    int opcion1;
    int opcion2;

    cout << "Player1: ";
    cin >> opcion1;
    cout << "Player2: ";
    cin >> opcion2;

    if (opcion1<0 || opcion1> numberOfLines("Players") || opcion2<0 || opcion2> numberOfLines("Players") || opcion1 == opcion2)
    {
        cout << "Datos erroneos" << endl;
        return false;
    }

    unsigned int matchId = readLastID("Matchs") + 1;

    _match = Match(&_playerBuffer[opcion1 - 1], &_playerBuffer[opcion2 - 1],matchId);
    system("cls");
    if (_modeOption == 0)
    {
        gameVisualsController(_match, false);
    }
    else 
    {
        _match.simGame();
    }
    
    updatePlayerTable(_playerBuffer, _playerBuffer[opcion1 - 1].getID(), _playerBuffer[opcion2 - 1].getID());

    _matchBuffer[numberOfLines("Matchs")] = _match;

    insertMatchs(_match);

    return true;
}

bool loadMatch(Match _matchBuffer[],Player _playerBuffer[])
{
    int opcion1 = 0;

    if (readLastID("Matchs") < 1)
    {
        cout << "No hay partidas" << endl;
        return false;
    }
    for (int i = 0; i < numberOfLines("Matchs"); i++)
    {
        if (_matchBuffer[i].ended == 0)
        {
            cout << i + 1 << ": ";
            _matchBuffer[i].showStats();
        }
    }
        
    cout << "Juego a cargar: ";
    cin >> opcion1;

    if (opcion1 < 1 || opcion1 > numberOfLines("Matchs") || _matchBuffer[opcion1 - 1].ended != 0)
    {
        cout << "Opcion invalida o juego ya finalizado" << endl;
        return false;
    }
    system("cls");
    gameVisualsController(_matchBuffer[opcion1 - 1], true);
    updateMatchTable(_matchBuffer, _matchBuffer[opcion1 - 1].getID());
    updatePlayerTable(_playerBuffer, _matchBuffer[opcion1 - 1].player1->getID(), _matchBuffer[opcion1 - 1].player2->getID());

    return true;

}
