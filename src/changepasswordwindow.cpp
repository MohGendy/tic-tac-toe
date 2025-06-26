#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_not_change_pass_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wprofile);

}




void MainWindow::on_checkBox_oldpass_stateChanged(int arg1)
{
    this->ui->lineEdit_old_pass->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));
}


void MainWindow::on_checkBox_newpass_stateChanged(int arg1)
{
    this->ui->lineEdit_change_pass->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));

}


void MainWindow::on_checkBox_confirm_newpass_stateChanged(int arg1)
{
    this->ui->lineEdit_new_pass->setEchoMode(arg1?(QLineEdit::Normal):(QLineEdit::Password));

}


void MainWindow::on_pushButton_change_pass_clicked()
{
    QString oldPass = ui->lineEdit_old_pass->text() , newPass = ui->lineEdit_new_pass->text()  , newPass2 = ui->lineEdit_change_pass->text();
    string oldPassStr = oldPass.toStdString() , newPassStr = newPass.toStdString() , newPass2Str = newPass2.toStdString();
    int status = authenticateUserGUI(db , users[0].name , oldPassStr);
    switch(status){
    case 0:
        QMessageBox::warning(this,
                                tr("Profile Error"),
                                tr("Some error happend , please try again."));
        break;
    case 1: case 2:
        QMessageBox::warning(this,
                                tr("Profile Error"),
                                tr("Wrong password , please try again."));
        break;
    case 3:
        if(newPassStr == newPass2Str){
            if(newPassStr == oldPassStr){
                QMessageBox::warning(this,
                    tr("Profile Error"),
                    tr("New password is the same as old one , please try again."));
            }else{

                int status2 = updatePasswordGUI( db, users[0].id , newPass2Str);
                switch(status2){
                    case 0:
                        QMessageBox::warning(this,
                            tr("Profile Error"),
                            tr("Some error happend , please try again."));
                        break;
                    case 1:
                        QMessageBox::warning(this,
                            tr("Profile Error"),
                            tr("Short passowrd , please try again."));
                        break;
                    case 2:
                        QMessageBox::information(this,
                            tr("Profile Success"),
                            tr("Password changed successfully."));
                        ui->lineEdit_old_pass->clear();
                        ui->lineEdit_new_pass->clear(); 
                        ui->lineEdit_change_pass->clear();
                        ui->stackedWidget->setCurrentIndex(Wprofile);
    
                        break;
                }
            }
        }else{
            QMessageBox::warning(this,
                tr("Profile Error"),
                tr("Passwords do not match , please try again."));
        }
    }
}

