#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QPushButton>
#include "gameStructure.h"
#include <QDebug>
#include <iostream>

const QString activeStyle = "color:rgb(17, 17, 44); font-weight:bold; font-size:24px; background-color: rgba(255, 0, 255, 170);";
const QString inactiveStyle = "color:rgb(113, 113, 113); font-weight:bold; font-size:18px; background-color:rgba(85, 170, 255, 0);";
const QString btnWinStyle = "QPushButton {\n    min-width: 100px;        /* Retain original minimum width */\n    min-height: 100px;       /* Retain original minimum height */\n    font-size: 35px;         /* Retain original font size */\n    font-weight: bold;       /* Retain original font weight */\n    background-color: #1E1E4B; /* Dark blue to match background grid */\n    border: 2px solid #FF69B4; /* Neon pink border for glowing effect */\n    border-radius: 5px;      /* Slight rounding for modern look */\n    color: #FF00FF;          /* Neon pink text for X/O */\n    padding: 5px;            /* Internal padding for better appearance */\n}\n\nQPushButton:hover {\n    background-color: #2A2A5E; /* Lighter blue on hover for interactivity */\n    border: 2px solid #FF00FF; /* Brighter neon pink on hover */\n    color: #FFFFFF;          /* White text on hover for contrast */\n}\n\nQPushButton:pressed {\n    background-color: #FF69B4; /* Neon pink when pressed */\n    border: 2px solid #1E1E4B; /* Dark blue border when pressed */\n    color: #1E1E4B;          /* Dark blue text when pressed */\n}\n\n\nQPushButton:disabled {\n    background-color: rgb(51, 75, 57); /* Gray background when disabled */\n    border-color: #55ff00; /* Gray border when disabled */\n    color: #55ff00; /* Gray text when disabled */\n}";
const QString btnStyle = "";
void MainWindow::clearBoardGui(){
    // Normal 3x3 grid for tic tac toe
    const int gridRows = 3;
    const int gridCols = 3;

    // Create an array of pointers to your buttons (ensure these names match your UI object names)
    QPushButton* buttons[gridRows * gridCols] = { ui->b0, ui->b1, ui->b2,
                                                 ui->b3, ui->b4, ui->b5,
                                                 ui->b6, ui->b7, ui->b8 };

    // Loop through each button and clear its text and re-enable it
    for (int index = 0; index < gridRows * gridCols; ++index) {
        if (buttons[index]) {
            buttons[index]->setStyleSheet(btnStyle);
            buttons[index]->setText("");     // Clear the button's label
            buttons[index]->setEnabled(true);  // Re-enable the button for the next game
        }
    }

    gamedata.gameended = false ;
    if((gamedata.isAi)){
        if(ai == NULL){
            ai = new Ai(!gamedata.ismainuserfirst, gamedata.difficulty); // Create a new AI instance with the specified difficulty
        }else{
            ai->reset();
        }
    }
    // Reset the current player symbol; for example, to 'X'
    currentPlayersymbol = 'X';
    ui->user1_label->setStyleSheet(activeStyle);
    ui->user2_label->setStyleSheet(inactiveStyle);
    replayManager.resetBoard();
    board = Board(); // Reset the board state
    if(gamedata.isAi && !gamedata.ismainuserfirst){
        QTimer::singleShot(500, this, SLOT(performAimove()));
        //performAimove();
    }
    ui->newpushbutton->setEnabled(false);//disable new game button till the game ends
}

bool MainWindow::buttonmakemove(int row, int col ,QPushButton* button) {
    bool ret;
    if (!gamedata.gameended){
        ret = board.makeMove(row, col, currentPlayersymbol); // Make the move on the board
        if(gamedata.isAi && ret && currentPlayersymbol == (gamedata.ismainuserfirst ? 'X' : 'O')) {
        ai->movePlayer(row*3+col); //add player move to ai board 
        }
        ////board.displayBoard();
        //? add move to data base
        if(ret){
            // Update the UI to reflect the move
            replayManager.pushMove(row, col, currentPlayersymbol);

            button->setText(QString(currentPlayersymbol));
            button->setEnabled(false); // Disable the button after the move
            if (board.checkWin(currentPlayersymbol)) {
                winRecolor(row ,col,board.winCase(row,col));
                QMessageBox::information(this, "congrats ", QString("Player %1 wins!").arg(currentPlayersymbol));
                //? update wins in data base
                //? update win label for winner
                gamedata.gameended = true ;
                ui->newpushbutton->setEnabled(true);//enable new game button when the game ends
                //todo make tile green
                if(currentPlayersymbol=='X'){
                    ui->label_55_c->setText(QString::number(ui->label_55_c->text().toInt()+1));
                }else{
                    ui->label_54_c->setText(QString::number(ui->label_54_c->text().toInt()+1));
                }
                std::string winner = (currentPlayersymbol == users[0].symbol) ? users[0].name : (!gamedata.isAi?users[1].name:"AI");
                std::string finalBoard = replayManager.getSerializedBoard();
                int gameId = insertGameHistory(db, users[0].id, ( !gamedata.isAi?users[1].id:-1), winner, finalBoard);
                if (gameId != -1) insertGameMoves(db, gameId, replayManager.getMoves());

            }

            else if (board.isFull()) {
                QMessageBox::information(this, "Game Over", "It's a tie!");
                //?update ties in data base
                //?update ties label
                ui->label_53_c->setText(QString::number(ui->label_53_c->text().toInt()+1));
                gamedata.gameended= true ;
                ui->newpushbutton->setEnabled(true);//enable new game button when the game ends
                //todo make tile yellow
               std::string finalBoard = replayManager.getSerializedBoard();
                std::string winner = "TIE";
                int gameId = insertGameHistory(db, users[0].id, users[1].id, winner, finalBoard);
                if (gameId != -1) insertGameMoves(db, gameId, replayManager.getMoves());


            }

            // Switch players && swtich guest
            currentPlayersymbol = (currentPlayersymbol == 'X') ? 'O' : 'X';
                if (currentPlayersymbol == 'X') {
                    ui->user1_label->setStyleSheet(activeStyle);
                    ui->user2_label->setStyleSheet(inactiveStyle);
                } else { // currentPlayersymbol is 'O'
                    ui->user2_label->setStyleSheet(activeStyle);
                    ui->user1_label->setStyleSheet(inactiveStyle);
                }

            }
    }
    return ret; // Return whether the move was successful
}

void MainWindow::performAimove(){
    qDebug()<< "AI is making a move...\n";
    int move = -1;
    if(ai->moveAi(&move)){
        int row = move / 3; // Calculate row from move
        int col = move % 3; // Calculate column from move
        QPushButton* button = findChild<QPushButton*>(QString("b%1").arg(move)); // Find the button by name


    if(button){
        buttonmakemove(row, col, button);
    }
    } else {
        QMessageBox::warning(this, "AI Error", "AI could not make a valid move.");
    }
}


void MainWindow::on_newpushbutton_clicked()
{
    clearBoardGui();
}

void MainWindow::loadgameScreen(){
    
    ui->user1_label->setText(QString::fromStdString(gamedata.ismainuserfirst ?users[0].name:(!gamedata.isAi?users[1].name:"Ai")));
    ui->user2_label->setText(QString::fromStdString(gamedata.ismainuserfirst ?(!gamedata.isAi?users[1].name:"Ai"):users[0].name));
    ui->label_53_c->setText("0");
    ui->label_55_c->setText("0");
    ui->label_54_c->setText("0");
    clearBoardGui();
}

void MainWindow::on_pushButton_back_from_board_to_main_p_clicked()
{
    if(!gamedata.keep) users[1].id =-1;
    this->ui->stackedWidget->setCurrentIndex(Wmain);
    if(ai != NULL){
        delete ai;
        ai = NULL;
    }
}

void MainWindow::winRecolor(int row , int col , int winCase){
    QPushButton* buttons[3 * 3] = { ui->b0, ui->b1, ui->b2,
                                    ui->b3, ui->b4, ui->b5,
                                    ui->b6, ui->b7, ui->b8 };

    switch(winCase){
        case 1:
            buttons[row*3]->setStyleSheet(btnWinStyle);
            buttons[row*3+1]->setStyleSheet(btnWinStyle);
            buttons[row*3+2]->setStyleSheet(btnWinStyle);
            break;
        case 2:
            buttons[col]->setStyleSheet(btnWinStyle);
            buttons[col+3]->setStyleSheet(btnWinStyle);
            buttons[col+6]->setStyleSheet(btnWinStyle);
            break;
        case 3:
            buttons[0]->setStyleSheet(btnWinStyle);
            buttons[4]->setStyleSheet(btnWinStyle);
            buttons[8]->setStyleSheet(btnWinStyle);
            break;
        case 4:
            buttons[2]->setStyleSheet(btnWinStyle);
            buttons[4]->setStyleSheet(btnWinStyle);
            buttons[6]->setStyleSheet(btnWinStyle);
            break;
    }
}
