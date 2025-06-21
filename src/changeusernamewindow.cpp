#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>


void MainWindow::on_pushButton_cancel_username_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wprofile);
}

void MainWindow::on_pushButton_save_username_clicked()
{
    QString newname = ui->enter_new_username->text();
    string newUsername = newname.toStdString();
    if(newUsername == users[0].name){
        QMessageBox::warning(this,
                        tr("Profile Error"),
                        tr("This is your username , please try again."));
    }else{
        int status = updateUsernameGUI(db, users[0].id ,users[0].name, newUsername);
        switch(status){
            case 0:
                QMessageBox::warning(this,
                            tr("Profile Error"),
                            tr("Some error happend , please try again."));
                break;
            case 1:
                QMessageBox::warning(this,
                            tr("Profile Error"),
                            tr("This username is used , please try again."));
                break;
            case 2:
                this->ui->stackedWidget->setCurrentIndex(Wprofile);
                users[0].name=newUsername;
                QMessageBox::information(this,
                    tr("Profile success"),
                    tr("Username changed successfully"));
                break;
        }
    }
}
