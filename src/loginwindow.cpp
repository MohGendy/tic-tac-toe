#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>


void MainWindow::on_pushButton_goto_signup_clicked() // go to signup btn
{
    this->ui->stackedWidget->setCurrentIndex(Wsignup);
    this->ui->lineEdit_username->clear();
    this->ui->lineEdit_password->clear();
    this->ui->checkBox_show_password->setChecked(false); //!added this
}

void MainWindow::on_pushButton_3_clicked() // back btn
{
    this->ui->lineEdit_username->clear();
    this->ui->lineEdit_password->clear();
    this->ui->checkBox_show_password->setChecked(false);  //!added this
    if(!logincase){
        this->ui->stackedWidget->setCurrentIndex(Wmain);
    }else{
        this->ui->stackedWidget->setCurrentIndex(Wmodes);
    }
}

void MainWindow::on_pushButton_login_function_clicked() //login btn
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
        if(logincase == 1 && name == users[0].name){
                QMessageBox::warning(this,
                tr("Login Error"),
                tr("You are already logged in , please logout first."));
        }else{

            int id = getUserId(db, name);
            this->ui->lineEdit_username->clear();
            this->ui->lineEdit_password->clear();
            this->ui->checkBox_show_password->setChecked(false);
            if(!logincase){
                QMessageBox::information(this,
                                tr("Login Success"),
                                (tr("Welcome , \n")+(username)));
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
                if(QMessageBox::Ok == QMessageBox::question(
                    this,tr("Login Success"),tr("Do you want to stay logged in?"),
                    QMessageBox::Ok|QMessageBox::Cancel) 
                ) gamedata.keep = 1;
                else gamedata.keep = 0;
                this->ui->stackedWidget->setCurrentIndex(Wicon);
                
            }
        }
    }


    
}
