

#ifndef DB_H
#define DB_H

#include <vector>
#include <string>
#include "sqlite3.h"

using namespace std;
//database init

bool database_init(sqlite3*& db);
bool enableForeignKeys(sqlite3* db);
bool createTables(sqlite3* db);
void mainMenu(sqlite3* db);
//user login
bool authenticateUser(sqlite3* db, string &username);
bool registerUser(sqlite3* db);
int getUserId(sqlite3* db, const string& username);
//update username and pass
bool updateUsername(sqlite3* db, string& currentUsername);
bool updatePassword(sqlite3* db, const string& username) ;

// insert data to Database functions
int insertGameHistory(sqlite3* db, int user1_id, int user2_id, const string& winner, const string& finalBoard);
// show data from Database functions
bool showGameHistoryForPlayer(sqlite3* db, string username);
void showMovesForGame(sqlite3* db, int game_id);
void replayStoredGame(sqlite3* db, int game_id);
//playgame
void playGame(sqlite3* db, const string& player1, const string& player2) ;

// Structures
struct Move {
    int row, col;
    char player;
};

vector<Move> loadMovesForGame(sqlite3* db, int game_id);
bool insertGameMoves(sqlite3* db, int game_id, const vector<Move>& moves);

struct GameRecord {
    vector<Move> moves;
    string result;
};

struct GameInfo {
    int gameId;
    string opponent;
    string result;      // "win" / "loss" / "draw"
    string datePlayed;  // ISO timestamp
};


class ReplayManager {
private:
    vector<Move> moveStack;
    vector<GameRecord> gameHistory;
    char board[3][3];
    int xWins, oWins, draws;
    int currentMoveIndex;

public:
    ReplayManager();

    void resetBoard();

    void pushMove(int row, int col, char player);

    void undoMove();

    char checkWinner() const;

    bool isBoardFull() const ;

    void endGame();

    void displayBoard() const ;

    void displayHistory() const ;

    string getSerializedBoard();

    void replayGame(int gameIndex);

    void loadSerializedBoard(const string& boardString);
    
    char getCell(int row, int col) const ;
    
    vector<Move> getMoves() const ;
};
extern ReplayManager replayManager;

int registerUserGUI(sqlite3* db , const string& username , const string& password);

int authenticateUserGUI(sqlite3* db, const string& username , const string& password );

int fetchPlayerStats(sqlite3* db, int userId, string name, int& wins , int& losses , int& ties);

//update username and pass
bool updateUsername(sqlite3* db, string& currentUsername);
bool updatePassword(sqlite3* db, const string& username) ;

int updateUsernameGUI(sqlite3* db,const int & id , const string& currentUsername,const string& newUsername);
int updatePasswordGUI(sqlite3* db, const int & id , const string& password) ;

bool fetchGamesForPlayer(sqlite3* db, int userId, std::vector<GameInfo>& out);

bool loadMovesForGameGUI(sqlite3* db, int game_id , vector<Move>& out);

#endif
