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

QString DBSetDialog::password() const
{
    return ui->lE_passwd->text();
}

QString DBSetDialog::hostname() const
{
    return ui->lE_hostname->text();
}

QString DBSetDialog::username() const
{
    return ui->lE_name->text();
}

QString DBSetDialog::dbname() const
{
    return ui->lE_dbname->text();
}
int DBSetDialog::port() const
{
    // called only after accept which checks convertibility
    // TODO: not pretty
    QString tmp;
    tmp = ui->lE_port->text();
    bool ok;
    int res = tmp.toInt(&ok,10);
    return res;
}

void DBSetDialog::on_b_accept_clicked()
{
    QString tmp;
    tmp = ui->lE_port->text();
    bool ok;
    int res = tmp.toInt(&ok,10);
    if(ok)
    {
        accept();
    }
    else
    {
        reject();
    }
}
