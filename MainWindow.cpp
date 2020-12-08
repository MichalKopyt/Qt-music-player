#include <iostream>
#include <QFileDialog>
#include <QtGui>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Song.h"
#include <QMediaMetaData>

static QImage img("C:/music/cover.png");
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    //connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::on_media_loaded);

    model = new SongsModel(QSize(4, 0), this);
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
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open mp3 files");
    Song* songPtr;
    for (QString file : filePaths){
        songPtr = new Song(file);
        connect(songPtr, &Song::songFileReady, [=](Song* s) {
            model->addSong(s);
        });
    }
}

/*void MainWindow::on_media_loaded()
{
    if (player->mediaStatus() == QMediaPlayer::LoadedMedia){
            if (player->isMetaDataAvailable()) {
                std::cout << "META_DATA_AVAILABLE" << std::endl;
                ui->playedSongLabel->setText(player->metaData(QMediaMetaData::AlbumArtist).toString()
                                             + " - " +
                                             player->metaData(QMediaMetaData::Title).toString());
                QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();
                if (!url.isEmpty())
                    setCover(url.toString());
                else
                    setDefaultCover();
            } else
                std::cout << "META_DATA_NOT_AVAILABLE" << std::endl;
        player->play();
    }
}*/

void MainWindow::on_playButton_clicked()
{
    if (fileName.isNull())
        return;
    if (ui->playButton->text() == "Start") {
        if (player->mediaStatus() == QMediaPlayer::NoMedia || !(player->currentMedia().request().url().toString().endsWith(fileName))){
            player->setMedia(QUrl::fromLocalFile(fileName));
            setCurrentMetadata();
        }
        player->play();
        ui->playButton->setText("Stop");
    } else if (ui->playButton->text() == "Stop") {
        if (player->mediaStatus() == QMediaPlayer::NoMedia || !(player->currentMedia().request().url().toString().endsWith(fileName))){
            player->stop();
            player->setMedia(QUrl::fromLocalFile(fileName));
            setCurrentMetadata();
            player->play();
        } else {
            player->pause();
            ui->playButton->setText("Start");
        }
    }
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    currentSong = model->getSong(index.row());
    fileName = currentSong->fileName();
    on_playButton_clicked();
}

void MainWindow::setCurrentMetadata()
{
    ui->playedSongLabel->setText(currentSong->artist() + " - " + currentSong->title());

    QPixmap cover;
    cover.convertFromImage(currentSong->cover());
    QRect rect = ui->verticalLayout->geometry();
    ui->coverLabel->setPixmap(cover.scaled(rect.height(), rect.height()));
}
