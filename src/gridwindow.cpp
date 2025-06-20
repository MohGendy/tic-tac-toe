#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QPushButton>
#include "gameStructure.h"
#include <QDebug>
#include <iostream>

struct Board board;

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

    // Reset the current player symbol; for example, to 'X'
    currentPlayersymbol = 'X';
    board = Board(); // Reset the board state
}

bool MainWindow::buttonmakemove(int row, int col ,QPushButton* button) {
    bool ret = board.makeMove(row, col, currentPlayersymbol); // Make the move on the board
    ////board.displayBoard();
    //? add move to data base
    if(ret){
        // Update the UI to reflect the move
        button->setText(QString(currentPlayersymbol));
        button->setEnabled(false); // Disable the button after the move
        if (board.checkWin(currentPlayersymbol)) {
            QMessageBox::information(this, "Game Over", QString("Player %1 wins!").arg(currentPlayersymbol));

            //todo make tile green
        }

        if (board.isFull()) {
            QMessageBox::information(this, "Game Over", "It's a tie!");
            //todo make tile yellow
        }

        // Switch players
        currentPlayersymbol = (currentPlayersymbol == 'X') ? 'O' : 'X';
        }
    return ret; // Return whether the move was successful
}


void MainWindow::on_newpushbutton_clicked()
{
    clearBoardGui();
}
