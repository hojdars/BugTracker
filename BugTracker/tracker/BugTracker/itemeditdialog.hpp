#ifndef ITEMEDITDIALOG_HPP
#define ITEMEDITDIALOG_HPP

#include <QDialog>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <memory>
#include <unordered_map>
#include <set>
#include <QDebug>
#include <vector>

#include "dataobject.hpp"


namespace Ui {
class ItemEditDialog;
}

class my_widget
{
public:
    virtual QWidget * get_ptr() = 0;
    virtual ~my_widget() = default;
    virtual QString get_text() = 0;
};

class my_ComboBox : public my_widget
{
private:
    QComboBox * ptr_;
public:
    my_ComboBox(QWidget * ui, QString value, QStringList&& names )
    {
        ptr_ = new QComboBox(ui);
        names.sort();
        ptr_->addItems(names);
        ptr_->setCurrentIndex(names.indexOf(value,0));
    }
    virtual QWidget * get_ptr()
    {
        return ptr_;
    }

    virtual QString get_text()
    {
        return ptr_->currentText();
    }
};

class my_textEdit : public my_widget
{
private:
    QPlainTextEdit * ptr_;
public:
    my_textEdit(QString text, QWidget * ui)
    {
        ptr_ = new QPlainTextEdit(text,ui);
    }
    virtual QWidget * get_ptr()
    {
        return ptr_;
    }

    virtual QString get_text()
    {
        return ptr_->toPlainText();
    }
};
class my_lineEdit : public my_widget
{
private:
    QLineEdit * ptr_;
public:
    my_lineEdit(QString text, QWidget * ui)
    {
        ptr_ = new QLineEdit(text,ui);
        ptr_->setDragEnabled(true);
    }
    virtual QWidget * get_ptr()
    {
        return ptr_;
    }

    virtual QString get_text()
    {
        return ptr_->text();
    }
};



class ItemEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemEditDialog(QWidget *parent = 0, std::vector<QString> bug_values = std::vector<QString>(),
                            DataObject* data_ptr = 0);

    ~ItemEditDialog();

    std::unique_ptr< std::vector<QLineEdit*> > line_edits_ptr_; // holds the lineEdits so we can access them, note that we do not need deleting the parent does it

    std::vector< my_widget* > widgets_;

    QStringList return_strings(const std::unordered_map<std::string, int>& state_names,
                               const std::set< int>& enum_cols); // gets the strings from the lineEdits

private:
    Ui::ItemEditDialog *ui;

};

#endif // ITEMEDITDIALOG_HPP
