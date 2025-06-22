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
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}


