#include "itemeditdialog.hpp"
#include "ui_itemeditdialog.h"

ItemEditDialog::ItemEditDialog(QWidget *parent, std::vector<QString> bug_values,
                               DataObject* data_ptr) :
    QDialog(parent),
    ui(new Ui::ItemEditDialog)
{
    ui->setupUi(this);

    // we make as many line edits as we have columns
    line_edits_ptr_ = std::make_unique<std::vector<QLineEdit*>  >();
    for(size_t i = 1; i < bug_values.size(); i++)
    {
        auto is_enum = data_ptr->enum_cols.find(i);
        if(is_enum != data_ptr->enum_cols.end())
        {
            // for implementing more enums, change the 1 to meaningfull
            widgets_.push_back(new my_ComboBox(this,bug_values[i],data_ptr->return_states_atIndex(1)));
        }
        else
            widgets_.push_back(new my_lineEdit(bug_values[i],this));
    }

    for(int i = 0; i < widgets_.size(); ++i)
    {
        ui->lay_Main->addWidget(widgets_[i]->get_ptr());
    }
}

QStringList ItemEditDialog::return_strings(const std::unordered_map<std::string, int>& state_names,
                           const std::set< int>& enum_cols) // gets the strings from the lineEdits
{
    QStringList ret;

    int i = 1;
    for(auto it : widgets_)
    {
        auto find = enum_cols.find(i);
        QString ret_val;
        if(find != enum_cols.end()) // we have an enum column
        {
            QString str = it->get_text();
            std::string std_str = str.toStdString();

            auto find_usr_input = state_names.find(std_str);
            if(find_usr_input != state_names.end())
            {
                ret_val = QString::number(state_names.at(std_str));
            }
            else // user wrote something invalid
            {
                ret.clear();
                return ret;
            }
        }
        else
        {
            ret_val = it->get_text();
        }

        ret.push_back(ret_val);
        i++;
    }

    return ret;
}

ItemEditDialog::~ItemEditDialog()
{
    delete ui;
}
