#ifndef DBSETDIALOG_HPP
#define DBSETDIALOG_HPP

#include <QDialog>

namespace Ui {
class DBSetDialog;
}

class DBSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBSetDialog(QWidget *parent = 0);
    QString dbname() const;
    QString username() const;
    QString password() const;
    QString hostname() const;
    int port() const;
    ~DBSetDialog();

private slots:
    void on_b_accept_clicked();

private:
    Ui::DBSetDialog *ui;
};

#endif // DBSETDIALOG_HPP
