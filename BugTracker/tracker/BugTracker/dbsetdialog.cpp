#include "dbsetdialog.hpp"
#include "ui_dbsetdialog.h"

DBSetDialog::DBSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBSetDialog)
{
    ui->setupUi(this);
    std::string name,pass,host,db;
    int port;

    // load connection settings from 'settings.ini'
    name = "postgres";
    pass = "root";
    host = "localhost";
    db = "testdb";
    port = 5432;

    std::ifstream ifs;
    ifs.open("settings.ini");

    if(ifs.is_open())
    {
        ifs >> name >>  pass >>  host >> db >> port;
    }
    ifs.close();

    ui->le_username->setText(QString::fromStdString(name));
    ui->le_pass->setText(QString::fromStdString(pass));
    ui->le_hostaname->setText(QString::fromStdString(host));
    ui->le_dbname->setText(QString::fromStdString(db));
    ui->le_port->setText(QString::number(port));
}

DBSetDialog::~DBSetDialog()
{
    delete ui;
}

void DBSetDialog::on_but_set_clicked()
{
    QString tmp;
    tmp = ui->le_port->text();
    bool ok;
    tmp.toInt(&ok,10);
    if(ok)
    {
        accept();
    }
    else
    {
        reject();
    }
}

QString DBSetDialog::password() const
{
    return ui->le_pass->text();
}

QString DBSetDialog::hostname() const
{
    return ui->le_hostaname->text();
}

QString DBSetDialog::username() const
{
    return ui->le_username->text();
}

QString DBSetDialog::dbname() const
{
    return ui->le_dbname->text();
}
int DBSetDialog::port() const
{
    // called only after accept which checks convertibility
    QString tmp;
    tmp = ui->le_port->text();
    bool ok;
    int res = tmp.toInt(&ok,10);
    return res;
}
