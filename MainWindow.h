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

    void on_action_Open_file_triggered();

    void on_media_loaded();

    void on_playButton_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QString fileName;
    SongsModel* model;
};
#endif // MAINWINDOW_H
