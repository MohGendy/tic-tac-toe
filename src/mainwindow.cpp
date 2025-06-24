#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <qdebug.h>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentPlayersymbol('X')
{
    ui->setupUi(this);
    this->setFixedSize(900, 700);
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
            if(!gamedata.isAi) {buttonmakemove(row, col, button);}
            else if (currentPlayersymbol == (gamedata.ismainuserfirst ? 'X' : 'O') && buttonmakemove(row, col, button)) {
                // After a successful human move,
                // switch players and check if it's AI’s turn.
                // For instance, if the main user is first, then AI is 'O'
                if (gamedata.isAi && currentPlayersymbol == (gamedata.ismainuserfirst ? 'O' : 'X') && !gamedata.gameended) {
                    // Use a timer for a short delay (500ms) before letting the AI move.
                    QTimer::singleShot(500, this, SLOT(performAimove()));
                }
            }});
    }

    //mega grid callback connections
    for (int megaRow = 0; megaRow < 3; ++megaRow) {
        for (int subRow = 0; subRow < 3; ++subRow) {
            for (int megaCol = 0; megaCol < 3; ++megaCol) {
                for (int subCol = 0; subCol < 3; ++subCol) {
                    QString objectName = QString("b%1_%2_%3")
                                            .arg(subRow)
                                            .arg(subCol)
                                            .arg(megaRow * 3 + megaCol); // subgrid ID

                    QPushButton *button = this->findChild<QPushButton*>(objectName, Qt::FindChildrenRecursively);
                    //QPushButton *button = findChild<QPushButton*>(objectName);
                    if (!button) {
                        qDebug() << "Button not found:" << objectName;
                        continue;
                    }

                    if (button) {
                        int subgrid = megaRow * 3 + megaCol; // Calculate the subgrid ID
                        connect(button, &QPushButton::clicked, this, [this, subRow, subCol,subgrid, button]() {
                            handleMegaMove(subRow, subCol, subgrid, button);
                        });
                    }
                }
            }
        }
    }

}



MainWindow::~MainWindow()
{
    delete ui;
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
            users[0].id = -1;
            QMessageBox::information(this,
                tr("Logout Success"),
                tr("See you soon "));

    }
}


void MainWindow::on_pushButton_play_ai_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wai);
    gamedata.isAi = true;
}


void MainWindow::on_pushButton_play_friend_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Wmodes);
    gamedata.isAi = false;
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
    if(ui->stackedWidget->currentWidget() == ui->gameScreen){
        loadgameScreen();
    }
    if(ui->stackedWidget->currentWidget() == ui->mega_tic_tac_toe){
        loadmegaWindow();
    }

    if(ui->stackedWidget->currentWidget() == ui->games){
        loadHistoryScreen();
    }

    if(ui->stackedWidget->currentWidget() == ui->choose_your_icon){
        loadchooseicon();
    }
    
    if(ui->stackedWidget->currentWidget() == ui->replay_classic){
        LoadNormalReplay();
    }
    
    if(ui->stackedWidget->currentWidget() == ui->replay_mega){
        LoadMegaReplay();
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
        this->ui->label_username->setText(QString::fromStdString(users[0].name));
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



