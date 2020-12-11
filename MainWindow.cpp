#include <iostream>
#include <QFileDialog>
#include <QtGui>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Song.h"
#include <QMediaMetaData>
#include <iostream>

static QImage img("C:/music/cover.png");
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::on_media_status_changed);

    model = new SongsModel(this);
    //model->setData();
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefaultCover()
{
    QPixmap cover(":/images/albumIcon.png");
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
    if (position <= 0 && player->mediaStatus() != QMediaPlayer::NoMedia)
        return;
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

void MainWindow::on_action_Open_files_triggered()
{
//    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open mp3 files");
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
        "Open files", QStandardPaths::writableLocation(QStandardPaths::MusicLocation), "MP3 files (*.mp3)");
    Song* songPtr;
    for (QString file : filePaths){
        songPtr = new Song(file);
        connect(songPtr, &Song::songFileReady, [=](Song* s) {
            model->addSong(s);
        });
    }
}

void MainWindow::on_playButton_clicked()
{
    if (fileName.isNull()) {
        if (model->rowCount() > 0){
            if (ui->shuffleButton->isChecked()){
                currentSong = model->getSong(QRandomGenerator::global()->generate() % model->rowCount());
            } else {
                currentSong = model->getSong(0);
            }
            fileName = currentSong->fileName();
            setCurrentMetadata();
            player->setMedia(QUrl::fromLocalFile(fileName));
            player->play();
            ui->playButton->setText("Stop");
        } else {
            return;
        }
    } else if (ui->playButton->text() == "Start") {
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

void MainWindow::resetPlayer()
{
    setDefaultCover();
    ui->playedSongLabel->setText("No song chosen.");
    ui->playButton->setText("Start");
    fileName = nullptr;
    player->setMedia(QMediaContent());
}

void MainWindow::on_action_Delete_files_triggered()
{
    while (true) {
        QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
        if (indexes.empty())
            return;
        if (model->getSong(indexes[0].row())->fileName() == fileName)
            resetPlayer();
        model->removeRow(indexes[0].row());
    }
//    QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();
//    int countRows = indexes.count();
//    for( int i = countRows - 1; i >= 0; --i){
//        if (model->getSong(indexes.at(i).row())->fileName() == fileName)
//            resetPlayer();
//        model->removeRow(indexes.at(i).row());
//    }
}

void MainWindow::on_action_Clear_playlist_triggered()
{
    resetPlayer();
    model->removeAllRows();
}

void MainWindow::on_media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::EndOfMedia)
        on_nextButton_clicked();
}

void MainWindow::on_nextButton_clicked()
{
    if (model->rowCount() < 1)
        return;
    if (ui->shuffleButton->isChecked()){
        if (model->rowCount() > 1){
            int i;
            do {
                i = QRandomGenerator::global()->generate() % model->rowCount();
            } while (model->getSong(i) == currentSong);
            currentSong = model->getSong(i);
        }
    } else {
        currentSong = model->getSong((model->getSongIndex(currentSong) + 1) % model->rowCount());
    }
    fileName = currentSong->fileName();
    player->setMedia(QUrl::fromLocalFile(fileName));
    setCurrentMetadata();
    player->play();
    ui->playButton->setText("Stop");
}

void MainWindow::on_previousButton_clicked()
{
    if (model->rowCount() < 1)
        return;
    if (ui->shuffleButton->isChecked()){
        on_nextButton_clicked();
    } else {
        if (model->getSongIndex(currentSong) == 0)
            currentSong = model->getSong(model->rowCount() - 1);
        else
            currentSong = model->getSong((model->getSongIndex(currentSong) - 1));
        fileName = currentSong->fileName();
        player->setMedia(QUrl::fromLocalFile(fileName));
        setCurrentMetadata();
        player->play();
        ui->playButton->setText("Stop");
    }
}

void MainWindow::on_progressSlider_sliderPressed()
{
    player->pause();
}

void MainWindow::on_progressSlider_sliderReleased()
{
    player->play();
}
