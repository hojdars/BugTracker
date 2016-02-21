#include "mainwindow.hpp"
#include "ui_mainwindow.h"

Q_DECLARE_METATYPE(int*)


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    // Connect Signals
    connect(ui->tree_bugView,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(signalTry(QTreeWidgetItem*,int)));

    // Load prestored DB and connect to it
    // Load config.ini
    /// ...

    // Connect to the DB
    datab_inst_ = std::make_unique<DBHandler>();
    ui->statusBar->showMessage(datab_inst_->DB_connect());

    // Loading non-bug data from DB
    prepare_view_data();

    // Initialize the tree via "Data"
    initialize_treewidget();

    // Load the bugs into memory
    /// ...
    load_query_intoMemory("SELECT * FROM lidi");

    // Load the bugs into the tree
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
            bug_data_->bug_values_.back().push_back(query.value(0).toString());
            bug_data_->bug_values_.back().push_back(query.value(1).toString());
            bug_data_->bug_values_.back().push_back(query.value(2).toString());
            bug_data_->bug_values_.back().push_back(query.value(3).toString());
            bug_data_->bug_values_.back().push_back(query.value(4).toString());
            bug_data_->bug_values_.back().push_back(query.value(5).toString());
        }
    }
    else
    {
        ui->statusBar->showMessage("Error executing query " + command);
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
}

void MainWindow::on_actionConnect_triggered()
{
    ui->statusBar->showMessage(datab_inst_->DB_connect());
}

// NOT USED CURRENTLY AT ALL
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
            item->setText(2,qry.value(2).toString());
            item->setText(3,qry.value(3).toString());
            item->setText(4,qry.value(4).toString());
            item->setText(5,qry.value(5).toString());
        }
    }
    else
    {
        ui->statusBar->showMessage("Error executing query.");
    }
}

void MainWindow::signalTry(QTreeWidgetItem *item, int column)
{
    ui->statusBar->showMessage(item->data(0,0).toString());

    //std::unique_ptr<int> a = std::make_unique<int>(5);
     int * a = new int(column);

     // NOTE THE METATYPE DEFINE!

    item->setData( 0,0x0100, QVariant::fromValue<int*>(a) );
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
