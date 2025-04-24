// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DATABASE_H
#define DATABASE_H

#include "include/mysql.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

MYSQL *g_sql = 0l;

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("liprandi.com");
    db.setUserName("liprandi_root");
    db.setPassword("0713D0504l");
    db.setDatabaseName("liprandi_ticket");
    if (!db.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                        "This example needs SQLite support. Please read "
                        "the Qt SQL driver documentation for information how "
                        "to build it.\n\n"
                        "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;

    QString str = QString("insert into users values(1, 'alberto.bosco', 'Bosco', 'Alberto', 'alberto.bosco@cam-carmagnola.com', %1)")
                      .arg(QCryptographicHash::hash("C4mb14m1", QCryptographicHash::Sha256));
    query.exec(str);

    return true;


    if(!g_sql)
    {

    }
    try
    {
        // Establish a MySQL connection
        if(!mysql_real_connect(
                m_sql,
                host.data(), user.data(),
                password.data(), database.data(),
                port, nullptr, 0))
        {
            errdb
        }
    }
    catch (char *e)
    {
        qDebug() <<  e;
        return false;
    }

    // Execute a sql statement
    query("SHOW TABLES");

    return true;


}

#endif


