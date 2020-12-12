/*
 * File name: SongsModel.h
 * Author: Michał Kopyt (300231)
 * Project: Qt Music Player
 */

#ifndef SONGSLIST_H
#define SONGSLIST_H

#include <QAbstractTableModel>
#include <QSize>
#include <QModelIndex>
#include <QVariant>
#include "Song.h"

class SongsModel : public QAbstractTableModel {
    Q_OBJECT
public:
    SongsModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void addSong(QString fileName);
    void addSong(Song* song);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Song* getSong(int songIndex);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeAllRows(const QModelIndex &parent = QModelIndex());
    int getSongIndex(Song* song);
signals:
    void songsAvailabilityChanged(bool hasSongs);
private:
    QList<Song*> songsList;
};

#endif // SONGSLIST_H
