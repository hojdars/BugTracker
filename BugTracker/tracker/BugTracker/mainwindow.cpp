#include "mainwindow.hpp"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(int*)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);


    // load prestored DB and connect to it
    ui->statusBar->showMessage(datab_inst_.DB_connect());

    // initialize the tree
    ///TODO: Extract this to method
    ui->tree_bugView->setColumnCount(2);
    ui->tree_bugView->clear();
    load_tree_fromDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QMessageBox msg;
    msg.setText("Double clicked " + (item->text(column) ) );
    msg.exec();
}

///TODO: This sould also update settings.ini
void MainWindow::on_actionSettings_triggered()
{
    DBSetDialog dialog(this);
    dialog.exec();

    datab_inst_.set_params(dialog.username(), dialog.password(),
                           dialog.hostname(), dialog.dbname(), dialog.port());

    ui->statusBar->showMessage("OK, database set successfully.");
}

void MainWindow::on_actionConnect_triggered()
{
    ui->statusBar->showMessage(datab_inst_.DB_connect());
}

void MainWindow::on_tree_bugView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->statusBar->showMessage(item->data(0,0).toString());

    //std::unique_ptr<int> a = std::make_unique<int>(5);
     int * a = new int;

     // NOTE THE METATYPE DEFINE!

    item->setData( 0,0x0100, QVariant::fromValue<int*>(a) );
}

void MainWindow::load_tree_fromDB()
{
    QSqlQuery qry;
    if(qry.exec("SELECT * FROM lidi"))
    {
        while(qry.next())
        {
            QTreeWidgetItem * item = new QTreeWidgetItem(ui->tree_bugView);
            item->setText(0,qry.value(0).toString());
            item->setText(1,qry.value(1).toString());
        }
    }
    else
    {
        ui->statusBar->showMessage("Error executing querry.");
    }
}



