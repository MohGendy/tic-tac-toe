#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentPlayersymbol('X')
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(Wgame);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_play_friend->setEnabled(false);
    ui->pushButton_play_ai->setEnabled(false);
    ui->pushButton_load_game->setEnabled(false);
    if (!database_init(db)) {
        qInfo() << "failed to init db\n";
        this->close();
    }else{
        qInfo() << "init db\n";
    }

    // Normal 3x3 grid for tic tac toe
    const int gridRows = 3;
    const int gridCols = 3;
    // Create an array of pointers to your buttons
    QPushButton* buttons[gridRows * gridCols] = { ui->b0, ui->b1, ui->b2,
                                                 ui->b3, ui->b4, ui->b5,
                                                 ui->b6, ui->b7, ui->b8 };

    // Connect each button's clicked signal in a loop
    for (int index = 0; index < gridRows * gridCols; ++index) {
            int row = index / gridCols; // calculate row (integer division)
            int col = index % gridCols; // calculate column

            // Using a lambda to capture row, col, and button pointer.
            connect(buttons[index], &QPushButton::clicked, this, [this, row, col, button = buttons[index]](){
                // Call your method that handles the move for the board.
                buttonmakemove(row, col, button);
            });
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





