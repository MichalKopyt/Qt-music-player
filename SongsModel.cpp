/*
 * File name: SongsModel.cpp
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#include <QFont>
#include "SongsModel.h"

SongsModel::SongsModel(QObject *parent) : QAbstractTableModel(parent) {}

int SongsModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : songsList.length();
}

int SongsModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

QModelIndex SongsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();
    if (row >= rowCount() || column >= columnCount() || column < 0 || row < 0)
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex SongsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

QVariant SongsModel::data(const QModelIndex &index, int role) const
{
    switch (role){
    case Qt::DisplayRole:
        if (index.row() < songsList.length() && index.row() >= 0){
            switch (index.column()){
            case 0:
                return songsList[index.row()]->title();
            case 1:
                return songsList[index.row()]->artist();
            case 2:
                return songsList[index.row()]->album();
            default:
                break;
            }
        }
        break;
    case Qt::TextAlignmentRole:
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::ToolTipRole:
        if (index.row() < songsList.length() && index.row() >= 0){
            return songsList[index.row()]->fileName();
        }
        break;
    default:
        break;
    }
    return QVariant();
}

void SongsModel::addSong(QString fileName)
{
    addSong(new Song(fileName));
}

void SongsModel::addSong(Song* song)
{
    for (Song* s : songsList){
        if (s->fileName() == song->fileName())
            return;
    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    songsList.append(song);
    endInsertRows();
    if (songsList.length() == 1)
        emit songsAvailabilityChanged(true);
}

QVariant SongsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role){
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal){
            switch (section) {
            case 0:
                return QString("Title");
            case 1:
                return QString("Artist");
            case 2:
                return QString("Album");
            }
        }
        break;
    case Qt::FontRole:
        QFont boldFont;
        boldFont.setBold(true);
        return boldFont;
    }
    return QVariant();
}

Song* SongsModel::getSong(int songIndex)
{
    if (songIndex < 0 || songIndex >= songsList.length())
        return nullptr;
    return songsList[songIndex];
}

bool SongsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row + count <= rowCount()){
        beginRemoveRows(QModelIndex(), row, row + count - 1);
        songsList.erase(songsList.begin() + row, songsList.begin() + row + count);
        endRemoveRows();
        if (songsList.isEmpty())
            emit songsAvailabilityChanged(false);
        return true;
    }
    return false;
}

bool SongsModel::removeAllRows(const QModelIndex &parent)
{
    return removeRows(0, rowCount(), parent);
}

int SongsModel::getSongIndex(Song *song){
    return songsList.indexOf(song);
}
