#ifndef DATAOBJECT_HPP
#define DATAOBJECT_HPP

#include <QString>
#include <QStringList>
#include <vector>
#include <unordered_map>
#include <set>
#include <QDebug>

class DataObject
{
public:
    QStringList column_names_;

    // for multiple enumerator columns implement these into vector
    // and implement correctly return_states_atIndex(int i)
    // and fix all the references to that function
    std::unordered_map<int, QString> state_names_; // ID -> enumerator names
    std::unordered_map<std::string, int> rev_state_names_; // enum name -> ID

    DataObject() = default;

    std::vector< std::vector<QString> > bug_values_; // the values of the bugs
    std::set< int> enum_cols;
    int bug_count_;

    QStringList return_states_atIndex(int i);

    void view_data(); // debug function to view the held data
};

#endif // DATAOBJECT_HPP
