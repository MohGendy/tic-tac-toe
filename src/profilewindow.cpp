#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>



void MainWindow::on_pushButton_back_profile_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}


void MainWindow::on_pushButton_change_username_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wchangename);
}

void MainWindow::on_pushButton_change_password_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wchangepass);

}