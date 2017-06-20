 #include "servermodel.h"

ServerModel::ServerModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void ServerModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_list.clear();
    endRemoveRows();
}

void ServerModel::addFile(const QString &fileName)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_list << fileName;
    endInsertRows();
}

int ServerModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_list.count();
}

QVariant ServerModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_list.count())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return m_list.at(index.row());
    }
    return QVariant();
}
