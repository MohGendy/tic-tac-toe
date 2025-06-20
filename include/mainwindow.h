#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

enum windows{
    Wai=7,//
    Wchangepass=3,//
    Wchangename=2,//
    Wicon=6,//
    Wgame=11,//
    Wmodes=10,//
    Whistory=8,//
    Wlogin=4,//
    Wmain=0,//
    Wmega=9,//
    Wprofile=1,//
    Wsignup=5//
};

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
