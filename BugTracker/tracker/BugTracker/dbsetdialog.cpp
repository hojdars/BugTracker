#include "dbsetdialog.hpp"
#include "ui_dbsetdialog.h"

DBSetDialog::DBSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBSetDialog)
{
    ui->setupUi(this);
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
    // TODO: not pretty
    QString tmp;
    tmp = ui->le_port->text();
    bool ok;
    int res = tmp.toInt(&ok,10);
    return res;
}
