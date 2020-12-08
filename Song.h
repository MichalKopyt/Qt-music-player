#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QVariant>
#include <QImage>
#include <QFile>
#include <QMediaPlayer>

class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(QString year READ year)
    Q_PROPERTY(QImage cover READ cover)
    Q_PROPERTY(QString fileName READ fileName)

public:
    explicit Song(QString fileName);
    QString title() const;
    QString artist() const;
    QString album() const;
    QString year() const;
    QImage cover() const;
    QString fileName() const;
signals:
    void songFileReady(Song*);
private slots:
    void on_metadata_changed();

private:
    QString m_fileName;
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_year;
    QImage m_cover;
    QMediaPlayer* metadataLoader;
    void setMetadataLoader();
};

#endif // SONG_H
