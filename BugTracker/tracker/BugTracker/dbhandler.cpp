#include "dbhandler.hpp"

DBHandler::DBHandler()
{
    database_ = QSqlDatabase::addDatabase("QPSQL");

    set_params("postgres","root","localhost","testdb",5432);
}

DBHandler::DBHandler(std::vector<QString> dbparams ,int port)
{
    username_ = dbparams[0];
    password_ = dbparams[1];
    hostname_ = dbparams[2];
    dbname_ = dbparams[3];
    port_ = port;
    database_ = QSqlDatabase::addDatabase("QPSQL");
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

void DBHandler::close()
{
    if(database_.isOpen())
        database_.close();
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
