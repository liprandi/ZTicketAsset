#ifndef ZDATABASE_H
#define ZDATABASE_H

#include <QAbstractTableModel>
#include <QNetworkAccessManager>


class ZDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ZDatabase(QObject *parent = nullptr);
    ~ZDatabase();

    bool connectdb();

    static void setPhpAddress(const QString& php){g_php = php;}
signals:
    void started(const QByteArray& result);
    void sQuery(int id, const QJsonDocument& reply);
    void sLogin(const QJsonDocument& reply);
    void sNewUser(const QString& nick);
    void sRemoveUser(const QString& nick);
    void sNewAsset(const QString& id);
    void sRemoveAsset(const QString& id);
    void sNewLink(const QString& id);
    void sRemoveLink(const QString& id);
    void sOpenTicket(int id);
    void sCloseTicket(int id);
    void sNewTicketStep(int id);


public:
    void query(const QString &qString, uint id);
    void login(const QString &nick, const QString &password_hash);
    void newUser(const QString &name, const QString &surname, const QString &email);
    void removeUser(const QString &nick);
    void newAsset(const QString &id, const QString &type, const QString &model, const QString &description, QString deployed);
    void removeAsset(const QString &id);
    void newLink(const QString &asset, const QString &nick);
    void removeLink(const QString &asset, const QString &nick);
    void openTicket(int id, const QString &asset, const QString &nick, const QString &description);
    void closeTicket(int id);
    void newTicketStep(int id, const QString emoji, const QString &description);

private:    // data used for interface
    QNetworkAccessManager* m_manager;
    static QString g_php;
    static const QByteArray g_nouser;
};

#endif // ZDATABASE_H
