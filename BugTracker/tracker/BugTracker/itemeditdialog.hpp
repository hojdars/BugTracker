#ifndef ITEMEDITDIALOG_HPP
#define ITEMEDITDIALOG_HPP

#include <QDialog>
#include <QLineEdit>
#include <memory>
#include <QDebug>

#include <vector>

namespace Ui {
class ItemEditDialog;
}

class ItemEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemEditDialog(QWidget *parent = 0, std::vector<QString> bug_values = std::vector<QString>());
    ~ItemEditDialog();
    std::unique_ptr< std::vector<QLineEdit*> > line_edits_ptr_;
    QStringList return_strings();

private:
    Ui::ItemEditDialog *ui;

};

#endif // ITEMEDITDIALOG_HPP
