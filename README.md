# BugTracker

This is my project done for **Charles University, Faculty of
Mathematics and Physics** for a beginner course in **C++**.

It is a simple bug tracking software using ```PostgreSQL``` and ```Qt``` allowing the user to simply connect to a bug database and manage the bugs - add new ones, view, edit and filter existing ones.

---

# Documentation

### User Documentation

The program is a bug tracking software, using a remote PostgreSQL database.
In the _Settings_ tab, you can set the database parameters, like the hostname, database name, user name and password.

When setting up the database note, that it **has** to have these three tables:
* bugtable - the data of the bug reports
* bugstates - the enumerable states of the bug (Normal, Important,...)
* columns - the names of the columns of the main bugtable, to display in the view

**You can use the sql example database named `example_database.sql` that is added to the project. This is a predefined example SQL query that will set the whole database up itself.**

You can specify one enum type (there is more information about this restriction in the _Developer_ part of the docs) and assign it to multiple columns. For example: you can have a "bug importance" enum, and have two levels of importance in two columns, both of which would use this enum type.  

Once you have set the connection settings, you can select **Connect to databse** in the menu. If you have set the settings correctly, the program loads the databse. If not, a SQL error message is shown in the status bar.

Once connected, all of the bugs should load into the program and you are free to view them. Two actions are possible, _editing an existing bug_ and _adding a new one_.

Editing an existing bug is done by **double clicking** on the row of the bug. This brings up a dialog where you can edit the bug. If you fail to make a valid bug entry, the dialog closes and a SQL error is shown in the status bar of the program.

Adding a new bug is done through **Bug -> Add a new bug** in the menu. Again, a dialog appears and you have to fill in every field with the respective types of values (i.e. "False" if the columns is boolean, "25" if it is a number,...). If you fail to make a valid bug entry, the dialog closes and a SQL error is shown in the status bar of the program.

Note: the program expects more users working on the same database at the same time and is safe against this behavior. Therefore, two messages can show up when editing a bug.

* _"This bug is locked, because it is being edited by someone else."_ In this case, someone else might be changing the values of the bug and you should wait until the second user is done. You can retry in a minute.

* _"This bug has been edited and has a new value, please relod the database."_ In this case, the user finished editing the bug and you are now viewing an older version. The program wants you to reload the database, so it can download the new version and let you edit it safely, without losing your colleague's data.

Note: you login as a user to the database, if you do not have the rights to modify the database, you will not be able to do according actions (like edit, add or even view the bugs), this is a database problem and you should refer to your database administrator.

### Developer Documentation

The program is composed out of **N** components. Namel:
* Database object - ```dbhandler.hpp```
* Data object  - ```dataobject.hpp```
* Item Editor dialog - ```itemeditdialog.hpp```
* DB settings dialog - ```dbsetdialog.hpp```
* Main Window - ```mainwindow.hpp```

##### Database object
This class is a simple handler for the DB connection. The connection is stored in this object, along with the user data used for connection (such as username, hostname, and so on). When the ```QSqlDatabase``` object is destroyed, it kills the connection, so the whole ```DBHandler``` doesn't need a specific destructor.

##### Data object
The only reason for this object is bundling all the Bug data together. It doesn't have any methods operating it, all the components manipulate the data themselves. It stores the bugs in ```bug_values_```, it remembers which columns are enumerators in ```enum_cols```, it remembers names of columns in ```column_names_``` and it can translate the enum types to their values and back with ```state_names_``` and ```rev_state_names_```.

We store the data as ```QString```, because the ```QTreeView``` can only show ```QString``` objects. Therefore we need to convert all the data into strings. This helps with editing as well, since it allows the user to write the new data as a text.

If implementing support for multiple enumerator columns, some changes need to be made. See below the chapter about **Improving and adding features**.

##### Item Editor dialog
This dialog is used to edit (or add) bugs in the database. It modifies itself, adding the needed amount of ```QLineEdit``` for normal data and ```QComboBox``` for enumerator type columns.

To accomplish this, three classes are implemented, ```my_widget```, and its two children ```my_ComboBox``` and ```my_lineEdit```. This is done to provide ```get_text()``` interface for the dialog to call upon being accepted and to store both types of these ```QWidget``` objects in one container ```widgets_```.

A ```return_strings()``` method is implemented and called on the dialog upon the user pressing _Done_.

##### Database settings dialog
This is a very simple dialog, which lets you edit its items and if the values are accepted (the dialog checks, if the port number is indeed an integer) allows the calling code to access the values that the user accepted by methods such as ```QString username() const```.

##### MainWindow class

The runtime of the program relies on this class entirely. It's constructor is called upon opening the program. This class does several things:

* loads settings
* connects to the DB
* load/displays/modifies the data from DB
* shows all GUI

The DB is initialized by calling ```initialize_DB()``` which loads the settings and if all is correct, instantiates the Database Handler to a state with all its settings filed. ```load_new_database()``` then completely loads the DB into memory and displays it.

The MainWindow also handles all events that arise, handling things like clicking on buttons and doing the appropriate things. Methods with ```_slot``` in name are action handling methods, ```_triggered``` shows that the method handles a menu action while ```_clicked``` signalizes a button.

The main two features are viewing the bug, editing the bug and adding new bugs. This is handled in the methods (slots) ```tree_itemClicked_slot()``` and ```tree_itemDoubleClicked_slot()``` as well as ```add_new_item()``` which handles adding a new bug.

##### Improving and adding features
The main improvement that can be made to the application is adding the supprt for more enumerator columns. This should be very easy and the ground for this feature is prepared. There is a set for storing the information of which columns are enumerators in the DataObject class, and the two maps for converting values to IDs (in the relation DB) and back can be each made into a vector of maps. The method ```return_states_atIndex()``` is prepared which serves for returing all the states of a given enumerator's index which is needed **(and used by)** the ComboBoxes in ItemEditDialog. Reimplementing this method will make the ComboBoxes work properly. The only thing left to do is add the Database-side support (tables which indicate what columns are enumerators and the adequate enum tables) and changing the ```prepare_view_data()``` which loads the names of columns and the enumerator column to a state where all the enumerator columns are loaded.

One more thing to improve would be some kind of DB data paging, to allow the program to download only smaller chunks of data drom the DB. Note that I have **not** prepared ground for this change and it might require a more solid ```load_query_intoMemory()``` method implementation as it now does something like ```SELECT * from main_table;``` and load it into memory split accordingly.
