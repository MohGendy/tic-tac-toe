#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <qdebug.h>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(Wmain);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_play_friend->setEnabled(false);
    ui->pushButton_play_ai->setEnabled(false);
    ui->pushButton_load_game->setEnabled(false);
    ui->profileWidget->setVisible(false);
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


void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}


void MainWindow::on_pushButton_login_clicked()
{
    if(users[0].id == -1){
        logincase = 0;
        this->ui->stackedWidget->setCurrentIndex(Wlogin);
    }else{
            QString text = "Login";
            ui->pushButton_login->setText(text);
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_play_friend->setEnabled(false);
            ui->pushButton_play_ai->setEnabled(false);
            ui->pushButton_load_game->setEnabled(false);
            ui->profileWidget->setVisible(false);
            QMessageBox::information(this,
                tr("Logout Success"),
                tr("See you soon "));

    }
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




void MainWindow::on_pushButton_4_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wprofile);

}


void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(ui->stackedWidget->currentWidget() == ui->profile){
        loadProfile();
    }
}

void MainWindow::loadProfile(){
    int wins , losses , ties;
    int status = fetchPlayerStats( db, users[0].id, users[0].name, wins , losses , ties);
    qInfo() << status;
    switch (status) {
    case 0: case 1:
        QMessageBox::warning(this,
                             tr("Profile Error"),
                             tr("Some error happend , please try again."));
        this->ui->stackedWidget->setCurrentIndex(Wmain);
        break;
    case 2:
        int total = wins + losses + ties;

        QString txt =
            QStringLiteral("Games Played: ") + QString::number(total) +
            "\nWins: "   + QString::number(wins)   +
            "\nLosses: " + QString::number(losses) +
            "\nDraws: "  + QString::number(ties);
        ui->label_stats->setText(txt);
        QString txt2 = QString::fromStdString(users[0].name);
        ui->label_username->setText(txt2);
        break;
    }
}

