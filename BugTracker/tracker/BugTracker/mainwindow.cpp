#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    // Connect Signals
    connect(ui->tree_bugView,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(tree_itemDoubleClicked_slot(QTreeWidgetItem*,int)));
    connect(ui->tree_bugView,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(tree_itemClicked_slot(QTreeWidgetItem*,int)));

    //datab_inst_ = nullptr;

    // Load config.ini
    bool ok_state = initialize_DB();

    if(ok_state)
    {
        // Connect to the DB and do everything associated with it - erasing memory, loading new bugs, updating the TreeWidget
        load_new_database();
    }
}

bool MainWindow::initialize_DB()
{
    int port_setting = 5432;
    std::vector<QString> db_settings(4,0); // we need 4 items in the vector - username, password, database name, host name
    bool ok_state = load_settings(db_settings,port_setting);

    if(ok_state)
    {
        // Make a DB handler
        datab_inst_ = std::make_unique<DBHandler>(db_settings, port_setting);
    }
    return ok_state;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Loading settings
bool MainWindow::load_settings(std::vector<QString>& dbparams, int& port)
{
    // load connection settings from 'settings.ini'
    dbparams[0] = "postgres";
    dbparams[1] = "root";
    dbparams[2] = "localhost";
    dbparams[3] = "testdb";
    port = 5432;

    std::ifstream ifs;
    ifs.open("settings.ini");
    std::string name,pass,host,db;

    if(!ifs.is_open())
    {
        QMessageBox msg;
        msg.setText("Settings not found, using default.\nPlease set them in the 'Settings' menu.");
        msg.exec();
    }
    else
    {
        ifs >> name >>  pass >>  host >> db >> port;
    }
    dbparams[0] = QString::fromStdString(name);
    dbparams[1] = QString::fromStdString(pass);
    dbparams[2] = QString::fromStdString(host);
    dbparams[3] = QString::fromStdString(db);
    ifs.close();
    return true;
}

// Loading new database
void MainWindow::load_new_database()
{
    // We clear the previous DB Handler, this connection is dead
    if(datab_inst_ != nullptr)
    {
        datab_inst_->close();
        datab_inst_ = nullptr;
    }

    bool ok_state = initialize_DB();
    if(!ok_state)
    {
        QMessageBox msg;
        msg.setText("Settings not loaded correctly.\n Please fix any remaining issues before you can connect to the DB.");
        msg.exec();
        return;
    }

    // We clear the tree
    ui->tree_bugView->clear();

    // Connect to the DB
    ui->statusBar->showMessage(datab_inst_->DB_connect());

    // Make new data object - resets the memory
    bug_data_ = std::make_unique<DataObject>();

    // Loading non-bug data from DB
    // if it fails, the method notifies the user and we can end
    if(!prepare_view_data())
        return;

    // Initialize the tree via "Data"
    initialize_treewidget();

    // Load the bugs into memory
    load_query_intoMemory("SELECT * FROM " + datab_inst_->table_names.at(0));

    // Load the bugs into the tree
    load_tree_fromMemory();
}

// Loading DB metadata
bool MainWindow::prepare_view_data()
{
    bool success = true;
    QString all_errors = "";

    // load collumn names into the "Data" object
    QSqlQuery qry;
    if(qry.exec("SELECT * FROM " + datab_inst_->table_names.at(2)))
    {
        while(qry.next())
            bug_data_->column_names_.push_back(qry.value(1).toString());
    }
    else
    {
        success = false;
        all_errors += "Error loading column names.\n";
    }

    // load set with enumerator columns
    bug_data_->enum_cols.insert(4);
    bug_data_->enum_cols.insert(7);

    // load state (critical, etc.) into the "Data" object and update the filter check list
    ui->list_FilterStateChecks->clear();
    if(qry.exec("SELECT * FROM " + datab_inst_->table_names.at(1)))
    {
        while(qry.next())
        {
            bug_data_->state_names_.insert({qry.value(0).toInt(),
                                     qry.value(1).toString() });

            bug_data_->rev_state_names_.insert({ qry.value(1).toString().toStdString() , qry.value(0).toInt()});

            QListWidgetItem * itm = new QListWidgetItem(qry.value(1).toString()); // no memory leaks here?
            itm->setCheckState(Qt::Checked);
            itm->setData(0x0100,qry.value(0));
            ui->list_FilterStateChecks->addItem(itm);
        }
    }
    else
    {
        success = false;
        all_errors += "Error loading state names.\n";
    }
    if(!success)
    {
        QMessageBox msg;
        msg.setText(all_errors + datab_inst_->last_error());
        msg.exec();
    }
    return success;
}

// Clearing the TreeWidget
void MainWindow::initialize_treewidget()
{
    ui->tree_bugView->clear();
    ui->tree_bugView->setColumnCount(bug_data_->column_names_.size());
    ui->tree_bugView->setHeaderLabels(bug_data_->column_names_);
}

// Loading bugs into memory, adding up, not erasing beforehand!
void MainWindow::load_query_intoMemory(QString command)
{
    QSqlQuery query;
    if(query.exec(command))
    {
        while(query.next())
        {
            // careful here, push_back() into the original!
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
        ui->statusBar->showMessage("Error executing query:\n" + datab_inst_->last_error());
    }
}

// Loading our memory-stored bugs into the TreeView
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

            auto is_enum = bug_data_->enum_cols.find(col_counter);
            if(is_enum != bug_data_->enum_cols.end())
                item->setText(col_counter, bug_data_->state_names_[column_string.toInt()]);
            else
                item->setText(col_counter,column_string);

            item->setData( col_counter, 0x0100, QVariant::fromValue<int>(item_counter) );
            col_counter++;
        }
        item_counter++;
    }
}


/* -- Menu items -- */

void MainWindow::on_actionAdd_new_bug_triggered()
{
    add_new_item();
}
void MainWindow::on_actionSettings_triggered()
{
    // Runs a dialog to update the DB settings
    DBSetDialog dialog(this);
    dialog.exec();

    if(dialog.result() != QDialog::DialogCode::Accepted)
    {
        ui->statusBar->showMessage("Settings not set. No data updated.");
        return;
    }

    // Writes it to 'settings.ini' file
    std::fstream ofs;
    ofs.open("settings.ini",std::ios::out );
    ofs << dialog.username().toStdString() << std::endl << dialog.password().toStdString()
        << std::endl << dialog.hostname().toStdString() << std::endl << dialog.dbname().toStdString()
        << std::endl << dialog.port();

    ofs.close();
    ui->statusBar->showMessage("OK, database connection set successfully.");

    /*
    if(datab_inst_ != nullptr)
    {
        // Updates the DB parameters.
        datab_inst_->set_params(dialog.username(), dialog.password(),
                               dialog.hostname(), dialog.dbname(), dialog.port());
    }*/
}
void MainWindow::on_actionConnect_triggered()
{
    load_new_database();
}
void MainWindow::on_actionRefresh_bugs_triggered()
{
    load_new_database();
}

/* -- Event handling methods -- */

// Updating item view on right side when item is clicked
void MainWindow::tree_itemClicked_slot(QTreeWidgetItem *item, int column)
{
    int item_position =  item->data(column,0x0100).toInt(); // toInt() returns bool, check if okay
    QString text;
    int i = 0;

    // roughly formates with HTML and shows text in description box
    for(auto& field : bug_data_->bug_values_[item_position])
    {
        // enumerator problems
        auto is_enum = bug_data_->enum_cols.find(i);
        if(is_enum == bug_data_->enum_cols.end())
            text += "<b>" + bug_data_->column_names_.at(i) + "</b>:<br>" +field + "<br><br>";
        else
            text += "<b>" + bug_data_->column_names_.at(i) + "</b>:<br>" +bug_data_->state_names_.at(field.toInt()) + "<br><br>";

        i++;
    }
    ui->text_bugDesc->setText(text);
}

// Adding new item
// Creates a string representation of INSERT command into DB
// This method needs 'serial' primary key in the database!
QString MainWindow::sqlInsert_fromValues(QStringList values)
{
    QString sql_command = "INSERT INTO " + datab_inst_->table_names.at(0) + " (";
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

    for(auto it = values.begin(); it != values.end(); ++it)
    {
        sql_command += "'" + *it + "'";
        if(it != (values.end()-1))
        {
            sql_command += ", ";
        }
    }
    sql_command += ");";
    return sql_command;
}
void MainWindow::add_new_item()
{
    // Load up column names as initial values for the dialog
    std::vector<QString> initial_textvalues;
    for(auto& it : bug_data_->column_names_)
    {
        initial_textvalues.push_back(it);
    }

    ItemEditDialog edit_dialog(0,initial_textvalues, bug_data_.get());
    edit_dialog.exec();

    if(edit_dialog.result() != QDialog::DialogCode::Accepted)
    {
        ui->statusBar->showMessage("Item adding canceled.");
        return;
    }

    QStringList values = edit_dialog.return_strings(bug_data_->rev_state_names_,bug_data_->enum_cols);
    if(values.empty())
    {
        QMessageBox msg;
        msg.setText("Invalid data set, aborting the insert.");
        msg.exec();
        return;
    }


    // INSERT command with 'values'
    QString sql_command = sqlInsert_fromValues(values);

    // load querry into memory SELECT 'the new one'
    QSqlQuery query;

    // INSERT
    if(query.exec(sql_command))
        ui->statusBar->showMessage("Insert successful.");
    else
    {
        QMessageBox msg;
        msg.setText("Inserting failed.\n" + datab_inst_->last_error() + QString("\n") + query.lastError().text());
        msg.exec();
        return;
    }

    // SELECT new
    load_query_intoMemory("SELECT * FROM "+ datab_inst_->table_names.at(0) +" WHERE id=(SELECT max(id) FROM "+ datab_inst_->table_names.at(0) +" )");

    // update tree view from memory
    load_tree_fromMemory();
}

// Editing an existing item via double clicking
std::vector<QString> MainWindow::edit_memoryItem(size_t item_position, int& code)
{
    // if we are editing out of range, we don't edit
    if(item_position >= bug_data_->bug_values_.size())
    {
        QMessageBox msg;
        msg.setText("The item you are trying to edit doesn't exist. This edit isn't possible.");
        msg.exec();
        return std::vector<QString>();
    }

    // we make a backup of the row that is being edited
    std::vector<QString> backup = bug_data_->bug_values_.at(item_position);

    // we create a vector of strings to pass our dialog to initialize it's lineEdits with
    std::vector<QString> dialog_parameter(bug_data_->bug_values_.at(item_position).size());
    for(size_t i = 0; i < dialog_parameter.size(); i++)
    {
        // enumarators problems
        auto is_enum = bug_data_->enum_cols.find(i);
        if(is_enum != bug_data_->enum_cols.end())
            dialog_parameter[i] = bug_data_->state_names_[bug_data_->bug_values_.at(item_position).at(i).toInt()];
        else
            dialog_parameter[i] = bug_data_->bug_values_.at(item_position).at(i);
    }

    // creating and calling the edit dialog
    ItemEditDialog edit_dialog(0,dialog_parameter, bug_data_.get());
    edit_dialog.exec();
    code = edit_dialog.result();

    // if accepted we modify our memory data
    if(edit_dialog.result() == QDialog::DialogCode::Accepted)
    {
        QStringList result = edit_dialog.return_strings(bug_data_->rev_state_names_,bug_data_->enum_cols);
        if(result.empty())
        {
            code = 2; // we signal that user didn't input valid data so the next method ends
            return backup; // invalid data set
        }

        for(int i = 1; i < bug_data_->column_names_.size(); ++i)
        {
            bug_data_->bug_values_.at(item_position).at(i) = result.at(i-1);
        }
    }
    else
    {
        code = 99;
    }

    // we return the backed-up row, in case od Database failure
    return backup;
}
void MainWindow::tree_itemDoubleClicked_slot(QTreeWidgetItem *item, int column)
{
    size_t item_position = item->data(column,0x0100).toInt();
    QString ID = bug_data_->bug_values_.at(item_position).at(0);

    // LOCK the item
    QSqlQuery lock_transaction;
    if(lock_transaction.exec("BEGIN; SELECT * FROM " + datab_inst_->table_names.at(0) +" WHERE id=" + ID + " FOR UPDATE NOWAIT"))
        ui->statusBar->showMessage("Row locked successfully.");
    else
    {
        QMessageBox msg;
        msg.setText("Row is locked, please wait a few moments until someone else unlocks it.\n" + datab_inst_->last_error() );
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
    int return_code;
    std::vector<QString> backup = edit_memoryItem(item_position, return_code);
    if(return_code != 1)
    {
        if(return_code == 2)
            ui->statusBar->showMessage("Invalid data set. Please set valid data.");
        else if (return_code == 99)
            ui->statusBar->showMessage("Item not updated.");

        QSqlQuery end_transaction;
        end_transaction.exec("ROLLBACK");

        for(int i = 1; i < bug_data_->column_names_.size(); ++i)
        {
            bug_data_->bug_values_.at(item_position).at(i) = backup.at(i);
        }
        load_tree_fromMemory();
        return;
    }

    if(backup.size() == 0) // the item is out of range, user has been notified, we end
        return;

    // Update the tree view
    load_tree_fromMemory();

    // Update database
    //  UPDATE 'main table' SET col1=val1,... WHERE ID='itemid'
    QString sqlCommand = "UPDATE "+ datab_inst_->table_names.at(0) +" SET ";

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

    // We run the update querry and COMMIT the transaction to REMOVE THE LOCK
    QSqlQuery update_querry;
    if(update_querry.exec(sqlCommand + "; COMMIT;"))
        ui->statusBar->showMessage("Database updated successfully.");
    else // if the querry fails because user input the wrong data, we rollback, announce the error and return
    {
        QMessageBox msg;
        msg.setText("Error updating item.\n" + datab_inst_->last_error() + QString("\n") + update_querry.lastError().text());
        msg.exec();
        lock_transaction.exec("ROLLBACK");

        for(int i = 1; i < bug_data_->column_names_.size(); ++i)
        {
            bug_data_->bug_values_.at(item_position).at(i) = backup.at(i);
        }
        load_tree_fromMemory();
        return;
    }
}

/* -- Filters -- */

// Reseting all the items in the filter
void MainWindow::on_button_resetCriteria_clicked()
{
    // Erase all the text from LineEdits
    ui->edit_Filter1->setText("");
    ui->edit_Filter2->setText("");
    ui->edit_Filter3->setText("");
    ui->edit_Filter4->setText("");

    // Check all the states
    for(int i = 0; i < ui->list_FilterStateChecks->count(); ++i)
        ui->list_FilterStateChecks->item(i)->setCheckState(Qt::CheckState::Checked);
}

// Requesting only bugs that satisfy the criteria
void MainWindow::on_buton_filterBugs_clicked()
{
    // We make a huge SELECT SQL query:

    QString sql_command = "SELECT * FROM "+ datab_inst_->table_names.at(0) +" WHERE";
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



void MainWindow::on_actionExit_triggered()
{
    exit(0);
}
