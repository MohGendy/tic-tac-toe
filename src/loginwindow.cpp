#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>


void MainWindow::on_pushButton_goto_signup_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wsignup);
}
