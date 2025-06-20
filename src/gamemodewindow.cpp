#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_play_classic_clicked()
{   
    gamedata.isClassic = true; //set game mode to classic
    logincase = 1; //to log in the second user
    this->ui->stackedWidget->setCurrentIndex(Wlogin);
}


void MainWindow::on_pushButton_play_mega_clicked()
{
    gamedata.isClassic = false; //set game mode to mega
    logincase = 1; //to log in the second user
    this->ui->stackedWidget->setCurrentIndex(Wlogin);
}


void MainWindow::on_pushButton_back_mode_selection_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}
