#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_play_classic_clicked()
{   
    gamedata.isClassic = true; //set game mode to classic
    gamedata.isAi = false; 
    logincase = 1; //to log in the second user
    if(users[1].id != -1 && QMessageBox::Ok == QMessageBox::question(
        this,QString("Login"),QString::fromStdString("user "+users[1].name+" is logged in as 2nd player,\nDo you want to stay logged in?"),
        QMessageBox::Ok|QMessageBox::Cancel)
    ){
        this->ui->stackedWidget->setCurrentIndex(Wicon);
    }else{
        this->ui->stackedWidget->setCurrentIndex(Wlogin);
    }
}


void MainWindow::on_pushButton_play_mega_clicked()
{
    gamedata.isClassic = false; //set game mode to mega
    gamedata.isAi = false;
    logincase = 1; //to log in the second user
    if(users[1].id != -1 && QMessageBox::Ok == QMessageBox::question(
        this,QString("Login"),QString::fromStdString("user " +users[1].name+" is logged in as 2nd player,\nDo you want to stay logged in?"),
        QMessageBox::Ok|QMessageBox::Cancel)
    ){
        this->ui->stackedWidget->setCurrentIndex(Wicon);
    }else{
        this->ui->stackedWidget->setCurrentIndex(Wlogin);
    }
}


void MainWindow::on_pushButton_back_mode_selection_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}
