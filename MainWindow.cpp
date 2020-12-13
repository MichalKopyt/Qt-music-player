/*
 * File name: MainWindow.cpp
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#include <QFileDialog>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Song.h"

static QImage img("C:/music/cover.png");
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    player = new QMediaPlayer(this);
    model = new SongsModel(this);
    ui->tableView->setModel(model);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::on_media_status_changed);
    connect(model, &SongsModel::songsAvailabilityChanged, this, [&](bool b) {
        ui->action_Clear_playlist->setEnabled(b);
        if (!(ui->tableView->selectionModel()->selectedRows().isEmpty()))
            ui->action_Delete_files->setEnabled(b);
        ui->previousButton->setEnabled(b);
        ui->playButton->setEnabled(b);
        ui->nextButton->setEnabled(b);
    });
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [&](){
        if (!(ui->tableView->selectionModel()->selectedRows().isEmpty())
                && ui->action_Clear_playlist->isEnabled())
            ui->action_Delete_files->setEnabled(true);
        else
            ui->action_Delete_files->setEnabled(false);
    });
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, [&](QPoint pos){
        QMenu* menu = new QMenu();
        menu->addAction(ui->menu_File->actions()[0]);
        menu->addAction(ui->menu_File->actions()[1]);
        menu->addAction(ui->menu_File->actions()[2]);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    });
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
    setTimeLabelText(ui->progressLabel, position);
}

void MainWindow::on_duration_changed(qint64 position)
{
    if (position <= 0 && player->mediaStatus() != QMediaPlayer::NoMedia)
        return;
    ui->progressSlider->setMaximum(position);
    setTimeLabelText(ui->durationLabel, position);
}

void MainWindow::on_action_Add_files_triggered()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
        "Open files", QStandardPaths::writableLocation(QStandardPaths::MusicLocation), "MP3 files (*.mp3)");
    Song* songPtr;
    for (QString file : filePaths){
        songPtr = new Song(file);
        connect(songPtr, &Song::songFileReady, [&](Song* s) {
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
            player->setMedia(QUrl::fromLocalFile(fileName));
        } else {
            return;
        }
    } else if (ui->playButton->text() == "Start") {
        if (player->mediaStatus() == QMediaPlayer::NoMedia || !(player->currentMedia().request().url().toString().endsWith(fileName))){
            player->setMedia(QUrl::fromLocalFile(fileName));
        } else {
            player->play();
            ui->playButton->setText("Stop");
        }
    } else if (ui->playButton->text() == "Stop") {
        if (player->mediaStatus() == QMediaPlayer::NoMedia || !(player->currentMedia().request().url().toString().endsWith(fileName))){
            player->stop();
            player->setMedia(QUrl::fromLocalFile(fileName));
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

void MainWindow::setTimeLabelText(QLabel* label, qint64 position)
{
    int seconds = (position / 1000) % 60;
    int minutes = (position / 60000);
    QString labelText;
    if (QString::number(seconds).length() == 1)
        labelText = QString::number(minutes) + ":0" + QString::number(seconds);
    else
        labelText = QString::number(minutes) + ":" + QString::number(seconds);
    label->setText(labelText);
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
}

void MainWindow::on_action_Clear_playlist_triggered()
{
    resetPlayer();
    model->removeAllRows();
}

void MainWindow::on_media_status_changed(QMediaPlayer::MediaStatus status)
{
    switch(status){
    case QMediaPlayer::LoadedMedia:
        if (player->state() != QMediaPlayer::PlayingState) {
            setCurrentMetadata();
            player->play();
            ui->playButton->setText("Stop");
        }
        break;
    case QMediaPlayer::EndOfMedia:
        on_nextButton_clicked();
        break;
    case QMediaPlayer::InvalidMedia:
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Qt MP3 Player - unable to load file");
            msgBox.setWindowIcon(QIcon(":/images/albumIcon.png"));
            msgBox.setText("File \"" + fileName +"\" could not be loaded. It has been deleted from the playlist.");
            resetPlayer();
            model->removeRow(model->getSongIndex(currentSong));
            msgBox.exec();
        }
        break;
    default:
        break;
    }
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
    }
}

void MainWindow::on_progressSlider_sliderPressed()
{
    player->pause();
}

void MainWindow::on_progressSlider_sliderReleased()
{
    if (ui->playButton->text() != "Start")
        player->play();
}

void MainWindow::on_action_Exit_program_triggered()
{
    QCoreApplication::quit();
}
