/*
 * File name: Song.cpp
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#include <QMediaMetaData>
#include <QPixmap>
#include <QtDebug>
#include "Song.h"

Song::Song(QString fileName) : QObject(nullptr),
        m_fileName(fileName), m_title("UNKNOWN_TITLE"),
        m_artist("UNKNOWN_ARTIST"), m_album("UNKNOW_ALBUM"),
        m_cover(":/images/albumIcon.png")
{
    setMetadataLoader();
}

QString Song::title() const
{
    return m_title;
}

QString Song::artist() const
{
    return m_artist;
}

QString Song::album() const
{
    return m_album;
}

QImage Song::cover() const
{
    return m_cover;
}

QString Song::fileName() const
{
    return m_fileName;
}

void Song::setMetadataLoader()
{
    metadataLoader = new QMediaPlayer(this);
    connect(metadataLoader, &QMediaPlayer::mediaStatusChanged, this, &Song::on_media_status_changed);
    metadataLoader->setMedia(QUrl::fromLocalFile(m_fileName));
}

void Song::on_media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (metadataLoader->error() != QMediaPlayer::NoError
            || (status == QMediaPlayer::LoadedMedia && !(metadataLoader->isAudioAvailable()))) {
        qDebug() << "Error while opening file: " << fileName() << ". Please check this file or codecs in your OS." ;
        delete this;
        return;
    }
    if (status == QMediaPlayer::LoadedMedia){
        if (metadataLoader->isMetaDataAvailable()) {
            QStringList availableMetadata = metadataLoader->availableMetaData();

            if (availableMetadata.contains("Title")
                    && !(metadataLoader->metaData(QMediaMetaData::Title).toString().isEmpty()))
                m_title = metadataLoader->metaData(QMediaMetaData::Title).toString();

            if (availableMetadata.contains("AlbumArtist")
                    && !(metadataLoader->metaData(QMediaMetaData::AlbumArtist).toString().isEmpty()))
                m_artist = metadataLoader->metaData(QMediaMetaData::AlbumArtist).toString();

            if (availableMetadata.contains("AlbumTitle")
                    && !(metadataLoader->metaData(QMediaMetaData::AlbumTitle).toString().isEmpty()))
                m_album = metadataLoader->metaData(QMediaMetaData::AlbumTitle).toString();

            if (availableMetadata.contains("ThumbnailImage"))
                m_cover = metadataLoader->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
        }
        emit songFileReady(this);
        delete metadataLoader;
    }
}
