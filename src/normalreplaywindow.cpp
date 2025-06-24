#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "database.h"
#include <QMessageBox>

void MainWindow::on_pushButton_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(Whistory);
}

void MainWindow::normalReplayControl(bool action,bool init){
    static int i = 0;
    static QPushButton* btns[9] = {
        ui->b0_r, ui->b1_r,ui->b2_r,
        ui->b3_r,ui->b4_r,ui->b5_r,
        ui->b6_r,ui->b7_r,ui->b8_r
    };
    if(init){
        i = 0;
        for(int j = 0; j < 9 ; j++){
            btns[j]->setText(QString(" "));
        }
        ui->label_40_c->setText(QString::fromStdString(users[0].name));
        ui->label_2511->setText(QString::fromStdString(replayer.player2));
        ui->label_11->setText(QString::fromStdString(replayer.result));
    }else{
        if(action){
            if(i<replayer.moves.size()){
                btns[replayer.moves[i].row*3+replayer.moves[i].col]->setText(QString(replayer.moves[i].player));
                i++;
                ui->previouspushButton->setEnabled(true);
            }
            if(i>=replayer.moves.size()){
                ui->_r->setEnabled(false);
            }
        }else{
            if(i>0){
                i--;
                btns[replayer.moves[i].row*3+replayer.moves[i].col]->setText(QString(" "));
                ui->_r->setEnabled(true);
            }
            if(i<=0){
                ui->previouspushButton->setEnabled(false);
            }
        }
    }

}


void MainWindow::on__r_clicked()
{
    normalReplayControl(1,0);
}



void MainWindow::on_previouspushButton_clicked()
{
    normalReplayControl(0,0);
}

void MainWindow::LoadNormalReplay(){
    ui->_r->setEnabled(true);
    ui->previouspushButton->setEnabled(false);

}



