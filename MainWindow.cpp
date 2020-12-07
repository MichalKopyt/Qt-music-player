#include <iostream>
#include <QFileDialog>
#include <QtGui>
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
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefaultCover()
{
    QPixmap cover("C:/music/cover.png");
    QRect rect = ui->verticalLayout->geometry();
    ui->coverLabel->setPixmap(cover.scaled(rect.height(), rect.height()));
}


void MainWindow::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::on_position_changed(qint64 position)
{
    ui->progressSlider->setValue(position);
    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000);
    QString labelText;
    if (QString::number(seconds).length() == 1)
        labelText = QString::number(minutes) + ":0" + QString::number(seconds);
    else
        labelText = QString::number(minutes) + ":" + QString::number(seconds);
    ui->progressLabel->setText(labelText);
}

void MainWindow::on_duration_changed(qint64 position)
{
    ui->progressSlider->setMaximum(position);
    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000);
    QString labelText;
    if (QString::number(seconds).length() == 1)
        labelText = QString::number(minutes) + ":0" + QString::number(seconds);
    else
        labelText = QString::number(minutes) + ":" + QString::number(seconds);
    ui->durationLabel->setText(labelText);
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
    if (ui->playButton->text() == "Start") {
        if (player->mediaStatus() == QMediaPlayer::NoMedia)
            player->setMedia(QUrl::fromLocalFile(fileName));
        else
            player->play();
        ui->playButton->setText("Stop");
    } else if (ui->playButton->text() == "Stop") {
        player->pause();
        ui->playButton->setText("Start");
    }
}
