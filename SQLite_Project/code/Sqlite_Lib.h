#pragma warning(disable:4996)

#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
#include "Player.h"
#include "Match.h"

bool openTable();
bool closeSqlite();
int callback(void* _data, int _argc, char** _argv, char** _azColName);
bool tableExist(const string& _name);
bool printTableData(string _name);
bool Print_Table_Data_Ordered();
bool insertPlayers(Player _player);
bool insertMatchs(Match _match);
int readLastID(string _name);
bool deleteLine(int _id, string _name);
int numberOfLines(string _name);
bool updatePlayerBuffer(Player _playerBuffer[]);
bool updateMatchBuffer(Match _matchBuffer[]);
bool updatePlayerTable(Player _playerBuffer[], int _ID1, int _ID2);
bool updateMatchTable(Match _matchBuffer[], int _ID);
