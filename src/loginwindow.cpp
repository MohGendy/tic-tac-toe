#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>


void MainWindow::on_pushButton_goto_signup_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wsignup);
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!logincase){
        this->ui->stackedWidget->setCurrentIndex(Wmain);
    }else{
        this->ui->stackedWidget->setCurrentIndex(Wmodes);
    }
}

void MainWindow::on_pushButton_login_function_clicked()
{
    QString password , username;
    username = this->ui->lineEdit_username->text();
    password = this->ui->lineEdit_password->text();
    std::string pass = password.toStdString() , name = username.toStdString();
    int status = authenticateUserGUI( db, name , pass );
    
    switch(status){
    case 0:
        QMessageBox::warning(this,
                                tr("Login Error"),
                                tr("Some error happend , please try again."));
        break;
    case 1: case 2:
        QMessageBox::warning(this,
                                tr("Login Error"),
                                tr("Wrong username or password , please try again."));
        break;
    case 3:
        int id = getUserId(db, name);
        QMessageBox::information(this,
                        tr("Login Success"),
                        (tr("Welcome , \n")+(username)));
        this->ui->lineEdit_username->clear();
        this->ui->lineEdit_password->clear();
        if(!logincase){
            users[0].id=id;
            users[0].name=name;
            QString text = "Logout";
            ui->pushButton_login->setText(text);
            ui->pushButton_4->setEnabled(true);
            ui->pushButton_play_friend->setEnabled(true);
            ui->pushButton_play_ai->setEnabled(true);
            ui->pushButton_load_game->setEnabled(true);
            ui->label_guest->setText(username);
            ui->profileWidget->setVisible(true);
            this->ui->stackedWidget->setCurrentIndex(Wmain);
        }else{
            users[1].id=id;
            users[1].name=name;
            this->ui->stackedWidget->setCurrentIndex(Wicon);
        }
    }


    
}
