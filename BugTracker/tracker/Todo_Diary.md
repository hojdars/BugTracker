Main todo:
==========

##### Memory representation
* The loaded bugs vector<>   .. **OK-ish, refactor**

##### Important stuff:

* Refactor filter code, kinda tilt mode

* Recognize enum types when parsing bugs
    * Do a table with enums: their column and their value table


* Filter is not modular
    * column names are hard coded
    * line edits are hard coded
    * the field names are hard coded


* Solve all ```query failed``` cases
    * if update query fails, the item seems updated but is not
    * if column / state query fails the columns are not loaded


* ```TreeWidget::OnDoubleClick```
    * [OK] _should open a dialog to edit the bug in memory_
    * [OK] _then push the change into DB_
    * With **locking for concurrent access**


##### Minor stuff:

* Fix tree sorting? I need type sorting for all the columns...


* Abusing signals to do slot's job


* Maybe split ```load_new_database()``` into two methods ```update()``` and ```new_connect()``` ?

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
* [OK] _```load_query_into_memory``` **adds** instead of rewrting which is good but I should be calling ```load_new_memory``` on start_
    * solved by: ```load_new_database()``` does this, load_query_into_memory only adds bugs
* [OK] _Rewrite "Error executing querry" messages_
* [OK] _Filter on the bottom is not filtering_


Garbage:
========
empty!
