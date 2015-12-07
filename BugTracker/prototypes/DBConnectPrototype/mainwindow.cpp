#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    database = QSqlDatabase::addDatabase("QPSQL");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // TODO... does this work?
    if(database.isOpen()) { database.close(); }

    delete ui;
}

// Send a query and return a QString with results
QString MainWindow::printQuery(QString qs_query)
{
    //TODO return not print
    QSqlQuery qry;
    QString out = "";
    if(qry.exec(qs_query))
    {
        while(qry.next())
        {
            out += qry.value(0).toString() + " " + qry.value(1).toString() +"\n";
        }
    }
    else
    {
        out = "Error: " + database.lastError().text();
    }
    return out;
}

// estabilish a connection to DB
void MainWindow::connectDB(QString username, QString password, QString hostname,
               int port, QString dbname )
{
    if(database.isOpen()) { database.close(); }

    database.setHostName(hostname);
    database.setDatabaseName(dbname);
    database.setPort(port);
    bool ok = database.open(username,password);

    if(ok)
    {
        qDebug() << "DB connected.";
        ui->tB_output->setText(printQuery("SELECT * FROM lidi"));
    }
    else
    {
        qDebug() << "DB FAILED: " << database.lastError();
        ui->tB_output->setText("NOPE");
    }
}

void MainWindow::on_b_setDB_clicked()
{
    DBSetDialog dialog(this);

    QString textOut;
    if(dialog.exec() == QDialog::Accepted)
    {
        connectDB(dialog.username(),dialog.password(), dialog.hostname(),
                  dialog.port(), dialog.dbname());
    }
    else
    {
        textOut = "Fill the dialog if you want to connect.";
    }
}

void MainWindow::on_actionNew_connection_triggered()
{
    on_b_setDB_clicked();
}
