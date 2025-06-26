#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>



void MainWindow::on_pushButton_2_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Whistory);
}

void MainWindow::megaReplayControl(bool action,bool init){
    static int i = 0;
    static QPushButton* btns[9*9] = { //!!!!!!!!!!!!!!!11
        ui->b0_0_r, ui->b0_1_r, ui->b0_2_r,
        ui->b1_0_r, ui->b1_1_r, ui->b1_2_r,
        ui->b2_0_r, ui->b2_1_r, ui->b2_2_r,

        ui->b0_3_r, ui->b0_4_r, ui->b0_5_r,
        ui->b1_3_r, ui->b1_4_r, ui->b1_5_r,
        ui->b2_3_r, ui->b2_4_r, ui->b2_5_r,

        ui->b0_6_r, ui->b0_7_r, ui->b0_8_r,
        ui->b1_6_r, ui->b1_7_r, ui->b1_8_r,
        ui->b2_6_r, ui->b2_7_r, ui->b2_8_r,

        ui->b3_0_r, ui->b3_1_r, ui->b3_2_r,
        ui->b4_0_r, ui->b4_1_r, ui->b4_2_r,
        ui->b5_0_r, ui->b5_1_r, ui->b5_2_r,

        ui->b3_3_r, ui->b3_4_r, ui->b3_5_r,
        ui->b4_3_r, ui->b4_4_r, ui->b4_5_r,
        ui->b5_3_r, ui->b5_4_r, ui->b5_5_r,

        ui->b3_6_r, ui->b3_7_r, ui->b3_8_r,
        ui->b4_6_r, ui->b4_7_r, ui->b4_8_r,
        ui->b5_6_r, ui->b5_7_r, ui->b5_8_r,

        ui->b6_0_r, ui->b6_1_r, ui->b6_2_r,
        ui->b7_0_r, ui->b7_1_r, ui->b7_2_r,
        ui->b8_0_r, ui->b8_1_r, ui->b8_2_r,

        ui->b6_3_r, ui->b6_4_r, ui->b6_5_r,
        ui->b7_3_r, ui->b7_4_r, ui->b7_5_r,
        ui->b8_3_r, ui->b8_4_r, ui->b8_5_r,

        ui->b6_6_r, ui->b6_7_r, ui->b6_8_r,
        ui->b7_6_r, ui->b7_7_r, ui->b7_8_r,
        ui->b8_6_r, ui->b8_7_r, ui->b8_8_r
    };
    if(init){
        i = 0;
        for(int j = 0; j < 9*9 ; j++){
            btns[j]->setText(QString(" "));
        }
        // !!!!!!!!!!!!!!!!!!11
        ui->label_40_m->setText(QString::fromStdString(users[0].name));
        ui->label_1125->setText(QString::fromStdString(replayer.player2));
        ui->label_10->setText(QString::fromStdString(replayer.result));
        ui->play_mega->setText(QString("Play"));
        replayer.timer.stop();
        replayer.timer.disconnect();
        connect(&(replayer.timer),&QTimer::timeout,this,[this](){
                timerCallbackMega();
        });

    }else{
        if(action){
            if(i<replayer.moves.size()){
                btns[replayer.moves[i].row+replayer.moves[i].col*9]->setText(QString(replayer.moves[i].player));
                i++;
                ui->previouspushbuttonmega->setEnabled(true);
            }
            if(i>=replayer.moves.size()){
                ui->pushButton_back_from_board_to_main_mega_r->setEnabled(false);
                replayer.timer.stop();
                ui->play_mega->setText(QString("Play"));
            }
        }else{
            if(i>0){
                i--;
                btns[replayer.moves[i].row+replayer.moves[i].col*9]->setText(QString(" "));
                ui->pushButton_back_from_board_to_main_mega_r->setEnabled(true);
            }
            if(i<=0){
                ui->previouspushbuttonmega->setEnabled(false);
            }
        }
    }

}



void MainWindow::on_previouspushbuttonmega_clicked()
{
    megaReplayControl(0,0);
}


void MainWindow::on_pushButton_back_from_board_to_main_mega_r_clicked()
{
    megaReplayControl(1,0);

}

void MainWindow::LoadMegaReplay(){
    ui->pushButton_back_from_board_to_main_mega_r->setEnabled(true);
    ui->previouspushbuttonmega->setEnabled(false);
}

void MainWindow::timerCallbackMega(){
    megaReplayControl(1,0);
}

void MainWindow::on_play_mega_clicked()
{
    bool isStarted = ui->play_mega->text() == QString("Stop");
    if(!isStarted){
        replayer.timer.start(1000);
        ui->play_mega->setText(QString("Stop"));
    }else{
        replayer.timer.stop();
        ui->play_mega->setText(QString("Play"));
    }
}

