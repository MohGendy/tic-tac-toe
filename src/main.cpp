// // ReplayManager.cpp
// // #include "ReplayManager.hpp"
// #include "gameStructure.h"
// #include "database.h"
// #include <iostream>


// // int main() {
// //     Game game; // Create a new game instance
// //     // int choice;
// //     // cout << "enter 1 for pvp or 2 for ai vs player\n";
// //     // cin >> choice; // Get the choice from the user
// //     // switch (choice)
// //     // {
// //     // case 1:
// //     //     cout << "enter 1 for classic mode and 2 for mega mode\n";
// //     //     int mode;
// //     //     cin >> mode; // Get the mode choice from the user
// //     //     if (mode == 1) {
// //     //         cout << "Starting classic Tic Tac Toe...\n";
// //     //         game.play(); // Start the classic game
// //     //     } else if (mode == 2) {
// //     //         cout << "Starting Mega Tic Tac Toe...\n";
// //     //         game.megaPlay(); // Start the mega game
// //     //     } else {
// //     //         cout << "Invalid mode selected. Defaulting to classic mode.\n";
// //     //         game.play(); // Start the classic game
// //     //     }
// //     //     break;
// //     // case 2:
// //     //     cout << "starting AI vs Player Tic Tac Toe...\n";
// //     //     game.playAi(); //todo Start the AI vs Player game
// //     //     break;
    
// //     // default:
// //     //     cout << "Starting classic Tic Tac Toe...\n";
// //     //     game.play(); // Start the classic game
// //     //     break;
// //     // }


// //     game.play(); // Start the game

// //     return 0;
// // }
// int main() {
//     sqlite3* db;
//        if (!database_init(db)) {
//         return 1;
//     }

//     mainMenu(db);
//     sqlite3_close(db);
//     return 0;
// }
#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    qInfo() << "started";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    qInfo() << "Event loop exited, code =" << ret;
    return ret;
}
// #include <iostream>
// int main(){
//     qInfo() << "started";
// }
