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

#include "dbsetdialog.hpp"
#include "dbhandler.hpp"
#include "dataobject.hpp"

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
    void signalTry(QTreeWidgetItem *item, int column);

private:
    std::unique_ptr<DBHandler> datab_inst_;
    std::unique_ptr<DataObject> bug_data_;
    Ui::MainWindow *ui;

    void initialize_treewidget(); ///TODO: Implement

    void prepare_view_data();
};

#endif // MAINWINDOW_HPP
