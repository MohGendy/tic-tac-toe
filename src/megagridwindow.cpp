#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

const QString activeStyle = "color:rgb(17, 17, 44); font-weight:bold; font-size:24px; background-color: rgba(255, 0, 255, 170);";
const QString inactiveStyle = "color:rgb(113, 113, 113); font-weight:bold; font-size:18px; background-color:rgba(85, 170, 255, 0);";


bool MainWindow::handleMegaMove(int row, int col, int subgrid , QPushButton* button) {
    qDebug()<<"handle mega move started \n";
    int closed = -1;
    if(!gamedata.gameended){
        if(! megaboard.isGridwon(subgrid) ){
            if(allowedSubgrid == -1 ||subgrid == allowedSubgrid  ||  
                                    (megaboard.isSubgridFull(allowedSubgrid) ) ||
                                        (megaboard.isGridwon(allowedSubgrid))){
                if(megaboard.megaMakeMove(row, col, subgrid , currentPlayersymbol)){
                    int r_row = row*3+col;
                    replayManager.pushMove(r_row,subgrid,currentPlayersymbol);
                    button->setText(QString(QChar(currentPlayersymbol)));
                    button->setEnabled(false);

                    if(megaboard.checkSubgridWin(subgrid,currentPlayersymbol)){
                        QMessageBox::information(this, "congrats ", QString("Player %1 wins subgrid %2 !").arg(currentPlayersymbol).arg(subgrid));
                        if(currentPlayersymbol=='X') ui->label_55_m->setText(QString::number(ui->label_55_m->text().toInt()+1));
                        else ui->label_54_m->setText(QString::number(ui->label_54_m->text().toInt()+1));
                        //? mark gui subgrid as won with winning char
                        for(int irow = 0; irow < 3 ;irow++){
                            for(int icol = 0 ; icol < 3 ;icol++){
                                QString objectName = QString("b%1_%2_%3").arg(irow).arg(icol).arg(subgrid);
                                QPushButton *ibutton = this->findChild<QPushButton*>(objectName, Qt::FindChildrenRecursively);
                                if (ibutton) {
                                    ibutton->setEnabled(false);
                            }
                        }
                        }
                        closed = subgrid;
                    }

                    if( megaboard.megaCheckWin(currentPlayersymbol)){
                        QMessageBox::information(this, "congrats ", QString("Player %1 wins the game!").arg(currentPlayersymbol));
                        std::string winner = (currentPlayersymbol == users[0].symbol) ? users[0].name : users[1].name;
                        std::string finalBoard = replayManager.getSerializedBoard();
                        int gameId = insertGameHistory(db, users[0].id, users[1].id, winner, finalBoard);
                        if (gameId != -1) insertGameMoves(db, gameId, replayManager.getMoves());
                        gamedata.gameended = true ;
                        ui->newmegapushbutton->setEnabled(true);//enable new game
                    }

                    if (megaboard.megaIsFull()||megaboard.iswinningGridsFull()) {  // Check for a tie
                        QMessageBox::information(this, "Game Over", "It's a tie!");
                        gamedata.gameended = true ;
                        ui->newmegapushbutton->setEnabled(true);//enable new game
                        std::string winner = "TIE";
                        std::string finalBoard = replayManager.getSerializedBoard();
                        int gameId = insertGameHistory(db, users[0].id, users[1].id, winner, finalBoard);
                        if (gameId != -1) insertGameMoves(db, gameId, replayManager.getMoves());
                    }

                    if(megaboard.isSubgridFull(subgrid)){
                         ui->label_53_m->setText(QString::number(ui->label_53_m->text().toInt()+1));
                         closed = subgrid;
                    }

                    currentPlayersymbol = (currentPlayersymbol == 'X') ? 'O' : 'X';
                    if (currentPlayersymbol == 'X') {
                        ui->user1_label_mega->setStyleSheet(activeStyle);
                        ui->user2_label_mega->setStyleSheet(inactiveStyle);
                    } else { // currentPlayersymbol is 'O'
                        ui->user2_label_mega->setStyleSheet(activeStyle);
                        ui->user1_label_mega->setStyleSheet(inactiveStyle);
                    }
                    allowedSubgrid = row * 3 + col; //next player allowed subgrid
                    disableEnableControler( 0 ,closed);
                }

            else {
                QMessageBox::warning(this, "Invalid Move", QString("That spot is already taken. Try again with an empty cell."));
                return false; // Indicate that the move was not handled successfully
            }
        }
        else {
            QMessageBox::warning(this, "Invalid Move", QString("Invalid move. You must play in subgrid %1").arg(allowedSubgrid));
            return false; // Indicate that the move was not handled successfully}
        }

        return true; // Indicate that the move was handled successfully
        }
    }
    return false; // Indicate that the game ended
}



void MainWindow::clearmegawindow(){
    //enable all buttons
    for(int isubgrid = 0; isubgrid < 9; isubgrid++){
        for(int irow = 0; irow < 3 ;irow++){
            for(int icol = 0 ; icol < 3 ;icol++){
                QString objectName = QString("b%1_%2_%3").arg(irow).arg(icol).arg(isubgrid);
                QPushButton *ibutton = this->findChild<QPushButton*>(objectName, Qt::FindChildrenRecursively);
                if (ibutton) {
                    ibutton->setEnabled(true);
                    ibutton->setText("");
                }
            }
        }
        ui->label_53_m->setText("0");
        ui->label_55_m->setText("0");
        ui->label_54_m->setText("0");
    }
    //disable new button
    ui->newmegapushbutton->setEnabled(false);
    gamedata.gameended = false ;
    currentPlayersymbol = 'X';
    allowedSubgrid = -1 ;
    megaboard = megaBoard(); // Reset the mega board
    //reset lables (active/inactive)
    disableEnableControler( 1 ,-1);
    ui->user1_label_mega->setStyleSheet(activeStyle);
    ui->user2_label_mega->setStyleSheet(inactiveStyle);

    replayManager.resetBoard();
}

void MainWindow::on_exit_mega_clicked()
{
    ui->stackedWidget->setCurrentIndex(Wmain);
}

void MainWindow::on_newmegapushbutton_clicked()
{
    clearmegawindow();
}

void MainWindow::loadmegaWindow(){
    clearmegawindow();
    ui->user1_label_mega->setText(QString::fromStdString(gamedata.ismainuserfirst ?users[0].name:users[1].name));
    ui->user2_label_mega->setText(QString::fromStdString(gamedata.ismainuserfirst ?users[1].name:users[0].name));
}

void MainWindow::disableEnableControler(bool init ,int closed ){
    static bool sub[9] = {0};
    if(closed != -1){
        sub[closed] = 1;
    }
    if(init){
        for(int isubgrid = 0; isubgrid < 9; isubgrid++){
            sub[isubgrid] = 0;
        }
    }else{
        for(int isubgrid = 0; isubgrid < 9; isubgrid++){
            for(int irow = 0; irow < 3 ;irow++){
                for(int icol = 0 ; icol < 3 ;icol++){
                    QString objectName = QString("b%1_%2_%3").arg(irow).arg(icol).arg(isubgrid);
                    QPushButton *ibutton = this->findChild<QPushButton*>(objectName, Qt::FindChildrenRecursively);
                    if (ibutton && !sub[isubgrid] && ibutton->text().toStdString() != "X" && ibutton->text().toStdString() != "O" && (allowedSubgrid == -1 || isubgrid == allowedSubgrid || sub[allowedSubgrid])) {
                        ibutton->setEnabled(true);
                    }else{
                        ibutton->setEnabled(false);
                    }
                }
            }
        }
    }
}