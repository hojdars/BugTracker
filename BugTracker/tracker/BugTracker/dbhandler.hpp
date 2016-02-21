#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include <string>
#include <QString>
#include <fstream>
#include <QtSql>

class DBHandler
{
public:
    DBHandler();
    bool is_open(){ return database_.isOpen(); }
    void set_params(QString  username, QString  password, QString  hostname,
                    QString  dbname, int port);
    QString DB_connect();

private:
    QString  username_;
    QString  hostname_;
    QString  password_;
    QString  dbname_;
    int port_;
    QSqlDatabase database_;
};

#endif // DBHANDLER_HPP
