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
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionSettings_triggered();
    void on_actionConnect_triggered();

    void on_tree_bugView_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    DBHandler datab_inst_;
    Ui::MainWindow *ui;
    void initialize_treewidget(); ///TODO: Implement

};

#endif // MAINWINDOW_HPP
