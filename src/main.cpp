// ReplayManager.cpp
// #include "ReplayManager.hpp"
#include "gameStructure.h"
#include "database.h"
#include <iostream>


// int main() {
//     Game game; // Create a new game instance
//     // int choice;
//     // cout << "enter 1 for pvp or 2 for ai vs player\n";
//     // cin >> choice; // Get the choice from the user
//     // switch (choice)
//     // {
//     // case 1:
//     //     cout << "enter 1 for classic mode and 2 for mega mode\n";
//     //     int mode;
//     //     cin >> mode; // Get the mode choice from the user
//     //     if (mode == 1) {
//     //         cout << "Starting classic Tic Tac Toe...\n";
//     //         game.play(); // Start the classic game
//     //     } else if (mode == 2) {
//     //         cout << "Starting Mega Tic Tac Toe...\n";
//     //         game.megaPlay(); // Start the mega game
//     //     } else {
//     //         cout << "Invalid mode selected. Defaulting to classic mode.\n";
//     //         game.play(); // Start the classic game
//     //     }
//     //     break;
//     // case 2:
//     //     cout << "starting AI vs Player Tic Tac Toe...\n";
//     //     game.playAi(); //todo Start the AI vs Player game
//     //     break;
    
//     // default:
//     //     cout << "Starting classic Tic Tac Toe...\n";
//     //     game.play(); // Start the classic game
//     //     break;
//     // }


//     game.play(); // Start the game

//     return 0;
// }

int main() {
    sqlite3* db;
    int rc = sqlite3_open("tictac.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    // Enable foreign keys
    char* message_error;
    rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, 0, &message_error);
    if (rc != SQLITE_OK) {
        cerr << "Error enabling foreign keys: " << message_error << endl;
        sqlite3_free(message_error);
        sqlite3_close(db);
        return 1;
    }

    // Create tables if needed
    string sql_players = "CREATE TABLE IF NOT EXISTS PLAYERS("
                         "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "NAME TEXT UNIQUE NOT NULL,"
                         "PASSWORD TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql_players.c_str(), NULL, 0, &message_error);
    if (rc != SQLITE_OK) {
        cerr << "Error creating PLAYERS table: " << message_error << endl;
        sqlite3_free(message_error);
        sqlite3_close(db);
        return 1;
    }

    string sql_game_history = "CREATE TABLE IF NOT EXISTS Game_history ("
                              "ID INTEGER PRIMARY KEY AUTOINCREMENT,""user1_id INTEGER,"
                              "user2_id INTEGER,"
                              "winner TEXT,"
                              "date_played TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                              "FOREIGN KEY (user1_id) REFERENCES PLAYERS(ID),"
                              "FOREIGN KEY (user2_id) REFERENCES PLAYERS(ID)"
                              ");";
    rc = sqlite3_exec(db, sql_game_history.c_str(), NULL, 0, &message_error);
    if (rc != SQLITE_OK) {
        cerr << "Error creating Game_history table: " << message_error << endl;
        sqlite3_free(message_error);
        sqlite3_close(db);
        return 1;
    }
    // Check if 'board_moves' column exists
    bool columnExists = false;
    sqlite3_stmt* stmt;
    string checkColSql = "PRAGMA table_info(Game_history);";
    if (sqlite3_prepare_v2(db, checkColSql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (colName == "board_moves") {
                columnExists = true;
                break;
            }
        }
    }
    sqlite3_finalize(stmt);

    // Add board_moves column only if it does not exist
    if (!columnExists) {
        string alterSql = "ALTER TABLE Game_history ADD COLUMN board_moves TEXT;";
        int rc1 = sqlite3_exec(db, alterSql.c_str(), NULL, 0, &message_error);
        if (rc1 != SQLITE_OK) {
            cerr << "Error adding board_moves column: " << message_error << endl;
            sqlite3_free(message_error);
        }
    }
        string sql_game_moves = "CREATE TABLE IF NOT EXISTS Game_moves ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "game_id INTEGER NOT NULL,"
                            "move_number INTEGER NOT NULL,"
                            "row INTEGER NOT NULL,"
                            "col INTEGER NOT NULL,"
                            "player CHAR(1) NOT NULL,"
                            "FOREIGN KEY (game_id) REFERENCES Game_history(ID) "
                            ");";
    rc = sqlite3_exec(db, sql_game_moves.c_str(), NULL, 0, &message_error);
    if (rc != SQLITE_OK) {
        cerr << "Error creating Game_moves table: " << message_error << endl;
        sqlite3_free(message_error);
        sqlite3_close(db);
        return 1;
    }

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

    sqlite3_close(db);
    return 0;
}