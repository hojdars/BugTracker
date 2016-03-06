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
    int state_counts_;

    QStringList column_names_;
    //std::vector<QString> state_names_;
    std::unordered_map<int, QString> state_names_;
    std::unordered_map<std::string, int> rev_state_names_;

    DataObject() = default;

    std::vector< std::vector<QString> > bug_values_; // is this sufficient?
    int bug_count_;

    void view_data();
};

#endif // DATAOBJECT_HPP
