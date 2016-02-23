#include "dataobject.hpp"

// debug code for viewing the data in memory
void DataObject::view_data()
{
    for(auto& it1 : bug_values_)
    {
        for (auto& it : it1)
        {

            qDebug() << it;
        }
        qDebug() << "\n";
    }
}
