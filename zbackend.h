#ifndef ZBACKEND_H
#define ZBACKEND_H

#include <QObject>
#include <QQmlEngine>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qqml.h>
#include "zdatabase.h"
#include <mutex>
#include <condition_variable>


class ZBackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject credentials READ credentials NOTIFY credentialsChanged)
    Q_PROPERTY(QJsonArray assets READ assets NOTIFY assetsUpdated)
    Q_PROPERTY(QJsonArray tickets READ tickets NOTIFY ticketsUpdated)
    QML_ELEMENT
public:
    enum
    {
        id_assets = 101,
        id_tickets  = 102,
        id_currentTicket = 103,
    };
public:
    explicit ZBackEnd(QObject *parent = nullptr);
    QJsonObject credentials(){return m_credentials;}
    QJsonArray assets() const {return m_assets;}
    QJsonArray tickets() const {return m_tickets;}

signals:
    void credentialsChanged();
    void assetsUpdated();
    void ticketsUpdated();

public slots:
    void login(const QStringList &credentials);
    void addUser(const QStringList &newuser);
    void addAsset(const QStringList &newasset, QDateTime deployed = QDateTime::currentDateTimeUtc());
    void addLink(const QStringList &newlink);
    void newTicket(const QStringList &ticket); // id, user, asset, description
    void addTicketStep(const QStringList &step); // id, code, description
    void closeTicket(const QStringList &ticket); // id
    void importFromCsv(const QString &newuser);
    void queried(int id, const QJsonDocument& reply);
    void readAssets();
    void readTickets();


private:
    int m_sequence;         // sequence of queries
    QJsonObject m_credentials;
    QList<QStringList> m_newuser;
    QList<QStringList> m_newasset;
    QList<QStringList> m_newlink;
    QJsonArray m_assets;
    QJsonArray m_tickets;
    bool m_needUpdateAssets;
    bool m_needUpdateTickets;
    std::mutex m_mtxAssets;
    std::mutex m_mtxTickets;
    std::condition_variable m_cvAssets;
    std::condition_variable m_cvTickets;


    ZDatabase m_db;
};

#endif // ZBACKEND_H
