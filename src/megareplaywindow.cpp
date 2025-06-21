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
        ui->b0_r, ui->b1_r,ui->b2_r,
        ui->b3_r,ui->b4_r,ui->b5_r,
        ui->b6_r,ui->b7_r,ui->b8_r
    };
    if(init){
        i = 0;
        for(int j = 0; j < 9 ; j++){
            btns[j]->setText(QString(" "));
        }
        // !!!!!!!!!!!!!!!!!!11
        ui->label_40_m->setText(QString::fromStdString(users[0].name));
        ui->label_1125->setText(QString::fromStdString(replayer.player2));
        ui->label_10->setText(QString::fromStdString(replayer.result));
    }else{
        if(action){
            if(i<replayer.moves.size()){
                btns[replayer.moves[i].row*3+replayer.moves[i].col]->setText(QString(replayer.moves[i].player));
                i++;
            }
        }else{
            if(i>0){
                i--;
                btns[replayer.moves[i].row*3+replayer.moves[i].col]->setText(QString(" "));
            }
        }
    }

}



void MainWindow::on_previouspushbuttonmega_clicked()
{
    megaReplayControl(1,0);
}


void MainWindow::on_pushButton_back_from_board_to_main_mega_r_clicked()
{
    megaReplayControl(0,0);

}




