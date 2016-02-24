#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMessageBox>
#include <QtSql>
#include <QString>
#include <memory>
#include <fstream>
#include <iostream>

#include "dbsetdialog.hpp"
#include "dbhandler.hpp"
#include "dataobject.hpp"
#include "itemeditdialog.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void load_tree_fromDB();

private slots:
    void on_actionSettings_triggered();
    void on_actionConnect_triggered();

    void tree_itemClicked_signal(QTreeWidgetItem *item, int column);
    void tree_itemDoubleClicked_signal(QTreeWidgetItem *item, int column);

    void on_actionAdd_new_bug_triggered();

private:
    std::unique_ptr<DBHandler> datab_inst_;
    std::unique_ptr<DataObject> bug_data_;
    Ui::MainWindow *ui;

    void initialize_treewidget();

    void load_new_database();
    void prepare_view_data();
    void load_query_intoMemory(QString command);
    void load_tree_fromMemory();
    void load_settings(std::vector<QString>& dbparams, int& port);
    void edit_memoryItem(int item_position);
    void add_edit_newItem();

};

#endif // MAINWINDOW_HPP
