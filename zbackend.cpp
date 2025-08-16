#include <QCryptographicHash>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include "zemoji.h"
#include "zbackend.h"
#include "zdatabase.h"

ZBackEnd* g_backend = 0l;

ZBackEnd::ZBackEnd(QObject *parent)
    : QObject{parent}
    , m_needUpdate(false)
{
    qDebug() << "create back-end object";
    if(m_db.connectdb())
    {
        qDebug() << "create database connection";
    }

    connect(&m_db, &ZDatabase::sQuery, this, &ZBackEnd::queried);
    g_backend = this;
}

ZBackEnd::~ZBackEnd()
{
    g_backend = 0l;
}


// [0] = email
// [1] = password
void ZBackEnd::login(const QStringList &credentials)
{
    qDebug() << "Received credentials: " << credentials;
    if(credentials.length() == 2)
    {
        QString pw = QCryptographicHash::hash(credentials[1].toLatin1(), QCryptographicHash::Sha256).toHex();
        qDebug() << pw;
        m_db.login(credentials[0], pw.toLower());
    }
}

bool ZBackEnd::selectAsset(const QString& asset)
{
    bool found = false;

    for (const QJsonValue &value : std::as_const(m_assets)) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            if (obj.contains("id") && obj["id"].toString() == asset) {
                m_asset = obj;
                found = true;
                break; // stop after first match
            }
        }
    }

    if(found) {
        qDebug() << "Asset selected: " << m_asset;
        m_needUpdate = true;
    }
    return found;
}

bool ZBackEnd::selectTicket(const QString& ticket)
{
    bool found = false;

    for (const QJsonValue &value : std::as_const(m_tickets)) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            if (obj.contains("ticket") && obj["ticket"].toString() == ticket) {
                m_ticket = obj;
                found = selectAsset(obj["asset"].toString());
                break; // stop after first match
            }
        }
    }

    if(found) {
        qDebug() << "Ticket selected: " << m_ticket;
        m_needUpdate = true;
    }
    return found;
}


// [0] = Name
// [1] = Surname
// [2] = email
// [3] = password
// set default password to 'C4mb14m1'
// set nickname as name.surname in lowercase
void ZBackEnd::editUser(const QStringList &newuser)
{
    qDebug() << "adduser: " << newuser;

    if(newuser.length() == 4 && newuser[3].length() == 64) {
        m_db.editUser(newuser[0], newuser[1], newuser[2], newuser[3]);
    }
    else {
        m_db.editUser(newuser[0], newuser[1], newuser[2], ZDatabase::g_pwDefault);
    }
}

// [0] = id
// [1] = user
// [2] = asset
// [3] = description
void ZBackEnd::editTicket(const QStringList &ticket)
{
    qDebug() << "newTicket: " << ticket;
    if(ticket.length() == 4 && !ticket[0].isEmpty() && !ticket[1].isEmpty()  && !ticket[2].isEmpty()  && !ticket[3].isEmpty())
    {
        ZEmoji emoji("📖‍");

        m_db.editTicket(ticket[0], ticket[1], ticket[2], emoji.m_html, ticket[3]);
    }
}

// [0] = id
// [1] = type
// [2] = model
// [3] = description
void ZBackEnd::editAsset(const QStringList &newasset, QDateTime deployed)
{
    qDebug() << "addAsset: " << newasset;
    if(newasset.length() == 4)
    {
        QString dt = deployed.toString("yyyy-MM-ddThh:mm:ss");
        m_db.editAsset(newasset[0], newasset[1], newasset[2], newasset[3], dt);
    }
}


// [0] = asset
// [1] = email
void ZBackEnd::editLink(const QStringList &newlink)
{
    qDebug() << "addLink: " << newlink;
    if(newlink.length() == 2 && !newlink[0].isEmpty() && !newlink[1].isEmpty())
    {
        m_db.editLink(newlink[0], newlink[1]);
    }
}

// [0] = id
// [1] = nickname
// [2] = emoji
// [3] = description
void ZBackEnd::newStep(const QStringList &step)
{
    qDebug() << "addTicketStep: " << step;
    if(step.length() == 4 && !step[0].isEmpty() && !step[1].isEmpty() && !step[3].isEmpty())
    {
        ZEmoji emoji(step[2]);
        m_db.newStep(step[0], step[1], emoji.m_html, step[3]);
    }
}

// [0] = id
void ZBackEnd::closeTicket(const QStringList &ticket)
{
    qDebug() << "closeTicket: " << ticket;
    if(ticket.length() == 1 && !ticket[0].isEmpty())
    {
        ZEmoji emoji("📗");

        m_db.editTicket(ticket[0], ticket[1], ticket[2], emoji.m_html, ticket[3]);
    }
}

// [0] = user
void ZBackEnd::removeUser(const QStringList &user)
{
    qDebug() << "removeUser: " << user;
    if(user.length() == 1 && !user[0].isEmpty())
    {
        m_db.removeUser(user[0]);
    }
}

// [0] = asset
void ZBackEnd::removeAsset(const QStringList &asset)
{
    qDebug() << "removeAsset: " << asset;
    if(asset.length() == 1 && !asset[0].isEmpty())
    {
        m_db.removeAsset(asset[0]);
    }
}

// [0] = asset
// [1] = user
void ZBackEnd::removeLink(const QStringList &link)
{
    qDebug() << "removeLink: " << link;
    if(link.length() == 2 && !link[0].isEmpty() && !link[1].isEmpty())
    {
        m_db.removeLink(link[0], link[1]);
    }
}

void ZBackEnd::readUsers()
{
    if(m_users.isEmpty() || m_needUpdate)
    {
        m_needUpdate = false;
        m_db.readUsers();
    }
}


void ZBackEnd::readAssets()
{
    if(m_assets.isEmpty() || m_needUpdateAssets)
    {
        m_needUpdateAssets = false;
        QString nick = m_credentials["nickname"].toString(".");
        m_db.readAssets(nick);
    }
}

void ZBackEnd::readTickets()
{
    if(m_tickets.isEmpty() || m_needUpdateTickets)
    {
        m_needUpdateTickets = false;
        QString nick = m_credentials["nickname"].toString(".");
        m_db.readTickets(nick);
    }
}

void ZBackEnd::readSteps()
{
    if(m_steps.isEmpty() || m_needUpdateSteps)
    {
        m_needUpdateSteps = true;

        QString ticket = m_ticket["ticket"].toString(".");
        m_db.readSteps(ticket);
    }
}


void ZBackEnd::queried(int id, const QJsonDocument& reply)
{
    qDebug() << "queried: " << id;
    qDebug() << "reply: " << reply;

    switch(id)
    {
        case ZDatabase::id_login:
        {
            m_credentials = reply.object();
            if(m_credentials["nickname"].isString()){
                qDebug() << "loggged: " << m_credentials;
                emit credentialsChanged();
            }
            m_needUpdate = true;
        }
        break;
        case ZDatabase::id_editUser:
        case ZDatabase::id_editAsset:
        case ZDatabase::id_editTicket:
        case ZDatabase::id_editLink:
        case ZDatabase::id_newStep:
        case ZDatabase::id_closeTicket:
        case ZDatabase::id_removeUser:
        case ZDatabase::id_removeAsset:
        case ZDatabase::id_removeTicket:
        case ZDatabase::id_removeLink:
        {
            qDebug() << "done: " << id << ":" << reply.object();
            m_needUpdate = true;
        }
        break;
        case ZDatabase::id_readUsers:
        {
            if(reply.isArray())
            {
                m_users = reply.array();
                qDebug() << "users read";
            }
        }
        break;
        case ZDatabase::id_readAssets:
        {
            if(reply.isArray())
            {
                m_assets = reply.array();
                qDebug() << "assets read";
            }
        }
        break;
        case ZDatabase::id_readTickets:
        {
            if(reply.isArray())
            {
                m_tickets = reply.array();
                for(int i = 0; i < m_tickets.count(); i++)
                {
                    QJsonObject v = m_tickets.at(i).toObject();
                    ZEmoji emoji = ZEmoji::FromStatus(v["status"].toString());
                    v.insert("emoji", emoji.m_emoji[0]);
                    m_tickets.replace(i, v);
                }
                qDebug() << "tickets read";
            }
        }
        break;
        case ZDatabase::id_readSteps:
        {
            if(reply.isArray())
            {
                m_steps = reply.array();
                for(int i = 0; i < m_steps.count(); i++)
                {
                    QJsonObject v = m_steps.at(i).toObject();
                    ZEmoji emoji = ZEmoji::FromStatus(v["status"].toString());
                    v.insert("emoji", emoji.m_emoji[0]);
                    m_steps.replace(i, v);
                }
                qDebug() << "steps read";
            }
        }
        break;
        default:
        {

        }
    }
}


void ZBackEnd::importFromCsv(const QString& url)
{
    qDebug() << "importFromCsv: " << url;
    QUrl urlfile(url);

    QFile f(urlfile.toLocalFile());
    if(f.open(QFile::ReadOnly))
    {
        QTextStream txt(&f);
        QStringList field;
        bool header = true;
        while(!txt.atEnd())
        {
            auto line = txt.readLine();
            auto list = line.split(';');
            if(header)
            {
                field = list;
            }
            else
            {
                QMap<QString, QString> item;
                for(int i = 0; i < field.length() && i < list.length(); i++)
                {
                    item[field[i]] = list[i];
                }
                csv_users.append({item["Name"], item["Surname"], item["Email"]});
                csv_assets.append({item["IdAsset"], item["Asset"], item["Os"], "Laptop", item["Deploy"]});
                csv_links.append({item["IdAsset"], item["Email"]});
            }
            header = false;
        }
        f.close();
    }
}

