#include "SongsModel.h"

SongsModel::SongsModel(QSize s, QObject *parent) : QAbstractItemModel(parent), m_size(s) {}

int SongsModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : size().height();
}

int SongsModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : size().width();
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
    if (role == Qt::DecorationRole || role == (int) SongRole::cover){

    }
    if (role == Qt::ToolTipRole){

    }
    if (role == (int) SongRole::title){

    }
    if (role == (int) SongRole::artist){

    }
    if (role == (int) SongRole::album){

    }
    if (role == (int) SongRole::year){

    }
    return QVariant();
}

QSize SongsModel::size() const {
    return m_size;
}

void SongsModel::setSize(const QSize &s) {
    if (m_size == s)
        return;
    beginResetModel();
    m_size = s;
    endResetModel();
    emit sizeChanged(m_size);
}
