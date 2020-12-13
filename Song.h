/*
 * File name: Song.h
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QVariant>
#include <QImage>
#include <QMediaPlayer>

class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(QImage cover READ cover)
    Q_PROPERTY(QString fileName READ fileName)

public:
    explicit Song(QString fileName);
    QString title() const;
    QString artist() const;
    QString album() const;
    QImage cover() const;
    QString fileName() const;

signals:
    void songFileReady(Song*);

private slots:
    void on_media_status_changed(QMediaPlayer::MediaStatus status);

private:
    QString m_fileName;
    QString m_title;
    QString m_artist;
    QString m_album;
    QImage m_cover;
    QMediaPlayer* metadataLoader;
    void setMetadataLoader();
};

#endif // SONG_H
