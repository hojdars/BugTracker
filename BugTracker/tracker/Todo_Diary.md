Main todo:
==========

##### Memory representation
* The loaded bugs vector<>   .. **OK-ish, refactor**

##### Loading from DB
* load collumn names for treeView
    * labels below need to change, **labels in preview window   .. POSTPONED**


* ```TreeWidget::OnDoubleClick```
    * [OK] _should open a dialog to edit the bug in memory_
    * [OK] _then push the change into DB_
    * With **locking for concurrent access**

* Make a execute SQL querry function with echoing into status bar

* ```load_query_into_memory``` **adds** instead of rewrting which is good but I should be calling ```load_new_memory``` on start

* Filter on the bottom is not filtering

* Recognize enum types when parsing bugs

* Maybe split ```load_new_database()``` into two methods ```update()``` and ```new_connect()``` ?
* Rewrite "Error executing querry" messages
* Do a table with enums: their column and their value table
* Memory leak in lineEdits on EditItemDialog maybe?

Done
====

* [OK] _The labels of collumns_
* [OK] _The states_


* [OK] _load state names (like critical, normal, etc.)_
    * [OK] _type enum list in filter needs to change_
* [OK] _load the bugs correctly_
    * [OK] _loaded into memory_
    * [OK] correct things in correct collumns
* [OK] _```TreeWidget::OnClick``` - should update the description on the right_
* [OK] _```TreeWidget::setSortingEnabled()```  -  Sorting on column's name click_
* [OK] _Settings loading on app launch .. OK_
* [OK] _Settings do not update .ini file_
* [OK] ```MainWindow::SignalTry()``` is a prototype to be used with ```TreeWidget::OnItemDoubleClick()```
* [OK] _Add a "Add New Bug" button_


Garbage:
========
empty!
