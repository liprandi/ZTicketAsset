#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QTimer>
#include "zemoji.h"
#include "zdatabase.h"

#define errdb {qDebug() <<  mysql_error(m_sql); return false;}

const QByteArray ZDatabase::g_nouser("{\"name\": \".\", \"surname\": \".\", \"email\": \".\", \"password\": \".\", \"valid\": \"0\"");
const QString ZDatabase::g_pwDefault = "58ec43f4b7eb08f7ddb3b2e9092b83d9d80fb66fe2929219d506ac0b384eb7d5";
QString ZDatabase::g_php;

ZDatabase::ZDatabase(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}

ZDatabase::~ZDatabase()
{
    delete m_manager;
}

bool ZDatabase::connectdb()
{
    QNetworkRequest request;
    QUrl url(g_php + "?sel=0&query=SHOW%20VARIABLES");

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        qDebug() << "reply: " <<  buffer;
        if(reply->error())
        {
            qDebug() << reply->errorString();
        }
        emit started(buffer);
    });

   return true;
}


void ZDatabase::queryAsync(const QString& qString, uint id, int timeoutMs)
{
    QNetworkRequest request;
    QString str = QString("?sel=%1&query=%2").arg(id).arg(qString);
    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QTimer *timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->start(5000);  // 5 seconds timeout

    QNetworkReply* reply = m_manager->get(request);

    connect(timeoutTimer, &QTimer::timeout, this, [reply, id, this]() {
        if (reply->isRunning()) {
            qDebug() << "Request timed out. Aborting.";
            reply->abort();
        }
        emit sQuery(id, QJsonDocument());
    });

    connect(reply, &QNetworkReply::finished, this, [reply, this]
    {
        auto buffer = reply->readAll();
        int token = buffer.indexOf('\r');
        if(token > 0)
        {
            int sel = buffer.mid(4, token - 4).toInt();

            QJsonParseError err;
            QByteArray json = buffer.mid(token);
            auto receive = QJsonDocument::fromJson(json, &err);

            if(err.error != QJsonParseError::NoError)
            {
                qDebug() << err.errorString();
            }
            emit sQuery(sel, receive);
        }
    });
}

void ZDatabase::login(const QString &nick, const QString &password_hash)
{
    QString str = QString("SELECT *,IF(password = '%1', '1', '2') AS valid FROM users WHERE nickname='%2' AND password='%3'").arg(g_pwDefault, nick, password_hash);

    queryAsync(str, id_login);
}
void ZDatabase::editUser(const QString &name, const QString &surname, const QString &email, const QString& password)
{
    QString nick = QString("%1.%2").arg(name.toLower(), surname.toLower());
    QString str = QString("REPLACE INTO users (nickname, name, surname, email, password) "
                        "values ('%1', '%2', '%3', '%4', '%5')")
                    .arg(nick, name, surname, email, password);

    queryAsync(str, id_editUser);
}
void ZDatabase::editAsset(const QString &id, const QString &type, const QString &model, const QString &description, QString deployed)
{
    QString str = QString("REPLACE INTO assets (id, type, model, description) "
                          "values ('%1', '%2', '%3', '%4')")
                      .arg(id, type, model, description);

    queryAsync(str, id_editAsset);
}
void ZDatabase::editTicket(const QString &id, const QString &nick, const QString &asset, const QString &status, const QString &description)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QString dt = now.toString("yyyy-MM-ddThh:mm:ss");

    QString str = QString("INSERT INTO tickets (ticket, dt_open, status, user, asset, description) "
                          "values ('%1', '%2', '%3', '%4', '%5', '%6')")
                      .arg(id, dt, status, nick, asset, description);

    queryAsync(str, id_editTicket);
}
void ZDatabase::editLink(const QString &asset, const QString &email)
{
    QString str;
    if(email.compare(".") == 0)
    {
        str = QString("REPLACE INTO asset_user (asset, user) "
                      "VALUES ('%1', '.')")
                  .arg(asset);
    }
    else
    {
        str = QString("REPLACE INTO asset_user (asset, user) "
                      "VALUES ('%1', (SELECT nickname FROM users WHERE email='%2'))")
                  .arg(asset, email);
    }

    queryAsync(str, id_editLink);
}
void ZDatabase::newStep(const QString &id, const QString &nick, const QString &status, const QString &description)
{
    QString str = QString("INSERT INTO steps (ticket, nickname, status, description) "
                          "values ('%1', '%2', '%3', '%4')")
                      .arg(id, nick, status, description);

    queryAsync(str, id_newStep);
}
void ZDatabase::closeTicket(const QString &id)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QString dt = now.toString("yyyy-MM-ddThh:mm:ss");

    ZEmoji emoji("👍");

    QString str = QString("UPDATE tickets SET dt_close='%1', status='%2' WHERE ticket='%3'")
                      .arg(dt, emoji.m_html, id);

    queryAsync(str, id_closeTicket);
}
void ZDatabase::removeAsset(const QString &id)
{
    QString str = QString("DELETE FROM assets WHERE id='%1';")
    .arg(id);

    queryAsync(str, id_removeAsset);

    QString lnk = QString("DELETE FROM asset_user WHERE asset='%1';")
                      .arg(id);

    queryAsync(lnk, id_removeLink);
}
void ZDatabase::removeUser(const QString &nick)
{
    QString str = QString("DELETE FROM users WHERE nickname='%1';")
    .arg(nick);

    queryAsync(str, id_removeUser);

    QString lnk = QString("DELETE FROM asset_user WHERE user='%1';")
                      .arg(nick);

    queryAsync(lnk, id_removeLink);
}
void ZDatabase::removeLink(const QString &asset, const QString &nick)
{
    QString str = QString("DELETE FROM asset_user WHERE asset='%1'AND user='%2';")
                      .arg(asset, nick);

    queryAsync(str, id_removeLink);
}
void ZDatabase::readUsers()
{
    QString str = "SELECT * FROM users ORDER BY users.surname";

    queryAsync(str, id_readUsers);

}
void ZDatabase::readAssets(const QString &nick)
{
    QString str = QString("SELECT assets.id, assets.type, assets.model, assets.description, IFNULL(assets.deployed, '2025-01-01T00:00:00') AS date, asset_user.user FROM assets, asset_user WHERE asset_user.asset = assets.id AND (asset_user.user='%1' OR asset_user.user='.')").arg(nick);

    queryAsync(str, id_readAssets);

}
void ZDatabase::readTickets(const QString &nick)
{
    QString str = QString("SELECT ticket, dt_open, user, asset, status, description FROM tickets WHERE user = '%1'").arg(nick);

    queryAsync(str, id_readTickets);
}
void ZDatabase::readSteps(const QString &id)
{
    QString str = QString("SELECT ticket, nickname, status, description FROM steps WHERE ticket = '%1'").arg(id);

    queryAsync(str, id_readSteps);
}
