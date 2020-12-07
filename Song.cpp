#include "Song.h"

Song::Song() : QObject(nullptr){
    m_title = "UNKNOWN_TITLE";
    m_artist = "UNKONW_ARTIST";
    m_album = "UNKNOW_ALBUM";
    m_year = 2020;
    m_cover = new QImage("C:/music/cover.png");
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

unsigned int Song::year() const
{
    return m_year;
}

QImage* Song::cover() const
{
    return m_cover;
}

QString Song::fileName() const
{
    return m_fileName;
}
