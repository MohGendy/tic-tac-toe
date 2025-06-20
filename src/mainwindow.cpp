#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"

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

