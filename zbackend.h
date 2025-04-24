#ifndef ZBACKEND_H
#define ZBACKEND_H

#include <QObject>
#include <QQmlEngine>
#include <QStringList>
#include <QJsonDocument>
#include <qqml.h>
#include "zdatabase.h"

class ZBackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString credentials READ credentials NOTIFY credentialsChanged)
    QML_ELEMENT
public:
    explicit ZBackEnd(QObject *parent = nullptr);
    QString credentials(){return m_credentials.toJson();}

signals:
    void credentialsChanged(QString );

public slots:
    void login(const QStringList &credentials);
    void addUser(const QStringList &newuser);
    void addAsset(const QStringList &newasset, QDateTime deployed = QDateTime::currentDateTimeUtc());
    void addLink(const QStringList &newlink);
    void importFromCsv(const QString &newuser);


private:
    int m_sequence;         // sequence of queries
    QJsonDocument m_credentials;
    QList<QStringList> m_newuser;
    QList<QStringList> m_newasset;
    QList<QStringList> m_newlink;
    QJsonDocument m_showTable;
    ZDatabase m_db;
};

#endif // ZBACKEND_H
