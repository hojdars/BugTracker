#include "dbhandler.hpp"

DBHandler::DBHandler()
{
    database_ = QSqlDatabase::addDatabase("QPSQL");

    ///TODO: these should be loaded from .ini
    set_params("postgres","root","localhost","testdb",5432);
}

void DBHandler::set_params(QString username, QString  password, QString  hostname,
                QString  dbname, int port)
{
    username_ = username;
    hostname_ = hostname;
    password_ = password;
    dbname_ = dbname;
    port_ = port;
}


// estabilish a connection to DB
QString DBHandler::DB_connect()
{
    if(database_.isOpen())
        database_.close();

    database_.setHostName(hostname_);
    database_.setDatabaseName(dbname_);
    database_.setPort(port_);

    database_.open(username_,password_);

    if(database_.isOpen())
        return QString("Database opened OK.");
    else
        return database_.lastError().text();

}
