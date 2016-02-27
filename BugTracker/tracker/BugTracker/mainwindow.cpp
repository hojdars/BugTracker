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

    // We clear the tree
    ui->tree_bugView->clear();

    // Connect to the DB
    ui->statusBar->showMessage(datab_inst_->DB_connect());

    // Make new data object - resets the memory
    bug_data_ = std::make_unique<DataObject>();

    // Loading non-bug data from DB
    // if it fails, the method notifies user and we can end
    if(!prepare_view_data())
        return;

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

std::vector<QString> MainWindow::edit_memoryItem(int item_position)
{
    // if we are editing out of range, we don't edit
    if(item_position >= bug_data_->bug_values_.size())
    {
        QMessageBox msg;
        msg.setText("The item you are trying to edit doesn't exist. This edit isn't possible.");
        msg.exec();
        return std::vector<QString>();
    }

    std::vector<QString> backup = bug_data_->bug_values_.at(item_position);

    ItemEditDialog edit_dialog(0,bug_data_->bug_values_.at(item_position));
    edit_dialog.exec();
    if(edit_dialog.result() == QDialog::DialogCode::Accepted)
    {
        QStringList result = edit_dialog.return_strings();
        for(int i = 1; i < bug_data_->column_names_.size(); ++i)
        {
            bug_data_->bug_values_.at(item_position).at(i) = result.at(i-1);
        }
    }
    return backup;
}

QString MainWindow::sqlInsert_fromValues(QStringList values)
{
    QString sql_command = "INSERT INTO lidi (";
    for(auto& col_name : bug_data_->column_names_)
    {

        if( col_name != bug_data_->column_names_.first())
        {
            sql_command += col_name;
            if(col_name != bug_data_->column_names_.last())
                sql_command += + ", ";
        }
    }
    sql_command += ") VALUES (";
    for(auto& val_name : values)
    {
        sql_command += "'" + val_name + "'";
        if(val_name != values.last()) // HACK
            sql_command += ", ";
    }
    sql_command += ");";
    return sql_command;
}

// This needs serial primary key in the database!
void MainWindow::add_edit_newItem()
{
    // Load up column names as initial values for the dialog
    std::vector<QString> initial_textvalues;
    for(auto& it : bug_data_->column_names_)
    {
        initial_textvalues.push_back(it);
    }

    ItemEditDialog edit_dialog(0,initial_textvalues);
    edit_dialog.exec();

    if(edit_dialog.result() != QDialog::DialogCode::Accepted)
        return;

    QStringList values = edit_dialog.return_strings();

    // INSERT command with 'values'
    QString sql_command = sqlInsert_fromValues(values);

    // load querry into memory SELECT 'the new one'
    QSqlQuery query;

    // INSERT
    if(query.exec(sql_command))
        ui->statusBar->showMessage("Insert successful.");
    else
    {
        ui->statusBar->showMessage("Inserting failed.\n" + query.lastError().text());
        return;
    }

    // SELECT new
    load_query_intoMemory("SELECT * FROM lidi WHERE id=(SELECT max(id) FROM lidi )");

    // update tree view from memory
    load_tree_fromMemory();
}

// Editing the item
void MainWindow::tree_itemDoubleClicked_signal(QTreeWidgetItem *item, int column)
{
    size_t item_position = item->data(column,0x0100).toInt();
    QString ID = bug_data_->bug_values_.at(item_position).at(0);

    // LOCK the item
    QSqlQuery lock_transaction;
    if(lock_transaction.exec("BEGIN; SELECT * FROM lidi WHERE id=" + ID + " FOR UPDATE NOWAIT"))
        ui->statusBar->showMessage("Row locked successfully.");
    else
    {
        QMessageBox msg;
        msg.setText("Row is locked, please wait a few moments until someone else unlocks it.\n" + lock_transaction.lastError().text() );
        msg.exec();
        lock_transaction.exec("COMMIT;"); // end transaction
        return;
    }

    // Compare the memory data to the DB data, if it is not the same, we are overwriting someone else's data
    while(lock_transaction.next())
    {
        for(int i = 0; i < bug_data_->column_names_.size(); ++i)
        {
            if(lock_transaction.value(i).toString() != bug_data_->bug_values_.at(item_position).at(i))
            {
                QMessageBox msg;
                msg.setText("This row has been changed by someone else since you last connected to the database. \n Please, reload your database before making changes.");
                msg.exec();
                lock_transaction.exec("COMMIT;"); // end transaction
                return;
            }
        }
    }

    // Get data from dialog and update our memory
    std::vector<QString> backup = edit_memoryItem(item_position);

    if(backup.size() == 0) // the item is out of range, user has been notified, we end
        return;

    // Update the tree view
    load_tree_fromMemory();

    // Update database
    //  UPDATE lidi SET col1=val1,... WHERE ID='itemid'

    QString sqlCommand = "UPDATE lidi SET ";

    int i = 0;
    for(auto& column_name : bug_data_->column_names_)
    {
       if(i != 0)
       {
           sqlCommand += column_name+"='"+bug_data_->bug_values_.at(item_position).at(i) +"'";
           if(i > 0 && i != bug_data_->column_names_.size()-1)
               sqlCommand += ", ";
       }
       i++;
    }

    sqlCommand += " WHERE ID=" + ID;

    QSqlQuery update_querry;
    if(update_querry.exec(sqlCommand + "; COMMIT;"))
        ui->statusBar->showMessage("Database updated successfully.");
    else
    {
        ui->statusBar->showMessage("Error updating item.\n" + update_querry.lastError().text() );
        bug_data_->bug_values_.at(item_position) = backup;
    }
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
        ui->statusBar->showMessage("Error executing query:\n" + query.lastError().text());
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
            // for enum type columns
            if(col_counter == 4)
                item->setText(col_counter,bug_data_->state_names_[ column_string.toInt() - 1]);
            else
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


void MainWindow::initialize_treewidget()
{
    ui->tree_bugView->clear();
    ui->tree_bugView->setColumnCount(bug_data_->column_names_.size());
    ui->tree_bugView->setHeaderLabels(bug_data_->column_names_);
}

bool MainWindow::prepare_view_data()
{
    bool success = true;
    // load collumn names into the "Data" object
    QSqlQuery qry;
    if(qry.exec("SELECT * FROM cols"))
    {
        while(qry.next())
            bug_data_->column_names_.push_back(qry.value(1).toString());
    }
    else
    {
        success = false;
        QMessageBox msg;
        msg.setText("Error loading column names.\n" + qry.lastError().text() );
        msg.exec();
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
            itm->setData(0x0100,qry.value(0));
            ui->list_FilterStateChecks->addItem(itm);
        }
    }
    else
    {
        success = false;
        QMessageBox msg;
        msg.setText("Error loading state names.\n" + qry.lastError().text());
        msg.exec();
    }
    return success;
}

void MainWindow::on_actionAdd_new_bug_triggered()
{
    add_edit_newItem();
}

void MainWindow::on_button_resetCriteria_clicked()
{
    ui->edit_Filter1->setText("");
    ui->edit_Filter2->setText("");
    ui->edit_Filter3->setText("");
    ui->edit_Filter4->setText("");

    for(int i = 0; i < ui->list_FilterStateChecks->count(); ++i)
        ui->list_FilterStateChecks->item(i)->setCheckState(Qt::CheckState::Checked);
}

void MainWindow::on_buton_filterBugs_clicked()
{

    QString sql_command = "SELECT * FROM lidi WHERE";
    QStringList list_lineEdit;

    // We load all lineEdit filters
    if(ui->edit_Filter1->text() != "")
        list_lineEdit.push_back( QString(" position(") +  "'" + ui->edit_Filter1->text() + "'"  + " in name) > 0");

    if(ui->edit_Filter2->text() != "")
        list_lineEdit.push_back( QString(" position(") +  "'" + ui->edit_Filter2->text() + "'"  + " in description) > 0");

    if(ui->edit_Filter3->text() != "")
        list_lineEdit.push_back( QString(" position(") +  "'" + ui->edit_Filter3->text() + "'"  + " in author) > 0");

    if(ui->edit_Filter4->text() != "")
        list_lineEdit.push_back( QString(" id=")  + ui->edit_Filter4->text());

    // We load all checked categories
    QStringList list_checkBox;
    for(int i = 0; i < ui->list_FilterStateChecks->count(); ++i)
    {
        if(ui->list_FilterStateChecks->item(i)->checkState() == Qt::CheckState::Checked)
            list_checkBox.push_back(QString(" importance='") + QString::number(ui->list_FilterStateChecks->item(i)->data(0x0100).toInt()) +"'");
    }

    // we chain all the LineEdit requirements together with 'AND'
    for(int i = 0; i < list_lineEdit.size(); ++i)
    {
        if(i != 0)
            sql_command += " AND";
        sql_command += list_lineEdit[i];
    }

    // If we have any, we add a list of checked categories of the bug, with 'OR'
    // This defaults the behavior to "if nothing is checked, everything is"
    if(list_checkBox.size() > 0)
    {
        if(list_lineEdit.size() != 0)
            sql_command += " AND ";
        sql_command+=" (";

        for(int i = 0; i < list_checkBox.size(); ++i)
        {
            if(i != 0)
                sql_command += " OR";
            sql_command += list_checkBox[i];
        }
        sql_command += ")";
    }

    // we delete all bugs from memory, database remains the same, so we do not change column names and such
    bug_data_->bug_values_.clear();

    // we load selected bugs and update the tree view
    load_query_intoMemory(sql_command);
    load_tree_fromMemory();
}
