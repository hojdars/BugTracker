#ifndef DATAOBJECT_HPP
#define DATAOBJECT_HPP

#include <QString>
#include <QStringList>
#include <vector>

class DataObject
{
public:
    int state_counts_;

    QStringList column_names_;
    std::vector<QString> state_names_;

    DataObject() = default;

    std::vector< std::vector<QString> > bug_values_; // is this sufficient?
    int bug_count_;
};

#endif // DATAOBJECT_HPP
