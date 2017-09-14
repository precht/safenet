#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class ServerModel : public QAbstractListModel
{
    Q_OBJECT
public:

    ServerModel(QObject *parent = 0);
    void addFile(const QString& fileName);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void clear();

private:
    QList<QString> m_list;
};

#endif // SERVERMODEL_H
