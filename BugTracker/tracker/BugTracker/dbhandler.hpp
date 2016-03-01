#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include <string>
#include <vector>
#include <fstream>

#include <QString>
#include <QtSql>

class DBHandler
{
public:
    DBHandler();

    DBHandler(std::vector<QString>& dbparams ,int port, std::vector<QString> & tablenames);

    bool is_open(){ return database_.isOpen(); }
    void set_params(QString  username, QString  password, QString  hostname,
                    QString  dbname, int port);
    QString DB_connect();
    QString last_error();
    void close();

    // make private
    QString main_table_name;
    QString state_table_name;
    QString column_table_name;

    std::vector<QString> table_names;

private:
    QString username_;
    QString password_;
    QString hostname_;
    QString dbname_;
    int port_;
    QSqlDatabase database_;


};

#endif // DBHANDLER_HPP
