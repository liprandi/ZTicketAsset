#ifndef ZTABLE_H
#define ZTABLE_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QQmlEngine>

class ZTable : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit ZTable(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override
    {
        return 5;
    }

    int columnCount(const QModelIndex & = QModelIndex()) const override
    {
        return 4;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        switch (role) {
        case Qt::DisplayRole:
            return QString("%1, %2").arg(index.column()).arg(index.row());
        default:
            break;
        }

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return { {Qt::DisplayRole, "display"} };
    }
};


#endif // ZTABLE_H
