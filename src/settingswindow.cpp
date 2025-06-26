#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>




void MainWindow::on_back_main_settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(Wmain);
}

void MainWindow::loadbgMusic(){
    player.setAudioOutput(&audioOutput);
    player.setSource(QUrl("qrc:/sound/sound/sound.mp3"));
    audioOutput.setVolume(0.5);
    player.play();
    connect(&player,&QMediaPlayer::mediaStatusChanged,this,[this](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::EndOfMedia){
            player.setPosition(0);
            player.play();
        }
    });
}


void MainWindow::on_music_selection_currentIndexChanged(int index)
{
    switch(index){
        case 0:
            player.setSource(QUrl("qrc:/sound/sound/sound.mp3"));
            break;
        case 1:
            player.setSource(QUrl("qrc:/sound/sound/rain.mp3"));
            break;
    }
    player.setPosition(0);
    player.play();

}


void MainWindow::on_sound_level_sliderMoved(int position)
{
    audioOutput.setVolume(position/100.0);
}


