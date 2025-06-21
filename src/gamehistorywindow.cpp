#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>


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
            ui->tableWidget_games->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(history[i].datePlayed)));
            ui->tableWidget_games->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(history[i].opponent)));
            ui->tableWidget_games->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(history[i].result)));

        }
    }
}
