Main todo:
==========

##### Memory representation
* The loaded bugs vector<>   .. **OK-ish, refactor**
* _The labels of collumns     .. OK_
* _The states                 .. OK_

##### Loading from DB
* load collumn names for treeView                             .. OK
    * labels below need to change, **labels in preview window   .. POSTPONED**
* _load state names (like critical, normal, etc.)              .. OK_
    * _type enum list in filter needs to change                .. OK_
* _load the bugs correctly                                      .. OK_
    * _loaded into memory                                      .. OK _
    * _ correct things in correct collumns                      .. OK _


  _ ```TreeWidget::OnClick``` - should update the description on the right .. OK_

  ```TreeWidget::OnDoubleClick``` - should open a dialog to edit the bug

  _```TreeWidget::setSortingEnabled()```  -  Sorting on column's name click .. OK_

  DB is connecting to the preset, not to the .ini

Maybe split ```load_new_database()``` into two methods ```update()``` and ```new_connect()``` ?

  Settings do not update .ini file (or load it's data to show before edit!!)

  Filter on the bottom is not filtering

  Recognize enum types when parsing bugs

  Concurrent access not solved at all (yet)

  Rewrite "Error executing querry" messages

Garbage:
========

  ```MainWindow::SignalTry()``` is a prototype to be used with ```TreeWidget::OnItemDoubleClick```
