#ifndef SONGSLIST_H
#define SONGSLIST_H

#include <QAbstractItemModel>
#include <QSize>
#include <QModelIndex>
#include <QVariant>

enum class SongRole { title = Qt::UserRole, artist, album, year, cover };

class SongsModel : public QAbstractItemModel {
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
public:
    SongsModel(QSize s, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QSize size() const;
public slots:
    void setSize(const QSize &s);
signals:
    void sizeChanged(QSize);
private:
    QSize m_size;
};

#endif // SONGSLIST_H
