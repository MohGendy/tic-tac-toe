#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include <qlogging.h>


void MainWindow::on_pushButton_back_games_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}

void MainWindow::loadHistoryScreen(){
    vector<GameInfo> history;
    if(!fetchGamesForPlayer(db, users[0].id, history)){
        QMessageBox::critical(this, "Error", "Failed to load history");
    }else{
        ui->tableWidget_games->clearContents();
        ui->tableWidget_games->setRowCount(0);
        for(int i = 0;i<history.size();i++){
            int row = ui->tableWidget_games->rowCount();
            ui->tableWidget_games->insertRow(row);
            ui->tableWidget_games->setItem(row, 0, new QTableWidgetItem(QString::number(history[i].gameId)));
            ui->tableWidget_games->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(history[i].datePlayed)));
            ui->tableWidget_games->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(history[i].opponent)));
            ui->tableWidget_games->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(history[i].result)));

        }

        ui->tableWidget_games->setVisible(false);
        ui->tableWidget_games->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget_games->resizeColumnsToContents();
        ui->tableWidget_games->resizeRowsToContents();
        ui->tableWidget_games->setVisible(true);
    }
}



void MainWindow::on_tableWidget_games_cellClicked(int row, int column)
{
    int x = ui->tableWidget_games->item(row,0)->text().toInt();
    replayer.player2 = ui->tableWidget_games->item(row,2)->text().toStdString();
    replayer.result = ui->tableWidget_games->item(row,3)->text().toStdString();
    replayer.moves.clear();
    bool status = loadMovesForGameGUI(db,  x , replayer.moves);
    // for(int i = 0 ; i<replayer.moves.size();i++){
    //     qInfo() << replayer.moves[i].row*3+replayer.moves[i].col << " ";
    // }
    if(replayer.moves.size()<=9){
         normalReplayControl(1,1);
        this->ui->stackedWidget->setCurrentIndex(WnormalReplay);
    }else{
        megaReplayControl(1,1);
        this->ui->stackedWidget->setCurrentIndex(WmegaReplay);
    }
}

