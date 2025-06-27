#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
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
void ZDatabase::query(const QString& qString, uint sel)
{
    QNetworkRequest request;
    QString str = QString("?sel=%1&query=%2").arg(sel).arg(qString);
    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        int token = buffer.indexOf('\r');
        if(token > 0)
        {
            int id = buffer.mid(4, token - 4).toInt();

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
    QNetworkRequest request;

    QString str = QString("?sel=1&query=SELECT *,IF(password = '%1', '1', '2') AS valid FROM users WHERE nickname='%2' AND password='%3'").arg(g_pwDefault, nick, password_hash);
    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        QJsonParseError err;
        QByteArray json = buffer.mid(buffer.indexOf('\r'));
        auto doc = QJsonDocument::fromJson(json, &err);

        qDebug() << doc;

        if(err.error != QJsonParseError::NoError)
        {
            qDebug() << err.errorString();
        }
        auto firstline = doc[0];
        if(firstline.isObject())
        {
            emit sLogin(QJsonDocument(firstline.toObject()));
        }
        else
            emit sLogin(QJsonDocument::fromJson(ZDatabase::g_nouser));
    });
}
void ZDatabase::newUser(const QString &name, const QString &surname, const QString &email)
{
    QNetworkRequest request;

    QString nick = QString("%1.%2").arg(name.toLower(), surname.toLower());
    QString str = QString("?sel=2&query=REPLACE INTO users (nickname, name, surname, email, password) "
                        "values ('%1', '%2', '%3', '%4', '%5')")
                    .arg(nick, name, surname, email, g_pwDefault);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=2\r[]"))
            emit sNewUser(nick);
        else
            emit sNewUser("## ERROR in newUser ##");
    });
}
void ZDatabase::removeUser(const QString &nick)
{
    QNetworkRequest request;

    QString str = QString("?sel=3&query=DELETE FROM users WHERE nickname='%1';")
                      .arg(nick);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=3\r[]"))
            emit sRemoveUser(nick);
        else
            emit sRemoveUser("## ERROR in removeUser ##");
    });
}
void ZDatabase::newAsset(const QString &id, const QString &type, const QString &model, const QString &description, QString deployed)
{
    QNetworkRequest request;

    QString str = QString("?sel=4&query=REPLACE INTO assets (id, type, model, description) "
                          "values ('%1', '%2', '%3', '%4')")
                      .arg(id, type, model, description);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=4\r[]"))
            emit sNewAsset(id);
        else
            emit sRemoveUser("## ERROR in newAsset ##");
    });
}
void ZDatabase::removeAsset(const QString &id)
{
    QNetworkRequest request;

    QString str = QString("?sel=5&query=DELETE FROM assets WHERE id='%1';")
                      .arg(id);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=5\r[]"))
            emit sRemoveAsset(id);
        else
            emit sRemoveAsset("## ERROR in removeAsset ##");
    });
}
void ZDatabase::newLink(const QString &asset, const QString &email)
{
    QNetworkRequest request;
    QString str;
    if(email.compare(".") == 0)
    {
        str = QString("?sel=6&query=REPLACE INTO asset_user (asset, user) "
                              "VALUES ('%1', '.')")
                          .arg(asset);
    }
    else
    {
        str = QString("?sel=6&query=REPLACE INTO asset_user (asset, user) "
                      "VALUES ('%1', (SELECT nickname FROM users WHERE email='%2'))")
                          .arg(asset, email);
    }
    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=6\r[]"))
            emit sNewLink(asset);
        else
            emit sNewLink("## ERROR in newLink ##");
    });
}
void ZDatabase::removeLink(const QString &asset, const QString &nick)
{
    QNetworkRequest request;

    QString str = QString("?sel=7&query=DELETE FROM asset_user WHERE asset='%1'AND user='%2';")
                      .arg(asset, nick);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=7\r[]"))
            emit sRemoveLink(asset);
        else
            emit sRemoveLink("## ERROR in removeLink ##");
    });

}
void ZDatabase::openTicket(const QString &id, const QString &nick, const QString &asset, const QString &description)
{
    QNetworkRequest request;

    QDateTime now = QDateTime::currentDateTimeUtc();
    QString dt = now.toString("yyyy-MM-ddThh:mm:ss");

    QString st = QString("🙋‍").toUtf8();

    QString str = QString("?sel=8&query=INSERT INTO tickets (ticket, dt_open, status, user, asset, description) "
                          "values ('%1', '%2', %3, '%4', '%5', '%6')")
                      .arg(id, dt, st, nick, asset, description);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=8\r[]"))
            emit sOpenTicket(id);
        else
            emit sOpenTicket("");
    });

}
void ZDatabase::closeTicket(const QString &id)
{
    QNetworkRequest request;

    QDateTime now = QDateTime::currentDateTimeUtc();
    QString dt = now.toString("yyyy-MM-ddThh:mm:ss");

    QString st = QString("👍").toUtf8();

    QString str = QString("?sel=9&query=UPDATE tickets SET dt_close='%1', status=%2 WHERE ticket='%3'")
                      .arg(dt, st, id);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=9\r[]"))
            emit sCloseTicket(id);
        else
            emit sCloseTicket("");
    });

}
void ZDatabase::newTicketStep(const QString &id, const QString &nick, const QString &status, const QString &description)
{
    QNetworkRequest request;

    QString str = QString("?sel=10&query=INSERT INTO steps (ticket, nickname, status, description) "
                          "values ('%1', '%2', %3, '%4')")
                      .arg(id, nick, status, description);

    QUrl url(g_php + str);

    request.setRawHeader( "User-Agent" , "Mozilla Firefox" );
    request.setUrl(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=, this]
    {
        auto buffer = reply->readAll();
        if(buffer.contains("sel=10\r[]"))
            emit sNewTicketStep(id, nick);
        else
            emit sNewTicketStep("", "");
    });

}

