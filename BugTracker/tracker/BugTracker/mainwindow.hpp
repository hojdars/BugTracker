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
#include <algorithm>

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

private slots:
    void on_actionSettings_triggered();
    void on_actionConnect_triggered();

    void tree_itemClicked_slot(QTreeWidgetItem *item, int column);
    void tree_itemDoubleClicked_slot(QTreeWidgetItem *item, int column);

    void on_actionAdd_new_bug_triggered();

    void on_button_resetCriteria_clicked();

    void on_buton_filterBugs_clicked();

    void on_actionRefresh_bugs_triggered();

private:
    std::unique_ptr<DBHandler> datab_inst_;
    std::unique_ptr<DataObject> bug_data_;
    Ui::MainWindow *ui;

    void initialize_treewidget();

    void load_new_database();
    bool prepare_view_data();
    void load_query_intoMemory(QString command);
    void load_tree_fromMemory();
    void load_settings(std::vector<QString>& dbparams, int& port, std::vector<QString>& tables);
    std::vector<QString> edit_memoryItem(int item_position);
    void add_edit_newItem();
    QString sqlInsert_fromValues(QStringList values);
};

#endif // MAINWINDOW_HPP
