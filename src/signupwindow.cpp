#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_goto_login_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wlogin);
}


void MainWindow::on_checkBox_signup_show_password_stateChanged(int arg1)
{
    this->ui->lineEdit_signup_password->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));
    this->ui->lineEdit_signup_confirm_password->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));
}


void MainWindow::on_checkBox_show_password_stateChanged(int arg1)
{
    this->ui->lineEdit_password->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));
}


void MainWindow::on_pushButton_signup_clicked()
{
    QString password , username , password2;
    username = this->ui->lineEdit_signup_username->text();
    password = this->ui->lineEdit_signup_password->text();
    password2 = this->ui->lineEdit_signup_confirm_password->text();
    std::string pass = password.toStdString() , name = username.toStdString() , pass2 = password2.toStdString();

    if(pass != pass2 ){
        QMessageBox::warning(this,
                             tr("Signup Error"),
                             tr("The confirm password must match the password."));
    }else{
        int status = registerUserGUI(db,name,pass);
        switch (status) {
        case 0:
            QMessageBox::warning(this,
                                 tr("Signup Error"),
                                 tr("Some error happend , please try again."));
            break;
        case 1:
            QMessageBox::warning(this,
                                 tr("Signup Error"),
                                 tr("This username is used , please try again."));
            break;
        case 2:
            QMessageBox::warning(this,
                                 tr("Signup Error"),
                                 tr("Short passowrd , please try again."));

            break;
        case 3:
            QMessageBox::warning(this,
                                 tr("Signup Error"),
                                 tr("Short username , please try again."));

            break;
        case 4:
            QMessageBox::information(this,
                                 tr("Signup Success"),
                                 tr("Now login to start play."));
            this->ui->lineEdit_signup_username->clear();
            this->ui->lineEdit_signup_password->clear();
            this->ui->lineEdit_signup_confirm_password->clear();
            this->ui->checkBox_signup_show_password->setChecked(false); //!added this

            this->ui->stackedWidget->setCurrentIndex(Wlogin);
            break;
        default:
            break;
        }
    }
}

