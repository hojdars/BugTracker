Main todo:
==========

##### Important stuff:

* replace unordered_map with unordered_map of unordered_maps for enumerators :D

* table names not loading

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
* [OK] _Solve all ```query failed``` cases_
    * [OK] _if update query fails, the item seems updated but is not_
    * [OK] _if column / state query fails the columns are not loaded_
* [OK] _ Refactor filter code_
* [OK] ```TreeWidget::OnDoubleClick```
    * [OK] _should open a dialog to edit the bug in memory_
    * [OK] _then push the change into DB_
    * [OK] _With locking for concurrent access_
* [OK] _refresh button_
* [OK] _all table names are hard coded_
* [OK] _enum columns have to be set the true value in set dialog (e.g. 1 = important)_
    * _possible fix is making a map with string-to-number and when closing the dialog convert the text to numbers and return numbers. probably via the _```QStringList return_strings()```_method_



Improvements:
========
* Fix tree sorting? I need type sorting for all the columns...
* Filter is not modular
    * column names are hard coded
    * line edits are hard coded
    * the field names are hard coded
* Better enums!
