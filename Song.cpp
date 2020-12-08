#include "Song.h"
#include <QMediaMetaData>
#include <QPixmap>
#include <iostream>

Song::Song(QString fileName) : QObject(nullptr), m_fileName(fileName), m_title("UNKNOWN_TITLE"), m_artist("UNKNOWN_ARTIST"),
    m_album("UNKNOW_ALBUM"), m_year("2020"), m_cover("C:/music/cover.png"){
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

QString Song::year() const
{
    return m_year;
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
    connect(metadataLoader, static_cast<void(QMediaPlayer::*)(void)>(&QMediaPlayer::metaDataChanged), this, &Song::on_metadata_changed);
    metadataLoader->setMedia(QUrl::fromLocalFile(m_fileName));

}
//void Song::setCover()
//{
//    QPixmap cover;
//    cover.fromImage(m_cover);
//    QRect rect = ui->verticalLayout->geometry();
//    ui->coverLabel->setPixmap(cover.scaled(rect.height(), rect.height()));
//}

void Song::on_metadata_changed()
{
    if (metadataLoader->isMetaDataAvailable()) {
        QStringList availableMetadata = metadataLoader->availableMetaData();
        if (availableMetadata.contains("Title"))
            m_title = metadataLoader->metaData(QMediaMetaData::Title).toString();
        if (availableMetadata.contains("AlbumArtist"))
            m_artist = metadataLoader->metaData(QMediaMetaData::AlbumArtist).toString();
        if (availableMetadata.contains("AlbumTitle"))
            m_album = metadataLoader->metaData(QMediaMetaData::AlbumTitle).toString();
        if (availableMetadata.contains("Year"))
            m_year = metadataLoader->metaData(QMediaMetaData::Year).toString();
        if (availableMetadata.contains("ThumbnailImage"))
            m_cover = metadataLoader->metaData(QMediaMetaData::ThumbnailImage).value<QImage>();
        emit songFileReady(this);
    }
}
