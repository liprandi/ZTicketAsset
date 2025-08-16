#ifndef ZBACKEND_H
#define ZBACKEND_H

#include <QObject>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "zdatabase.h"


class ZBackEnd : public QObject
{
    Q_OBJECT
public:
    explicit ZBackEnd(QObject *parent = nullptr);
    virtual ~ZBackEnd();

    QJsonObject credentials(){return m_credentials;}
    QJsonArray assets() const {return m_assets;}
    QJsonArray tickets() const {return m_tickets;}
    QJsonArray steps() const {return m_steps;}

    void login(const QStringList &credentials);
    bool selectAsset(const QString& asset);
    bool selectTicket(const QString& ticket);
    void editUser(const QStringList &credentials);
    void editTicket(const QStringList &ticket);
    void editAsset(const QStringList &newasset, QDateTime deployed);
    void editLink(const QStringList &newlink);
    void newStep(const QStringList &step);
    void closeTicket(const QStringList &ticket);
    void removeUser(const QStringList &user);
    void removeAsset(const QStringList &asset);
    void removeLink(const QStringList &link);
    void readUsers();
    void readAssets();
    void readTickets();
    void readSteps();

public: // inline
    inline int updatePercent(){return m_update;}
signals:
    void credentialsChanged();
    void usersUpdated();
    void assetsUpdated();
    void ticketsUpdated();
    void stepsUpdated();

public slots:
    void importFromCsv(const QString &newuser);

    void queried(int id, const QJsonDocument& reply);

private:
    int m_update;
    QJsonObject m_credentials;
    QJsonObject m_ticket;
    QJsonObject m_asset;

    QJsonArray m_users;
    QJsonArray m_assets;
    QJsonArray m_tickets;
    QJsonArray m_steps;
private: // for import csv
    QStringList csv_users;
    QStringList csv_assets;
    QStringList csv_links;
private: // database interface
    ZDatabase m_db;

};

extern ZBackEnd* g_backend;

#endif // ZBACKEND_H
