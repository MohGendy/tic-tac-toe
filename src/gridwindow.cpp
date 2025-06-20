#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QPushButton>
#include "gameStructure.h"
#include <QDebug>
#include <iostream>

const QString activeStyle = "color:white; font-weight:bold; font-size:24px; background-color:rgba(85, 170, 255, 100);";
const QString inactiveStyle = "color:rgb(113, 113, 113); font-weight:bold; font-size:18px; background-color:rgba(85, 170, 255, 0);";

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
            buttons[index]->setText("");     // Clear the button's label
            buttons[index]->setEnabled(true);  // Re-enable the button for the next game
        }
    }

    gamedata.gameended = false ;
    if((gamedata.isAi)){
        if(ai != NULL){
            delete ai; // Clean up previous AI instance if it exists
        }
        ai = new Ai(!gamedata.ismainuserfirst, gamedata.difficulty); // Create a new AI instance with the specified difficulty
    }
    // Reset the current player symbol; for example, to 'X'
    currentPlayersymbol = 'X';
    ui->user1_label->setStyleSheet(activeStyle);
    ui->user2_label->setStyleSheet(inactiveStyle);
    board = Board(); // Reset the board state
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
            button->setText(QString(currentPlayersymbol));
            button->setEnabled(false); // Disable the button after the move
            if (board.checkWin(currentPlayersymbol)) {
                QMessageBox::information(this, "congrats 🥳", QString("Player %1 wins!").arg(currentPlayersymbol));
                //? update wins in data base
                //? update win label for winner
                gamedata.gameended = true ;
                ui->newpushbutton->setEnabled(true);//enable new game button when the game ends
                //todo make tile green
            }

            else if (board.isFull()) {
                QMessageBox::information(this, "Game Over", "It's a tie!");
                //?update ties in data base
                //?update ties label
                gamedata.gameended= true ;
                ui->newpushbutton->setEnabled(true);//enable new game button when the game ends
                //todo make tile yellow
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
    qDebug()<<"load game screen started ";
    ui->newpushbutton->setEnabled(false);//disable new game button till the game ends
    // //! just for testing
    // gamedata.isAi = false; // Set to true for AI game
    // gamedata.ismainuserfirst = true; // Set to true if the main user is first
    // gamedata.difficulty = normal; // Set difficulty level (0: easy, 1: medium, 2: hard)
    // //!end of testing parameters
    clearBoardGui();

    //? add guest names /users
}


void MainWindow::on_pushButton_back_from_board_to_main_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}
