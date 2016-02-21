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

    ///Bugs here
    /// ?
};

#endif // DATAOBJECT_HPP
