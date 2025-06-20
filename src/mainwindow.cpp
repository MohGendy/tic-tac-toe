#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

sqlite3* db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(Wmain);
    if (!database_init(db)) {
        qInfo() << "failed to init db\n";
        this->close();
    }else{
        qInfo() << "init db\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Replay_customContextMenuRequested(const QPoint &pos)
{

}


void MainWindow::on_pushButton_back_profile_clicked()
{

}


void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_login_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wlogin);
}


void MainWindow::on_pushButton_play_ai_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wai);
}


void MainWindow::on_pushButton_play_friend_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmodes);
}




void MainWindow::on_pushButton_load_game_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Whistory);

}


void MainWindow::on_pushButton_goto_signup_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wsignup);
}


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
                                 tr("Signedup success"),
                                 tr("Now login to start play."));
            this->ui->lineEdit_signup_username->clear();
            this->ui->lineEdit_signup_password->clear();
            this->ui->lineEdit_signup_confirm_password->clear();

            this->ui->stackedWidget->setCurrentIndex(Wlogin);
            break;
        default:
            break;
        }
    }
}

