#include <iostream>
#include "database.h"
#include "bcrypt.h"

using namespace std;


// ReplayManager Class


ReplayManager::ReplayManager() : xWins(0), oWins(0), draws(0), currentMoveIndex(0) {
    resetBoard();
}

void ReplayManager::resetBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
    moveStack.clear();
    currentMoveIndex = 0;
}

void ReplayManager::pushMove(int row, int col, char player) {
    moveStack.push_back({row, col, player});
    board[row][col] = player;
}

void ReplayManager::undoMove() {
    if (!moveStack.empty()) {
        Move last = moveStack.back();
        moveStack.pop_back();
        board[last.row][last.col] = ' ';
    }
}

char ReplayManager::checkWinner() const {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];
    return ' ';
}

bool ReplayManager::isBoardFull() const {
    return moveStack.size() == 9;
}

void ReplayManager::endGame() {
    GameRecord record;
    for (const auto& move : moveStack) {
        record.moves.push_back(move);
    }

    char winner = checkWinner();
    if (winner == 'X') {
        record.result = "X wins!";
        xWins++;
    } else if (winner == 'O') {
        record.result = "O wins!";
        oWins++;
    } else if (isBoardFull()) {
        record.result = "Draw";
        draws++;
    } else {
        record.result = "Game not finished";
    }

    gameHistory.push_back(record);
    resetBoard();
}

void ReplayManager::displayBoard() const {
    cout << "Current board:\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
}

void ReplayManager::displayHistory() const {
    cout << "\nGame History:\n";
    for (size_t i = 0; i < gameHistory.size(); ++i) {
        cout << "Game " << i + 1 << ": " << gameHistory[i].result << "\n";
        for (const auto& move : gameHistory[i].moves) {
            cout << "  Player " << move.player
                    << " to (" << move.row << "," << move.col << ")\n";
        }
    }
    cout << "\nStatistics:\n";
    cout << "X Wins: " << xWins << "\n";
    cout << "O Wins: " << oWins << "\n";
    cout << "Draws: " << draws << "\n";
}

string ReplayManager::getSerializedBoard() {
    string result;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result += board[i][j];
    return result;
}

void ReplayManager::replayGame(int gameIndex) {
    if (gameIndex < 0 || gameIndex >= (int)gameHistory.size()) {
        cout << "Invalid game index.\n";
        return;
    }
    const auto& record = gameHistory[gameIndex];
    resetBoard();
    currentMoveIndex = 0;

    char command;
    while (true) {
        displayBoard();
        cout << "Move " << currentMoveIndex << "/" << record.moves.size() << "\n";cout << "n: next move, p: previous move, q: quit replay\n";
        cin >> command;
        if (command == 'n') {
            if (currentMoveIndex < (int)record.moves.size()) {
                Move move = record.moves[currentMoveIndex];
                board[move.row][move.col] = move.player;
                currentMoveIndex++;
            } else {
                cout << "End of game.\n";
            }
        } else if (command == 'p') {
            if (currentMoveIndex > 0) {
                currentMoveIndex--;
                Move move = record.moves[currentMoveIndex];
                board[move.row][move.col] = ' ';
            } else {
                cout << "Start of game.\n";
            }
        } else if (command == 'q') {
            break;
        }
    }
}

void ReplayManager::loadSerializedBoard(const string& boardString) {
    int index = 0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = boardString[index++];
}

char ReplayManager::getCell(int row, int col) const {
    return board[row][col];
}

vector<Move> ReplayManager::getMoves() const { return moveStack; }
ReplayManager replayManager;


void playGame(sqlite3* db, const string& player1, const string& player2) {
    ReplayManager game;
    char currentPlayer = 'X';
    int row, col;
    bool gameRunning = true;

    while (gameRunning) {
        game.displayBoard();
        cout << "Player " << currentPlayer << "'s turn:\n";
        cout << "Enter row: ";
        cin >> row;
        cout << "Enter col: ";
        cin >> col;

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            cout << "Invalid position. Try again.\n";
            continue;
        }
        if (game.getCell(row, col) != ' ') {
        cout << "That position is already taken. Try a different one.\n";
        continue;
        }
        game.pushMove(row, col, currentPlayer);

        char winner = game.checkWinner();
        if (winner != ' ') {
            game.displayBoard();
            cout << "Player " << winner << " wins!\n";
            string finalBoardState = game.getSerializedBoard();


            // Save to database
            int player1_id = getUserId(db, player1);
            int player2_id = getUserId(db, player2);
            string winnerName = (winner == 'X') ? player1 : player2;
            int game_id=insertGameHistory(db, player1_id, player2_id, winnerName, finalBoardState);
            if (game_id != -1) insertGameMoves(db, game_id, game.getMoves());
             game.endGame();
            gameRunning = false;
        } else if (game.isBoardFull()) {
            game.displayBoard();
            cout << "It's a draw!\n";
            string finalBoardState = game.getSerializedBoard();


            // Save to database
            int player1_id = getUserId(db, player1);
            int player2_id = getUserId(db, player2);
            int game_id=insertGameHistory(db, player1_id, player2_id, "Draw", finalBoardState);
            if (game_id != -1) insertGameMoves(db, game_id, game.getMoves());
            game.endGame();
            gameRunning = false;

        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}
bool database_init(sqlite3*& db) {
    int rc = sqlite3_open("tictac.db", &db);
    if (rc != SQLITE_OK) {
        cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!enableForeignKeys(db)) return false;    // Enable foreign keys

    if (!createTables(db)) return false;
    return true;
}
bool enableForeignKeys(sqlite3* db) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "Error enabling foreign keys: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
bool createTables(sqlite3* db) {
    char* errMsg = nullptr;

    string sql_players = R"(
        CREATE TABLE IF NOT EXISTS PLAYERS(
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            NAME TEXT UNIQUE NOT NULL,
            PASSWORD TEXT NOT NULL
        );
    )";

    if (sqlite3_exec(db, sql_players.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cout << "Error creating PLAYERS table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    string sql_game_history = R"(
        CREATE TABLE IF NOT EXISTS Game_history (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            user1_id INTEGER,
            user2_id INTEGER,
            winner TEXT,
            board_moves TEXT,
            date_played TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user1_id) REFERENCES PLAYERS(ID),
            FOREIGN KEY (user2_id) REFERENCES PLAYERS(ID)
        );
    )";

    if (sqlite3_exec(db, sql_game_history.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cout << "Error creating Game_history table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    string sql_game_moves = R"(
        CREATE TABLE IF NOT EXISTS Game_moves (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            game_id INTEGER NOT NULL,
            move_number INTEGER NOT NULL,
            row INTEGER NOT NULL,
            col INTEGER NOT NULL,
            player CHAR(1) NOT NULL,
            FOREIGN KEY (game_id) REFERENCES Game_history(ID)
        );
    )";

    if (sqlite3_exec(db, sql_game_moves.c_str(), nullptr, 0, &errMsg) != SQLITE_OK) {
        cout << "Error creating Game_moves table: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}


void mainMenu(sqlite3* db) {

    int choice;
    string loggedInUser;

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            registerUser(db);
        } else if (choice == 2) {
            if (authenticateUser(db, loggedInUser)) {
                int subChoice;
                while (true) {
                    cout << "\nWelcome, " << loggedInUser << "!\n";
                cout << "1. Start New Game\n2. Show My Game History\n3. Show Moves\n4. Sign Out\nEnter your choice: ";
                    cin >> subChoice;

                    if (subChoice == 1) {
                        string opponent;
                        cout << "Enter opponent's username: ";
                        if (authenticateUser(db, opponent)) {
                            playGame(db, loggedInUser, opponent);
                        } else {
                            cout << "Opponent authentication failed.\n";
                        }
                    } else if (subChoice == 2) {
                        showGameHistoryForPlayer(db, loggedInUser);
                    } else if (subChoice == 3) {
                          // Get player ID first
                        int playerID = -1;
                        sqlite3_stmt* getUserStmt;
                        string getUserIdSQL = "SELECT ID FROM PLAYERS WHERE NAME = ?;";
                        if (sqlite3_prepare_v2(db, getUserIdSQL.c_str(), -1, &getUserStmt, NULL) == SQLITE_OK) {
                            sqlite3_bind_text(getUserStmt, 1, loggedInUser.c_str(), -1, SQLITE_STATIC);
                            if (sqlite3_step(getUserStmt) == SQLITE_ROW) {
                                playerID = sqlite3_column_int(getUserStmt, 0);
                            }
                            sqlite3_finalize(getUserStmt);
                        }
                        if (playerID == -1) {
                            cout << "Error retrieving player ID.\n";
                            break;
                        }
                        // Fetch games and simulate row numbering
                        string query = "SELECT ID, user1_id, user2_id, winner, date_played FROM Game_history "
                                       "WHERE user1_id = ? OR user2_id = ? ORDER BY date_played;";
                        sqlite3_stmt* stmt;
                        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
                            sqlite3_bind_int(stmt, 1, playerID);
                            sqlite3_bind_int(stmt, 2, playerID);

                            vector<int> gameIDs;
                            int rowNum = 1;
                            cout << "\nYour Games:\n";
                            while (sqlite3_step(stmt) == SQLITE_ROW) {
                                int id = sqlite3_column_int(stmt, 0);
                                const char* winner = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                                const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

                                cout << rowNum << ". Game ID: " << id << ", Winner: " << (winner ? winner : "None") << ", Date: " << (date ? date : "Unknown") << "\n";
                                gameIDs.push_back(id);
                                rowNum++;
                            }
                            sqlite3_finalize(stmt);

                            if (gameIDs.empty()) {
                                cout << "No games found.\n";
                            } else {
                                int choiceRow;
                                cout << "Enter the row number of the game to view moves: ";
                                cin >> choiceRow;

                                if (choiceRow > 0 && choiceRow <= gameIDs.size()) {
                                    int selectedGameID = gameIDs[choiceRow - 1];
                                    showMovesForGame(db, selectedGameID);
                                } else {
                                    cout << "Invalid row number.\n";
                                }
                            }
                } else {
                    cerr << "Error querying games: " << sqlite3_errmsg(db) << endl;
                }
                                } else if (subChoice == 4) {
                                    cout << "Signing out...\n";
                                    break;
                                } else {
                                    cout << "Invalid option.\n";
                                }
                            }
            } else {
                cout << "Login failed.\n";
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

}

bool insertGameMoves(sqlite3* db, int game_id, const vector<Move>& moves) {
    string insertSql = "INSERT INTO Game_moves (game_id, move_number, row, col, player) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertSql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing move insert: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    for (size_t i = 0; i < moves.size(); ++i) {
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, game_id);
        sqlite3_bind_int(stmt, 2, (int)i + 1);
        sqlite3_bind_int(stmt, 3, moves[i].row);
        sqlite3_bind_int(stmt, 4, moves[i].col);
        sqlite3_bind_text(stmt, 5, string(1, moves[i].player).c_str(), 1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            cerr << "Error inserting move: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            return false;
        }
    }
    sqlite3_finalize(stmt);
    return true;
}
vector<Move> loadMovesForGame(sqlite3* db, int game_id) {
    vector<Move> moves;
    string query = "SELECT row, col, player FROM Game_moves WHERE game_id = ? ORDER BY move_number ASC;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare load moves statement: " << sqlite3_errmsg(db) << endl;
        return moves;
    }
    sqlite3_bind_int(stmt, 1, game_id);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int row = sqlite3_column_int(stmt, 0);
        int col = sqlite3_column_int(stmt, 1);
        const unsigned char* playerText = sqlite3_column_text(stmt, 2);
        char player = playerText ? playerText[0] : ' ';
        moves.push_back({row, col, player});
    }
    sqlite3_finalize(stmt);
    return moves;
}

bool authenticateUser(sqlite3* db, string& username) {
    string enteredUsername;
    cout << "Enter username: ";
    cin >> enteredUsername;

    string checkUserSql = "SELECT PASSWORD FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, checkUserSql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Error preparing username check: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, enteredUsername.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        string storedPassword = (const char*)sqlite3_column_text(stmt, 0);
        sqlite3_finalize(stmt);

        string enteredPassword;
        cout << "Enter password: ";
        cin >> enteredPassword;

        if (bcrypt::validatePassword(enteredPassword, storedPassword)) {
            cout << "Authentication successful!" << endl;
            username = enteredUsername;
            return true;
        } else {
            cout << "Incorrect password." << endl;
            return false;
        }
    } else if (rc == SQLITE_DONE) {
        cout << "Username not found." << endl;
        sqlite3_finalize(stmt);
        return false;
    } else {
        cout << "Error executing query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
}

bool registerUser(sqlite3* db) {
    string newuser;
    cout << "Enter new_username: ";
    cin >> newuser;

    // Step 1: Check if username already exists
    string checkSql = "SELECT COUNT(*) FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing check statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, newuser.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(checkStmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(checkStmt, 0);
        if (count > 0) {
            cout << "Username already taken. Please choose a different one.\n";
            sqlite3_finalize(checkStmt);
            return false;
        }
    }
    sqlite3_finalize(checkStmt);

    // Step 2: Ask for password and confirmation
    string newpassword, confirmPassword;
    cout << "Enter new_password: ";
    cin >> newpassword;
    cout << "Confirm password: ";
    cin >> confirmPassword;

    if (newpassword != confirmPassword) {
        cout << "Passwords do not match. Registration failed.\n";
        return false;
    }

    // Step 3: Register the user
    string insertSql = "INSERT INTO PLAYERS (NAME, PASSWORD) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, insertSql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, newuser.c_str(), -1, SQLITE_STATIC);
    string hashed = bcrypt::generateHash(newpassword);
    sqlite3_bind_text(stmt, 2, hashed.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error executing insert: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    cout << "User registered successfully.\n";
    sqlite3_finalize(stmt);
    return true;
}

// Database helper functions
int getUserId(sqlite3* db, const string& username) {
    string query = "SELECT ID FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int id = -1;
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return id;
}

int insertGameHistory(sqlite3* db, int user1_id, int user2_id, const string& winner, const string& finalBoard) {
    string insertGameSql = "INSERT INTO Game_history (user1_id, user2_id, winner, board_moves) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertGameSql.c_str(), -1, &stmt, NULL);


    if (rc != SQLITE_OK) {
        cerr << " Error preparing game history insert: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, user1_id);
    sqlite3_bind_int(stmt, 2, user2_id);
    sqlite3_bind_text(stmt, 3, winner.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, finalBoard.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << " Error inserting game result: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    int game_id = (int)sqlite3_last_insert_rowid(db);
    cout << " Game history recorded." << endl;
    sqlite3_finalize(stmt);
    return game_id;
}

bool showGameHistoryForPlayer(sqlite3* db, string username) {
     int playerId=getUserId(db,username);
     if (playerId == -1) {
    cerr << "User not found.\n";
    return false;
    }
    string query =
        "SELECT gh.date_played, "
        "       u1.NAME AS Player1, "
        "       u2.NAME AS Player2, "
        "       gh.winner, "
        "       gh.board_moves "
        "FROM Game_history gh "
        "JOIN PLAYERS u1 ON gh.user1_id = u1.ID "
        "JOIN PLAYERS u2 ON gh.user2_id = u2.ID "
        "WHERE gh.user1_id = ? OR gh.user2_id = ? "
        "ORDER BY gh.date_played DESC;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        cerr << " Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, playerId); // Bind both parameters
    sqlite3_bind_int(stmt, 2, playerId);

    cout << "\nGame History with Final Boards:\n";
    cout << "-----------------------------------------------------------\n";

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        string date        = (const char*)sqlite3_column_text(stmt, 0);
        string player1Name = (const char*)sqlite3_column_text(stmt, 1);
        string player2Name = (const char*)sqlite3_column_text(stmt, 2);
        string winner      = (const char*)sqlite3_column_text(stmt, 3);
        string boardMoves  = (const char*)sqlite3_column_text(stmt, 4);

        cout << "Date     : " << date << "\n";
        cout << "Player 1 : " << player1Name << "\n";
        cout << "Player 2 : " << player2Name << "\n";
        cout << "Winner   : " << winner << "\n";
        cout << "Final Board:\n";

        // Display board in 3x3 format
        for (int i = 0; i < 9; ++i) {
            cout << (boardMoves[i] == ' ' ? '-' : boardMoves[i]) << " ";
            if (i % 3 == 2) cout << "\n";
        }
        cout << "-----------------------------------------------------------\n";
    }

    if (rc != SQLITE_DONE) {
        cerr << " Error retrieving game history: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return true;
}

void replayStoredGame(sqlite3* db, int game_id) {
    vector<Move> moves = loadMovesForGame(db, game_id);
    if (moves.empty()) {
        cout << "No moves found for game ID " << game_id << endl;
        return;
    }
    ReplayManager replay;
    replay.resetBoard();
    int currentMoveIndex = 0;
    char command;
    while (true) {
        replay.displayBoard();
        cout << "Move " << currentMoveIndex << "/" << moves.size() << "\n";
        cout << "n: next move, p: previous move, q: quit replay\n";
        cin >> command;
        if (command == 'n') {
            if (currentMoveIndex < (int)moves.size()) {
                Move move = moves[currentMoveIndex];
                replay.pushMove(move.row, move.col, move.player);
                currentMoveIndex++;
            } else {
                cout << "End of game.\n";
            }
        } else if (command == 'p') {
            if (currentMoveIndex > 0) {
                currentMoveIndex--;
                replay.undoMove();
            } else {
                cout << "Start of game.\n";
            }
        } else if (command == 'q') {
            break;
        }
    }
}

void showMovesForGame(sqlite3* db, int game_id) {
    vector<Move> moves = loadMovesForGame(db, game_id);
    if (moves.empty()) {
        cout << "No moves found for game ID " << game_id << endl;
        return;
    }
    cout << "Moves for Game ID " << game_id << ":\n";
    replayStoredGame(db, game_id);
}



int registerUserGUI(sqlite3* db , const string& username , const string& password) {

    // 0 => db err
    // 1 => username used
    // 2 => password too short
    // 3 => username too short
    // 4 => valid
    if(username.length() <= 4){
        return 3;
    }
    if (password.length() <= 6)
    {
        return 2;
    }
    


    // Step 1: Check if username already exists
    string checkSql = "SELECT COUNT(*) FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* checkStmt;
    int rc = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing check statement: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    sqlite3_bind_text(checkStmt, 1, username.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(checkStmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(checkStmt, 0);
        if (count > 0) {
            sqlite3_finalize(checkStmt);
            return 1;
        }
    }
    sqlite3_finalize(checkStmt);

    // Step 2: Register the user
    string insertSql = "INSERT INTO PLAYERS (NAME, PASSWORD) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, insertSql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    string hashed = bcrypt::generateHash(password);
    sqlite3_bind_text(stmt, 2, hashed.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error executing insert: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 4;
}

int authenticateUserGUI(sqlite3* db, const string& username , const string& password ) {

    //0 => db err
    //1 => not found
    //2 => wrong pass
    //3 => success

    string checkUserSql = "SELECT PASSWORD FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, checkUserSql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing username check: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        string storedPassword = (const char*)sqlite3_column_text(stmt, 0);
        sqlite3_finalize(stmt);

        if (bcrypt::validatePassword(password, storedPassword)) {
            return 3;
        } else {
            return 2;
        }
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 1;
    } else {
        cerr << "Error executing query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 0;
    }
}


int fetchPlayerStats(sqlite3* db, int userId, string name, int& wins , int& losses , int& ties) {
    static constexpr char const* sql = R"sql(
    SELECT
      p.NAME,
      -- count rows where winner = this player's name
      SUM(CASE WHEN gh.winner = p.NAME         THEN 1 ELSE 0 END) AS wins,
      -- count rows where winner != this player's name AND not a draw
      SUM(CASE WHEN gh.winner <> p.NAME
                 AND gh.winner <> 'Draw'      THEN 1 ELSE 0 END) AS losses,
      -- count rows where winner is the literal "Draw"
      SUM(CASE WHEN gh.winner = 'Draw'         THEN 1 ELSE 0 END) AS ties
    FROM PLAYERS AS p
    LEFT JOIN Game_history AS gh
      ON gh.user1_id = p.ID OR gh.user2_id = p.ID
    WHERE p.ID = ?1
    GROUP BY p.ID;
    )sql";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "SQLite prepare error: "
                  << sqlite3_errmsg(db) << "\n";
        return 0;
    }

    sqlite3_bind_int(stmt, 1, userId);
    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {

        wins   = sqlite3_column_int(stmt, 1);
        losses = sqlite3_column_int(stmt, 2);
        ties   = sqlite3_column_int(stmt, 3);
        sqlite3_finalize(stmt);
        return 2;
    }
    else if (rc == SQLITE_DONE) {
        std::cerr << "No user with ID=" << userId << "\n";
        sqlite3_finalize(stmt);
        return 1;
    }
    else {
        std::cerr << "SQLite step error: "
                  << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return 0;

    }

}

bool updateUsername(sqlite3* db, string& currentUsername) {
    // Step 1: Ask for current password
    string enteredPassword;
    cout << "Enter your current password to confirm identity: ";
    cin >> enteredPassword;

    // Step 2: Fetch stored hashed password
    string query = "SELECT PASSWORD FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Error preparing password fetch: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, currentUsername.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        cout << "Username not found.\n";
        sqlite3_finalize(stmt);
        return false;
    }

    string storedHashedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    // Step 3: Validate password
    if (!bcrypt::validatePassword(enteredPassword, storedHashedPassword)) {
        cout << "Incorrect password. Cannot proceed with username update.\n";
        return false;
    }

    // Step 4: Ask for new username
    string newUsername;
    cout << "Enter new username: ";
    cin >> newUsername;

    // Step 5: Check if new username already exists
    string checkSql = "SELECT COUNT(*) FROM PLAYERS WHERE NAME = ?;";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, NULL) != SQLITE_OK) {
        cerr << "Error preparing username check: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(checkStmt, 1, newUsername.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(checkStmt) == SQLITE_ROW && sqlite3_column_int(checkStmt, 0) > 0) {
        cout << "Username already taken.\n";
        sqlite3_finalize(checkStmt);
        return false;
    }
    sqlite3_finalize(checkStmt);

    // Step 6: Update username
    string updateSql = "UPDATE PLAYERS SET NAME = ? WHERE NAME = ?;";
    sqlite3_stmt* updateStmt;
    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, NULL) != SQLITE_OK) {
        cerr << "Error preparing username update: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(updateStmt, 1, newUsername.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 2, currentUsername.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(updateStmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error updating username: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(updateStmt);
        return false;
    }

    sqlite3_finalize(updateStmt);
    currentUsername = newUsername; // Update reference
    cout << "Username updated successfully.\n";
    return true;
}

bool updatePassword(sqlite3* db, const string& username) {
    string newPassword, confirmPassword;
    cout << "Enter new password: ";
    cin >> newPassword;
    cout << "Confirm new password: ";
    cin >> confirmPassword;

    if (newPassword != confirmPassword) {
        cout << "Passwords do not match.\n";
        return false;
    }

    string hashed = bcrypt::generateHash(newPassword);
    string updateSql = "UPDATE PLAYERS SET PASSWORD = ? WHERE NAME = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Error preparing password update: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, hashed.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to update password: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    cout << "Password updated successfully.\n";
    return true;
}

int updateUsernameGUI(sqlite3* db, const int & id,const string& currentUsername,const string& newUsername) {
    //0=> db err
    //1=> used
    //2=> valid
    char *errMsg = nullptr;
    int rc,out;

    // 1) Begin transaction
    rc = sqlite3_exec(db, "BEGIN;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "BEGIN failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return 0;
    }

    // 2) Update the PLAYERS table
    {
        sqlite3_stmt *st = nullptr;
        rc = sqlite3_prepare_v2(db,
            "UPDATE PLAYERS SET NAME = ?1 WHERE ID = ?2;",
            -1, &st, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Prepare PLAYERS failed: "
                      << sqlite3_errmsg(db) << "\n";
                      out = 0;
            goto rollback;
        }
        sqlite3_bind_text(st, 1, newUsername.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int (st, 2, id);
        rc = sqlite3_step(st);
        sqlite3_finalize(st);
        if (rc != SQLITE_DONE) {
            int err = sqlite3_errcode(db);
            if (err == 19) {
                cerr << "Username is already exists.\n";
                out = 1;
            }else{
                cerr << "STEP PLAYERS failed: "
                          << sqlite3_errmsg(db) << "\n";
                          out = 0;
            }
            goto rollback;
        }
    }

    // 3) Update all Game_history.winner fields matching oldName
    {
        sqlite3_stmt *st = nullptr;
        rc = sqlite3_prepare_v2(db,
            "UPDATE Game_history "
            "   SET winner = ?1 "
            " WHERE winner = ?2;",
            -1, &st, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare Game_history failed: "
                      << sqlite3_errmsg(db) << "\n";
            goto rollback;
        }
        sqlite3_bind_text(st, 1, newUsername.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(st, 2, currentUsername.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(st);
        sqlite3_finalize(st);
        if (rc != SQLITE_DONE) {
            std::cerr << "STEP Game_history failed: "
                      << sqlite3_errmsg(db) << "\n";
            out = 0;
            goto rollback;
        }
    }

    // 4) Commit on success
    rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return 0;
    }
    return 2;

rollback:
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return out;
}

int updatePasswordGUI(sqlite3* db, const int & id , const string& password) {
    //0= db err
    //1= very short
    //2= success
    if(password.length()<=4){
        return 1;
    }

    string hashed = bcrypt::generateHash(password);
    string updateSql = "UPDATE PLAYERS SET PASSWORD = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, updateSql.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Error preparing password update: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    sqlite3_bind_text(stmt, 1, hashed.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to update password: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    cout << "Password updated successfully.\n";
    return 2;
}






