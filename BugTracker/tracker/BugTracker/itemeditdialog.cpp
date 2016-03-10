#include "itemeditdialog.hpp"
#include "ui_itemeditdialog.h"

ItemEditDialog::ItemEditDialog(QWidget *parent, std::vector<QString> bug_values) :
    QDialog(parent),
    ui(new Ui::ItemEditDialog)
{
    ui->setupUi(this);

    // we make as many line edits as we have columns
    line_edits_ptr_ = std::make_unique<std::vector<QLineEdit*>  >();
    for(size_t i = 1; i < bug_values.size(); i++)
    {
        line_edits_ptr_->push_back(new QLineEdit(bug_values[i],this));
    }

    // Then add them into the GUI
    for(auto it : *line_edits_ptr_)
    {
        ui->lay_Main->addWidget(it);
    }

}

QStringList ItemEditDialog::return_strings(const std::unordered_map<std::string, int>& state_names,
                           const std::set< int>& enum_cols) // gets the strings from the lineEdits
{
    QStringList ret;

    int i = 1;
    for(auto it : *line_edits_ptr_)
    {
        auto find = enum_cols.find(i);
        // we have an enum column

        QString ret_val;
        if(find != enum_cols.end())
        {
            qDebug() << "found";
            QString str = it->text();
            std::string std_str = str.toStdString();

            auto find_usr_input = state_names.find(std_str);
            if(find_usr_input != state_names.end())
            {
                ret_val = QString::number(state_names.at(std_str));
            }
            else
            {
                // user wrote something fucked up
                ret.clear();
                return ret;
            }
        }
        else
        {
            ret_val = it->text();
        }

        ret.push_back(ret_val);
        qDebug() << ret_val << "\n";
        i++;
    }

    return ret;
}

ItemEditDialog::~ItemEditDialog()
{
    delete ui;
}
