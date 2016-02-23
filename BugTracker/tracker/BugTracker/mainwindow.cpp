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
    connect(ui->tree_bugView,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(tree_itemClicked_signal(QTreeWidgetItem*,int)));

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
}

void MainWindow::on_actionConnect_triggered()
{
    ui->statusBar->showMessage(datab_inst_->DB_connect());
}

// Not USED, waiting for onClick
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
