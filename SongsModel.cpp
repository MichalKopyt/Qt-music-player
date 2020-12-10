#include "SongsModel.h"
#include <iostream>

SongsModel::SongsModel( QObject *parent) : QAbstractTableModel(parent), m_size(0, 4) {}

int SongsModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : size().height();
}

int SongsModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : 4;
}

QModelIndex SongsModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid())
        return QModelIndex();
    if (row >= size().height() || column >= size().width() || column < 0 || row < 0)
        return QModelIndex();
    return createIndex(row, column, nullptr);
}

QModelIndex SongsModel::parent(const QModelIndex &index) const {
    return QModelIndex();
}

QVariant SongsModel::data(const QModelIndex &index, int role) const {
    if (index.column() == 0){
        if (index.row() < songsList.length())
            return songsList[index.row()]->title();
    }
    if (index.column() == 1){
        if (index.row() < songsList.length())
            return songsList[index.row()]->artist();
    }
    if (index.column() == 2){
        if (index.row() < songsList.length())
            return songsList[index.row()]->album();
    }
    if (index.column() == 3){
        if (index.row() < songsList.length())
            return songsList[index.row()]->year();
    }
    return QVariant();
}

QSize SongsModel::size() const {
    return m_size;
}

void SongsModel::addSong(QString fileName) {
    addSong(new Song(fileName));
}

void SongsModel::addSong(Song* song) {
    songsList.append(song);
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    insertRows(rowCount(), 1);
    endInsertRows();
    emit dataChanged(index(rowCount(), 0), index(rowCount(), 3));
    std::cout << rowCount() << " " << columnCount() << std::endl;
}

QVariant SongsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Title");
        case 1:
            return QString("Artist");
        case 2:
            return QString("Album");
        case 3:
            return QString("Year");
        }
    }
    return QVariant();
}

bool SongsModel::insertRows(int row, int count, const QModelIndex &parent){
    if (row == rowCount()){
        setSize(QSize(rowCount() + count, columnCount()));
        return true;
    }
    return false;
}

Song* SongsModel::getSong(int songIndex){
    if (songIndex < 0 || songIndex >= songsList.length())
        return nullptr;
    return songsList[songIndex];
}

void SongsModel::setSize(const QSize &s) {
    if (m_size == s)
        return;
    beginResetModel();
    m_size = s;
    endResetModel();
    emit sizeChanged(m_size);
}
