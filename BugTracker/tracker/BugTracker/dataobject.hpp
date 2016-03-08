#ifndef DATAOBJECT_HPP
#define DATAOBJECT_HPP

#include <QString>
#include <QStringList>
#include <vector>
#include <unordered_map>
#include <QDebug>

class DataObject
{
public:
    QStringList column_names_;

    std::unordered_map<int, QString> state_names_; // ID -> enumerator names
    std::unordered_map<std::string, int> rev_state_names_; // enum name -> ID

    DataObject() = default;

    std::vector< std::vector<QString> > bug_values_; // the values of the bugs
    int bug_count_;

    void view_data(); // debug function to view the held data
};

#endif // DATAOBJECT_HPP
