#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>
#include <qlogging.h>

void MainWindow::on_pushButton_back_player2_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmain);
}


void MainWindow::on_pushButton_start_game_clicked()
{   
    QString icon = this->ui->comboBox_iconChoice->currentText();
    if(icon.toStdString()[0]=='O'){
        qInfo() << "first\n";
        users[0].symbol='O';
        users[1].symbol='X';
        gamedata.ismainuserfirst = false;
    }else{
        users[0].symbol='X';
        users[1].symbol='O';
        gamedata.ismainuserfirst = true;
    }
    if(gamedata.isClassic || gamedata.isAi){
        this->ui->stackedWidget->setCurrentIndex(Wgame);
    }else{
        this->ui->stackedWidget->setCurrentIndex(Wmega);
    }
}

void MainWindow::loadchooseicon(){
    QString username = QString::fromStdString(users[0].name);
    this->ui->label_player2_title->setWordWrap(true);
    this->ui->label_player2_title->setText(QString("Ready, %1?\nPick your icon").arg(username));
    qDebug() << "Username: " << QString::fromStdString(users[0].name);
}


