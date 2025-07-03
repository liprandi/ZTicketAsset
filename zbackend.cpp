#include <QCryptographicHash>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include "zemoji.h"
#include "zbackend.h"
#include "zdatabase.h"

extern QObject *qmlObj;


ZBackEnd::ZBackEnd(QObject *parent)
    : QObject{parent}
    , m_needUpdateAssets(false)
    , m_needUpdateTickets(false)
    , m_sequence(0)
{
    qDebug() << "create back-end object";
    if(m_db.connectdb())
    {
        qDebug() << "create database connection";
    }
    connect(&m_db, &ZDatabase::started, this, [=, this](const QByteArray result)
    {
        qDebug() << result;

        QString pw = QCryptographicHash::hash("C4mb14m1", QCryptographicHash::Sha256).toHex();
        QString q = QString("INSERT IGNORE INTO users (nickname, name, surname, email, password) "
                            "values ('paolo.liprandi', 'Paolo', 'Liprandi', 'liprandi@camdobrasil.com', '%1')").arg(pw);
        m_db.query(q, ++m_sequence);
    });
    connect(&m_db, &ZDatabase::sLogin, this, [=, this](const QJsonDocument& reply)
    {
        qDebug() << "login user: " << reply["nickname"];

        m_credentials = reply.object();
        m_needUpdateAssets = m_needUpdateTickets = true;
        if(m_credentials["nickname"].isString())
            emit credentialsChanged();
    });

    connect(&m_db, &ZDatabase::sNewUser, this, [=, this](const QString& nick)
    {
        qDebug() << "added user: " << nick;

        if(m_newuser.length() > 0)
        {
            auto item = m_newuser.takeFirst();
            addUser(item);
        }
    });
    connect(&m_db, &ZDatabase::sNewAsset, this, [=, this](const QString& id)
    {
        qDebug() << "added asset: " << id;

        if(m_newasset.length() > 0)
        {
            auto asset = m_newasset.takeFirst();
            addAsset({asset[0], asset[1],  asset[2], asset[3]}, QDateTime::fromString(asset[4], "dd/MM/yyyy"));
        }
    });

    connect(&m_db, &ZDatabase::sNewLink, this, [=, this](const QString& id)
    {
        qDebug() << "added link: " << id;

        if(m_newlink.length() > 0)
        {
            auto link = m_newlink.takeFirst();
            addLink(link);
        }
        else
        {
            readAssets();
        }
    });

    connect(&m_db, &ZDatabase::sOpenTicket, this, [=, this](const QString& id)
    {
        qDebug() << "Opened Ticket: " << id;

        readTickets();
    });
    connect(&m_db, &ZDatabase::sNewTicketStep, this, [=, this](const QString&  id)
    {
        qDebug() << "Add step to Ticket: " << id;

        readTickets();
    });
    connect(&m_db, &ZDatabase::sCloseTicket, this, [=, this](const QString&  id)
    {
        qDebug() << "Ticket closed: " << id;

        readTickets();
    });

    connect(&m_db, &ZDatabase::sQuery, this, &ZBackEnd::queried);

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
// [0] = Name
// [1] = Surname
// [2] = email
// set default password to 'C4mb14m1'
// set nickname as name.surname in lowercase
void ZBackEnd::addUser(const QStringList &newuser)
{
    qDebug() << "adduser: " << newuser;
    if(newuser.length() == 3)
    {
        m_db.newUser(newuser[0], newuser[1], newuser[2]);
    }
}

// [0] = id
// [1] = type
// [2] = model
// [3] = description
void ZBackEnd::addAsset(const QStringList &newasset, QDateTime deployed)
{
    qDebug() << "addAsset: " << newasset;
    if(newasset.length() == 4)
    {
        QString dt = deployed.toString("yyyy-MM-ddThh:mm:ss");
        m_db.newAsset(newasset[0], newasset[1], newasset[2], newasset[3], dt);
    }
}


// [0] = asset
// [1] = email
void ZBackEnd::addLink(const QStringList &newlink)
{
    qDebug() << "addLink: " << newlink;
    if(newlink.length() == 2 && !newlink[0].isEmpty() && !newlink[1].isEmpty())
    {
        m_db.newLink(newlink[0], newlink[1]);
    }
}

void closeTicket(const QStringList &ticket); // id

// [0] = id
// [1] = user
// [2] = asset
// [3] = description
void ZBackEnd::newTicket(const QStringList &ticket)
{
    qDebug() << "newTicket: " << ticket;
    if(ticket.length() == 4 && !ticket[0].isEmpty() && !ticket[1].isEmpty()  && !ticket[2].isEmpty()  && !ticket[3].isEmpty())
    {
        m_db.openTicket(ticket[0], ticket[1], ticket[2],  ticket[3]);
    }
}

// [0] = id
// [1] = nickname
// [2] = emoji
// [3] = description
void ZBackEnd::addTicketStep(const QStringList &step)
{
    qDebug() << "addTicketStep: " << step;
    if(step.length() == 4 && !step[0].isEmpty() && !step[1].isEmpty() && !step[3].isEmpty())
    {
        ZEmoji emoji(step[2]);
        m_db.newTicketStep(step[0], step[1], emoji.m_html, step[3]);
    }
}

// [0] = id
void ZBackEnd::closeTicket(const QStringList &ticket)
{
    qDebug() << "closeTicket: " << ticket;
    if(ticket.length() == 1 && !ticket[0].isEmpty())
    {
        m_db.closeTicket(ticket[0]);
    }
}

void ZBackEnd::readAssets()
{
    if(m_assets.isEmpty() || m_needUpdateAssets)
    {
        m_needUpdateAssets = true;

        QString str = QString("SELECT assets.id, assets.type, assets.model, assets.description, IFNULL(assets.deployed, '2025-01-01T00:00:00') AS date, asset_user.user FROM assets, asset_user WHERE asset_user.asset = assets.id AND (asset_user.user='%1' OR asset_user.user='.')").arg(m_credentials["nickname"].toString("."));

        m_db.query(str, id_assets);
//        std::unique_lock<std::mutex> lock(m_mtxAssets);
//        m_cvAssets.wait_for(lock, std::chrono::seconds(10), [this]{ return !m_needUpdateAssets; });
        emit assetsUpdated();
    }
}

void ZBackEnd::readTickets()
{
    if(m_tickets.isEmpty() || m_needUpdateTickets)
    {
        m_needUpdateTickets = true;

        QString str = QString("SELECT ticket, dt_open, user, asset, status, description FROM tickets WHERE user = '%1'").arg(m_credentials["nickname"].toString("."));

        m_db.query(str, id_tickets);
  //      std::unique_lock<std::mutex> lock(m_mtxTickets);
  //      m_cvTickets.wait_for(lock, std::chrono::seconds(10), [this]{ return !m_needUpdateTickets; });
        emit ticketsUpdated();
    }
}

void ZBackEnd::readEmojis()
{
    if(g_emoji.size() > 0)
    {
        m_emojis.empty();
        for(const auto& item: g_emoji)
        {
            if(item.size() == 4 && !item[0].isEmpty() && !item[1].isEmpty() && !item[3].isEmpty())
            {
                QJsonObject v;
                v.insert("emoji", item[0]);
                v.insert("html", item[1]);
                v.insert("text", item[3]);
                m_emojis.append(v);
            }
        }
        emit emojisUpdated();
    }
}

void ZBackEnd::readSteps(const QString &ticket)
{
    if(m_steps.isEmpty() || m_needUpdateSteps)
    {
        m_needUpdateSteps = true;

        QString str;

        if(ticket.isEmpty())
            str = QString("SELECT ticket, nickname, status, description FROM steps");
        else
            str = QString("SELECT ticket, nickname, status, description FROM steps WHERE ticket = '%1'").arg(m_credentials["ticket"].toString("."));

        m_db.query(str, id_steps);
        //      std::unique_lock<std::mutex> lock(m_mtxTickets);
        //      m_cvTickets.wait_for(lock, std::chrono::seconds(10), [this]{ return !m_needUpdateTickets; });
        emit stepsUpdated();
    }
}


void ZBackEnd::queried(int id, const QJsonDocument& reply)
{
    qDebug() << "queried: " << id;
    qDebug() << "reply: " << reply;

    switch(id)
    {
    case id_assets:
        if(reply.isArray())
        {
            m_assets = reply.array();
//            std::unique_lock<std::mutex> lock(m_mtxAssets); // Acquire the mutex
            m_needUpdateAssets = false;
//            m_cvAssets.notify_one();
        }
        break;
    case id_tickets:
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
//            std::unique_lock<std::mutex> lock(m_mtxTickets); // Acquire the mutex
            m_needUpdateTickets = false;
//            m_cvTickets.notify_one();
        }
        break;
    case id_steps:
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
            m_needUpdateSteps = false;
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
                m_newuser.append({item["Name"], item["Surname"], item["Email"]});
                m_newasset.append({item["IdAsset"], item["Asset"], item["Os"], "Laptop", item["Deploy"]});
                m_newlink.append({item["IdAsset"], item["Email"]});
            }
            header = false;
        }
        f.close();
    }
    if(m_newuser.length() > 0)
    {
        auto user = m_newuser.takeFirst();
        addUser(user);
    }
    if(m_newasset.length() > 0)
    {
        auto asset = m_newasset.takeFirst();
        addAsset({asset[0], asset[1],  "Laptop", asset[2]}, QDateTime::fromString(asset[3], "dd/MM/yyyy"));
    }
    if(m_newlink.length() > 0)
    {
        auto link = m_newlink.takeFirst();
        addLink(link);
    }
}

