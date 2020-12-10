#ifndef SONGSLIST_H
#define SONGSLIST_H

#include <QAbstractTableModel>
#include <QSize>
#include <QModelIndex>
#include <QVariant>
#include "Song.h"

enum class SongRole { title = Qt::UserRole, artist, album, year, cover };

class SongsModel : public QAbstractTableModel {
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
public:
    SongsModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QSize size() const;
    void addSong(QString fileName);
    void addSong(Song* song);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Song* getSong(int songIndex);
public slots:
    void setSize(const QSize &s);
signals:
    void sizeChanged(QSize);
private:
    QSize m_size;
    QList<Song*> songsList;
};

#endif // SONGSLIST_H
