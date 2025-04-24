#include <QCryptographicHash>
#include <QUrl>
#include <QFile>
#include <QTextStream>

#include "ztable.h"

ZTable::ZTable(QObject *parent)
    : QAbstractTableModel{parent}
{

}
