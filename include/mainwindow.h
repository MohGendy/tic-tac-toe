#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "database.h"


enum windows{
    Wai=8,//
    Wchangepass=3,//
    Wchangename=2,//
    Wicon=7,//
    Wgame=13,//
    Wmodes=12,//
    Whistory=10,//
    Wlogin=5,//
    Wmain=0,//
    Wmega=11,//
    Wprofile=1,//
    Wsignup=6,//
    WmegaReplay=4,
    WnormalReplay=9,
};

struct userStruct{
    int id;         //user id
    char symbol;   //in game symbole
    string name;
};

struct gameStruct{
    int id;                   //game id from data base
    bool isAi;               //is ai game or player vs player
    bool isClassic;         //is classic game or mega
    short difficulty;      //ai difficulty level
    bool ismainuserfirst; //is main user first or second
};

//controlers
inline sqlite3* db;

inline bool logincase = 0;
inline struct userStruct users[2] = {{-1,' '},{-1,' '}};
inline struct gameStruct gamedata; //game structure

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Replay_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_back_profile_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_play_ai_clicked();

    void on_pushButton_play_friend_clicked();

    void on_pushButton_load_game_clicked();

    void on_pushButton_goto_signup_clicked();

    void on_pushButton_goto_login_clicked();

    void on_checkBox_signup_show_password_stateChanged(int arg1);

    void on_checkBox_show_password_stateChanged(int arg1);

    void on_pushButton_signup_clicked();

    void on_pushButton_play_classic_clicked();

    void on_pushButton_play_mega_clicked();

    void on_pushButton_back_mode_selection_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_login_function_clicked();

    void clearBoardGui();

    bool buttonmakemove(int row, int col ,QPushButton* button);

    void on_newpushbutton_clicked();

    void on_pushButton_change_username_clicked();

    void on_pushButton_change_password_clicked();

    void on_pushButton_4_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void loadProfile();

    void on_back_Wai_clicked();

    void on_pushButton_easy_clicked();

    void on_pushButton_medium_clicked();

    void on_pushButton_hard_clicked();

private:
    Ui::MainWindow *ui;
    char currentPlayersymbol;
};
#endif // MAINWINDOW_H
