#ifndef ZDATABASE_H
#define ZDATABASE_H

#include <QAbstractTableModel>
#include <QNetworkAccessManager>

class ZDatabase : public QObject
{
    Q_OBJECT
public:
    enum
    {
        id_login = 0,
        id_editUser,
        id_editAsset,
        id_editTicket,
        id_editLink,
        id_newStep,
        id_closeTicket,
        id_removeTicket,
        id_removeAsset,
        id_removeUser,
        id_removeLink,
        id_editStep,
        id_readUsers,
        id_readAssets,
        id_readTickets,
        id_readSteps,
        idSize
    };
public:

    explicit ZDatabase(QObject *parent = nullptr);
    ~ZDatabase();

    bool connectdb();

    static void setPhpAddress(const QString& php){g_php = php;}
signals:
    void sQuery(int id, const QJsonDocument& reply);
    void started(const QByteArray& buffer);

public:
    void login(const QString &nick, const QString &password_hash);
    void editUser(const QString &name, const QString &surname, const QString &email, const QString& password);
    void editAsset(const QString &id, const QString &type, const QString &model, const QString &description, QString deployed);
    void editTicket(const QString &id, const QString &nick, const QString &asset, const QString &status, const QString &description);
    void editLink(const QString &asset, const QString &nick);
    void newStep(const QString &id, const QString &nick, const QString &status, const QString &description);
    void closeTicket(const QString &id);
    void removeAsset(const QString &id);
    void removeUser(const QString &nick);
    void removeLink(const QString &asset, const QString &nick);
    void readUsers();
    void readAssets(const QString &nick);
    void readTickets(const QString &nick);
    void readSteps(const QString &id);

protected:
    void queryAsync(const QString &qString, uint id, int timeoutMs = 3000);


private:    // data used for interface
    QNetworkAccessManager* m_manager;
    QByteArray m_buffer;

    static QString g_php;
public:
    static const QByteArray g_nouser;
    static const QString g_pwDefault;

};

#endif // ZDATABASE_H
