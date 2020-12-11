#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtCore>
#include <QtGui>
#include <SongsModel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setDefaultCover();

private slots:
    void on_progressSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);

    void on_position_changed(qint64 position);

    void on_duration_changed(qint64 position);

    void on_action_Add_files_triggered();

    void on_playButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_action_Delete_files_triggered();

    void on_action_Clear_playlist_triggered();

    void on_media_status_changed(QMediaPlayer::MediaStatus status);

    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void on_progressSlider_sliderPressed();

    void on_progressSlider_sliderReleased();

    void on_action_Exit_program_triggered();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QString fileName;
    SongsModel* model;
    Song* currentSong;
    void setCurrentMetadata();
    void resetPlayer();
};
#endif // MAINWINDOW_H
