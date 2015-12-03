#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <dbsetdialog.hpp>
#include <QDebug>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void connectDB(QString username, QString password, QString hostname, int port, QString dbname);
    QString printQuery(QString qs_query);
    QSqlDatabase database; // is this pretty?
    ~MainWindow();

private slots:
    void on_actionNew_connection_triggered();

    void on_b_setDB_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
