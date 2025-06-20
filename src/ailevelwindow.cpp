#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include "ai.h"

void MainWindow::on_back_Wai_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}


void MainWindow::on_pushButton_easy_clicked()
{
    gamedata.difficulty=easy;
    this->ui->stackedWidget->setCurrentIndex(Wicon);
}


void MainWindow::on_pushButton_medium_clicked()
{
    gamedata.difficulty=normal;
    this->ui->stackedWidget->setCurrentIndex(Wicon);
}


void MainWindow::on_pushButton_hard_clicked()
{
    gamedata.difficulty=hard;
    this->ui->stackedWidget->setCurrentIndex(Wicon);
}
