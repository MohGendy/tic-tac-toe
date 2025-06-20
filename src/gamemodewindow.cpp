#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_play_classic_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wgame);
}


void MainWindow::on_pushButton_play_mega_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmega);
}


void MainWindow::on_pushButton_back_mode_selection_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}
