#include <iostream>
#include <QFileDialog>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Song.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::on_media_loaded);

    model = new SongsModel(QSize(0, 0), this);
    //model->setData();



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

//void on_startButton_clicked()
//{
//    player->setMedia(QUrl::fromLocalFile(fileName));
//}

//void on_stopButton_clicked()
//{
//    player->stop();
//}

void MainWindow::on_position_changed(qint64 position)
{
    ui->progressSlider->setValue(position);
}

void MainWindow::on_duration_changed(qint64 position)
{
    ui->progressSlider->setMaximum(position);
}

void MainWindow::on_action_Open_file_triggered()
{
    fileName = QFileDialog::getOpenFileName(this, "Open mp3 file", "C://");
    std::cout << fileName.toStdString() << std::endl;
}

void MainWindow::on_media_loaded()
{
    if (player->mediaStatus() == QMediaPlayer::LoadedMedia){
        player->play();
    }
    Song song;
}

void MainWindow::on_playButton_clicked()
{
    if (fileName.isNull())
        return;
    QObject* senderObj = sender();
    if (senderObj->isWidgetType()){
        QPushButton* button = qobject_cast<QPushButton*>(senderObj);
        if (button) {
            if (button->text() == "Start") {
                if (player->mediaStatus() == QMediaPlayer::NoMedia)
                    player->setMedia(QUrl::fromLocalFile(fileName));
                else
                    player->play();
                button->setText("Stop");
            } else if (button->text() == "Stop") {
                player->pause();
                button->setText("Start");
            }
        }
    }
}
