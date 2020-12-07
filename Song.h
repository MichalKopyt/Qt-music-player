#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QVariant>
#include <QImage>
#include <QFile>

class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(unsigned int year READ year)
    Q_PROPERTY(QImage* cover READ cover)
    Q_PROPERTY(QString fileName READ fileName)

public:
    explicit Song();
    QString title() const;
    QString artist() const;
    QString album() const;
    unsigned int year() const;
    QImage* cover() const;
    QString fileName() const;
signals:

private:
    QString m_title;
    QString m_artist;
    QString m_album;
    unsigned int m_year;
    QImage* m_cover;
    QString m_fileName;

};

#endif // SONG_H
