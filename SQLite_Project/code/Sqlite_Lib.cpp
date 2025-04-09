#include "Sqlite_Lib.h"
using namespace std;

//almacena la cadena sql
string sql;

//Puntero para la conexion de SQL
sqlite3* db;

//Mensaje de error
char* zErrMsg = 0;

//Resultado de abrir el archivo
int rc;

//dbEaber para mirar sqlite
//abrir la base de datos al iniciar el programa; cerrarla al haber un error y volverla a abrir, cerrarla al finalizar el programa
int callback(void* _data, int _argc, char** _argv, char** _azColName) 
{

    for (int i = 0; i < _argc; i++)
    {
        printf("%s = %s || ", _azColName[i], _argv[i] ? _argv[i] : "NULL");
    }
    printf("\n");
    return 0;

}

bool openTable() {

    rc = sqlite3_open("Tables.db", &db);

    if (rc != SQLITE_OK) {

        cout << zErrMsg << endl;
        return false;
    }

    return true;
}

bool closeSqlite() {

    sqlite3_close(db);
    return true;
}

//comprueba si existe la tabla name, si no existe la crea
bool tableExist(const string& _name) 
{

    if (_name == "Players") {
        sql = "CREATE TABLE IF NOT EXISTS Players ("
            "id INT PRIMARY KEY NOT NULL,"
            "name TEXT NOT NULL,"
            "games INT NOT NULL,"
            "wins INT NOT NULL);";

        rc = sqlite3_exec(db, sql.c_str(), callback, (void*)0, &zErrMsg);
    }
    else if (_name == "Matchs") {
        sql = "CREATE TABLE IF NOT EXISTS Matchs ("
            "id INT PRIMARY KEY NOT NULL,"
            "name1 TEXT NOT NULL,"
            "points1 INT NOT NULL,"
            "name2 TEXT NOT NULL,"
            "points2 INT NOT NULL,"
            "time INT NOT NULL,"
            "winner TEXT NOT NULL,"
            "ended INT NOT NULL);";

        rc = sqlite3_exec(db, sql.c_str(), callback, (void*)0, &zErrMsg);
    }
    else {

        return false;
    }

    return true;
}
// Función para imprimir los datos de la tabla name
bool printTableData(string _name) 
{
        string sql = "SELECT * FROM " + _name + ";";

        rc = sqlite3_exec(db, sql.c_str(), callback, NULL, &zErrMsg);

        if (rc != SQLITE_OK) {
            cout << zErrMsg << endl;
            return false;
        }

    return true;
}
//imprimir tabla players en orden de Wins
bool Print_Table_Data_Ordered() {
    string sql = "SELECT * FROM Players ORDER BY wins DESC;";

    rc = sqlite3_exec(db, sql.c_str(), callback, NULL, &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << zErrMsg << endl;
        return false;
    }

    return true;
}
//Inserta el jugador dado en la tabla
bool insertPlayers(Player _player)
{
    sqlite3_stmt* stmt = NULL;

    const char* sql = "INSERT INTO Players(id, name, games, wins) VALUES(?,?,?,?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, _player.getID());
    sqlite3_bind_text(stmt, 2, _player.getName().c_str(), _player.getName().length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, _player.getMatches());
    sqlite3_bind_int(stmt, 4, _player.getWins());

    int result = sqlite3_step(stmt);

    return true;
}
//Inserta el partido dado en la tabla
bool insertMatchs(Match _match)
{
    sqlite3_stmt* stmt = NULL;

    const char* sql = "INSERT INTO Matchs (id, name1, points1, name2, points2, time, winner, ended) VALUES (?,?,?,?,?,?,?,?);";

    //Preparar la sentencia SQL
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // Vincular los valores a los marcadores de posición
    sqlite3_bind_int(stmt, 1, _match.getID());
    sqlite3_bind_text(stmt, 2, _match.player1->getName().c_str(), _match.player1 -> getName().length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, _match.points[0]);
    sqlite3_bind_text(stmt, 4, _match.player2->getName().c_str(), _match.player2->getName().length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, _match.points[1]);
    sqlite3_bind_int(stmt, 6, _match.matchTime);
    sqlite3_bind_text(stmt, 7, _match.winner.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 8, _match.ended);

    // Ejecutar la sentencia
    int result = sqlite3_step(stmt);

    return true;
}
//Lee todos los id de la tabla Players y devuelve el ultimo
int readLastID(string _name)
{
    string sql = "SELECT * FROM " + _name + ";";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cout << "Error" << endl;
        return 0;
    }

    int lastId = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        int id = sqlite3_column_int(stmt, 0);

        lastId = id;
    }

    if (rc != SQLITE_DONE) {
        cout << "Error" << endl;
        return 0;
    }

    sqlite3_finalize(stmt);

    if (lastId < 0)
    {
       lastId = 0;
    }

    return lastId;
}
//borra una linea de la tabla que le pasamos en name
bool deleteLine(int _id, string _name) 
{
    // Construir la consulta DELETE
    string sql = "DELETE FROM " + _name + " WHERE id = " + to_string(_id) + "; ";

    // Ejecutar la consulta DELETE
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        cout << zErrMsg << endl;
        return false;
    }

    return true;
}
//actualiza el buffer de Player con la informacion de la tabla al iniciar el programa
bool updatePlayerBuffer(Player _playerBuffer[])
{
    int bufferPos = 0;

    const char* sql = "SELECT * FROM Players;";
    sqlite3_stmt* stmt;

    // Preparar la sentencia SQL
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cout << "Error" << endl;
        return false;
    }
    // Ejecutar la sentencia
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        Player player;

        player.setID(sqlite3_column_int(stmt, 0));
        player.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        player.setMatches(sqlite3_column_int(stmt, 2));
        player.setWins(sqlite3_column_int(stmt, 3));

        _playerBuffer[bufferPos] = player;
        bufferPos++;

    }

    if (rc != SQLITE_DONE) {
        cout << "Error" << endl;
        return 0;
    }

    // Finalizar la sentencia después de usarla
    sqlite3_finalize(stmt);

    return true;
}
//actualiza el buffer de Matchs con la informacion de la tabla al iniciar el programa
bool updateMatchBuffer(Match _matchBuffer[])
{
    int bufferPos = 0;

    const char* sql = "SELECT * FROM Matchs;";
    sqlite3_stmt* stmt;

    // Preparar la sentencia SQL
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cout << zErrMsg << endl;
        return false;
    }
    // Ejecutar la sentencia
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        Match match;

        match.id = sqlite3_column_int(stmt, 0);
        match.player1->setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        match.points[0] = sqlite3_column_int(stmt, 2);
        match.player2->setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        match.points[1] = sqlite3_column_int(stmt, 4);
        match.matchTime = sqlite3_column_int(stmt, 5);
        match.winner = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        match.ended = sqlite3_column_int(stmt, 7);

        _matchBuffer[bufferPos] = match;

        bufferPos++;

    }

    if (rc != SQLITE_DONE) {
        cout << zErrMsg << endl;
        return 0;
    }

    // Finalizar la sentencia después de usarla
    sqlite3_finalize(stmt);

    return true;
}
//lee cuantas lineas hay en la tabla name
int numberOfLines(string _name) 
{
    int n = 0;
    if (_name == "Players")
    {
        const char* sql = "SELECT * FROM Players;";
        sqlite3_stmt* stmt;

        // Preparar la sentencia SQL
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc != SQLITE_OK) {
            cout << zErrMsg << endl;
            return 0;
        }
        // Ejecutar la sentencia
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

            n++;
        }

        if (rc != SQLITE_DONE) {
            cout << zErrMsg << endl;
            return 0;
        }

        // Finalizar la sentencia después de usarla
        sqlite3_finalize(stmt);
    }
    if (_name == "Matchs")
    {
        const char* sql = "SELECT * FROM Matchs;";
        sqlite3_stmt* stmt;

        // Preparar la sentencia SQL
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

        if (rc != SQLITE_OK) {
            cout << "Error al ejecutar la consulta" << endl;
            return 0;
        }
        // Ejecutar la sentencia
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

            n++;
        }

        if (rc != SQLITE_DONE) {
            cout << "Error al ejecutar la consulta" << endl;
            return 0;
        }

        // Finalizar la sentencia después de usarla
        sqlite3_finalize(stmt);
    }

    return n;
}
//actualiza las filas con los id dados de la tabla Player 
bool updatePlayerTable(Player _playerBuffer[], int _ID1, int _ID2)
{
    sqlite3_stmt* stmt = NULL;

    const char* sql = "UPDATE Players SET games=?, wins=? WHERE id=?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    for (int i = 0; i < numberOfLines("Players"); i++)
    {
        if (_playerBuffer[i].getID() == _ID1)
        {
            sqlite3_bind_int(stmt, 1, _playerBuffer[i].getMatches());
            sqlite3_bind_int(stmt, 2, _playerBuffer[i].getWins());
            sqlite3_bind_int(stmt, 3, _ID1);

            int result = sqlite3_step(stmt);

            if (result != SQLITE_DONE) {
                cout << "Error al ejecutar la consulta" << endl;
                sqlite3_finalize(stmt);
                return false;
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }

    for (int i = 0; i < numberOfLines("Players"); i++)
    {
        if (_playerBuffer[i].getID() == _ID2)
        {
            sqlite3_bind_int(stmt, 1, _playerBuffer[i].getMatches());
            sqlite3_bind_int(stmt, 2, _playerBuffer[i].getWins());
            sqlite3_bind_int(stmt, 3, _ID2);

            int result = sqlite3_step(stmt);

            if (result != SQLITE_DONE) {
                cout << "Error al ejecutar la consulta" << endl;
                sqlite3_finalize(stmt);
                return false;
            }
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }

    sqlite3_finalize(stmt);

    return true;
}
bool updateMatchTable(Match _matchBuffer[], int _ID)
{
    sqlite3_stmt* stmt = NULL;

    const char* sql = "UPDATE Matchs SET points1=?, points2=?, time=?, winner=?, ended=? WHERE id=?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    for (int i = 0; i < numberOfLines("Matchs"); i++)
    {
        if (_matchBuffer[i].getID() == _ID)
        {
            sqlite3_bind_int(stmt, 1, _matchBuffer[i].points[0]);
            sqlite3_bind_int(stmt, 2, _matchBuffer[i].points[1]);
            sqlite3_bind_int(stmt, 3, _matchBuffer[i].matchTime);           
            sqlite3_bind_text(stmt, 4, _matchBuffer[i].winner.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, _matchBuffer[i].ended);
            sqlite3_bind_int(stmt, 6, _ID);

            int result = sqlite3_step(stmt);

            if (result != SQLITE_DONE) {
                cout << zErrMsg << endl;
                sqlite3_finalize(stmt);
                return false;
            }

            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
    }

    sqlite3_finalize(stmt);

    return true;
}
