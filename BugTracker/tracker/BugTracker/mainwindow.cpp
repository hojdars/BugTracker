#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    // Connect Signals
    connect(ui->tree_bugView,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(tree_itemDoubleClicked_signal(QTreeWidgetItem*,int)));
    connect(ui->tree_bugView,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(tree_itemClicked_signal(QTreeWidgetItem*,int)));

    // Load config.ini
    int port_setting = 5432;
    std::vector<QString> db_settings(4,0); // we need 4 items in the vector
    load_settings(db_settings,port_setting);

    // Make a DB handler
    datab_inst_ = std::make_unique<DBHandler>(db_settings, port_setting);

    // Connect to the DB
    load_new_database();
}

void MainWindow::load_settings(std::vector<QString>& dbparams, int& port)
{
    dbparams[0] = "postgres";
    dbparams[1] = "root";
    dbparams[2] = "localhost";
    dbparams[3] = "testdb";
    port = 5432;

    std::ifstream ifs;
    ifs.open("settings.ini");
    if(!ifs.is_open())
    {
        QMessageBox msg;
        msg.setText("Settings not found, using default.\nPlease set them in the 'Settings' menu.");
        msg.exec();
        return;
    }
    std::string name,pass,host,db;
    ifs >> name >>  pass >>  host >> db >> port;
    dbparams[0] = QString::fromStdString(name);
    dbparams[1] = QString::fromStdString(pass);
    dbparams[2] = QString::fromStdString(host);
    dbparams[3] = QString::fromStdString(db);
    ifs.close();
}

void MainWindow::load_new_database()
{
    // if we are connected, the handler will close the connection
    datab_inst_->close();

    // Connect to the DB
    ui->statusBar->showMessage(datab_inst_->DB_connect());

    // Loading non-bug data from DB
    prepare_view_data();

    // Initialize the tree via "Data"
    initialize_treewidget();

    // Load the bugs into memory
    load_query_intoMemory("SELECT * FROM lidi");

    // Load the bugs into the tree
    load_tree_fromMemory();
}

void MainWindow::tree_itemClicked_signal(QTreeWidgetItem *item, int column)
{
    int item_position =  item->data(column,0x0100).toInt(); // toInt() returns bool, check if okay
    QString text;
    int i = 0;
    for(auto& field : bug_data_->bug_values_[item_position])
    {
        text += "<b>" + bug_data_->column_names_.at(i) + "</b>:" +field + "<br>";
        i++;
    }
    ui->text_bugDesc->setText(text);
}

// Editing the item
void MainWindow::tree_itemDoubleClicked_signal(QTreeWidgetItem *item, int column)
{
    // LOCK the item
    /// ...

    // Get data from dialog and update our memory
    size_t item_position = item->data(column,0x0100).toInt();
    ItemEditDialog tst(0,bug_data_->bug_values_.at(item_position));
    tst.exec();
    if(tst.result() == QDialog::DialogCode::Accepted)
    {
        qDebug() << tst.line_edits_ptr_->at(1)->text();
        for(int i = 0; i < bug_data_->column_names_.size(); ++i)
        {
            bug_data_->bug_values_.at(item_position).at(i) = tst.line_edits_ptr_->at(i)->text();
        }
    }

    // Update the tree view
    load_tree_fromMemory();

    // Update database
    /// ...

    // UNLOCK the item
    /// ...
}


void MainWindow::load_query_intoMemory(QString command)
{
    QSqlQuery query;
    if(query.exec(command))
    {
        while(query.next())
        {
            bug_data_->bug_values_.push_back(std::vector<QString>());
            for(int i = 0; i < bug_data_->column_names_.count(); ++i)
            {
                bug_data_->bug_values_.back().push_back(query.value(i).toString());
            }
        }

        // bug_data_->view_data(); // dumps the data in qDebug() << , for debugging reasons
    }
    else
    {
        ui->statusBar->showMessage("Error executing query " + command);
    }
}

void MainWindow::load_tree_fromMemory()
{
    // clears whatever is left
    ui->tree_bugView->clear();

    // For each bug, we create a new tree item and set all it's properties
    int item_counter = 0;
    for(auto& mem_item : bug_data_->bug_values_)
    {
        // delete not needed, QtTree handles that when we assing him as a parent in the constructor
        QTreeWidgetItem * item = new QTreeWidgetItem(ui->tree_bugView);

        int col_counter = 0;
        for(auto& column_string : mem_item)
        {
            item->setText(col_counter,column_string);
            item->setData( col_counter, 0x0100, QVariant::fromValue<int>(item_counter) );
            col_counter++;
        }
        item_counter++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/// TODO: This sould also update settings.ini
void MainWindow::on_actionSettings_triggered()
{
    DBSetDialog dialog(this);
    dialog.exec();

    datab_inst_->set_params(dialog.username(), dialog.password(),
                           dialog.hostname(), dialog.dbname(), dialog.port());

    ui->statusBar->showMessage("OK, database set successfully.");

    std::fstream ofs;
    ofs.open("settings.ini",std::ios::out );
    ofs << dialog.username().toStdString() << std::endl << dialog.password().toStdString()
        << std::endl << dialog.hostname().toStdString() << std::endl << dialog.dbname().toStdString()
        << std::endl << dialog.port();

    ofs.close();
}

void MainWindow::on_actionConnect_triggered()
{
    load_new_database();
}

// Not USED, waiting for onClick
void MainWindow::signalTry(QTreeWidgetItem *item, int column)
{
  //  ui->statusBar->showMessage(item->data(0,0).toString());

    //std::unique_ptr<int> a = std::make_unique<int>(5);
    //int * a = new int(column);

     // NOTE THE METATYPE DEFINE!
    // add this metatype definition to header
//    Q_DECLARE_METATYPE(int*)

    //item->setData( 0,0x0100, QVariant::fromValue<int*>(a) );

}

void MainWindow::initialize_treewidget()
{
    ui->tree_bugView->setColumnCount(bug_data_->column_names_.size());
    ui->tree_bugView->setHeaderLabels(bug_data_->column_names_);

    ui->tree_bugView->clear();
    //load_tree_fromDB();
}

void MainWindow::prepare_view_data()
{
    bug_data_ = std::make_unique<DataObject>();

    // load collumn names into the "Data" object
    QSqlQuery qry;
    if(qry.exec("SELECT * FROM cols"))
    {
        while(qry.next())
            bug_data_->column_names_.push_back(qry.value(1).toString());
    }
    else
    {
        ui->statusBar->showMessage("Error executing query 2.");
    }


    // load state (critical, etc.) into the "Data" object and update the filter check list
    ui->list_FilterStateChecks->clear();
    if(qry.exec("SELECT * FROM states"))
    {
        bug_data_->state_counts_ = qry.size();
        while(qry.next())
        {
            bug_data_->state_names_.push_back(qry.value(1).toString());
            QListWidgetItem * itm = new QListWidgetItem(qry.value(1).toString()); // no memory leaks here?
            itm->setCheckState(Qt::Unchecked);
            ui->list_FilterStateChecks->addItem(itm);
        }
    }
    else
    {
        ui->statusBar->showMessage("Error executing query 2.");
    }
}
