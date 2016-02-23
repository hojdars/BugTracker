Main todo:
==========

##### Memory representation
* The loaded bugs vector<>   .. **OK-ish, refactor**
* [OK] _The labels of collumns_
* [OK] _The states_

##### Loading from DB
* load collumn names for treeView
    * labels below need to change, **labels in preview window   .. POSTPONED**
* [OK] _load state names (like critical, normal, etc.)_
    * [OK] _type enum list in filter needs to change_
* [OK] _load the bugs correctly_
    * [OK] _loaded into memory_
    * [OK] correct things in correct collumns
* [OK] _ ```TreeWidget::OnClick``` - should update the description on the right
* [OK] _```TreeWidget::setSortingEnabled()```  -  Sorting on column's name click_
* [OK] _Settings loading on app launch .. OK_
* ```TreeWidget::OnDoubleClick``` - should open a dialog to edit the bug
* Settings do not update .ini file
* Filter on the bottom is not filtering


* Recognize enum types when parsing bugs
* Concurrent access not solved at all (yet)
* Maybe split ```load_new_database()``` into two methods ```update()``` and ```new_connect()``` ?
* Rewrite "Error executing querry" messages

Garbage:
========

  ```MainWindow::SignalTry()``` is a prototype to be used with ```TreeWidget::OnItemDoubleClick```
