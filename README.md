# BugTracker

This is my project done for **Charles University, Faculty of
Mathematics and Physics** for a beginner course in **C++**.

It is a simple bug tracking software using ```PostgreSQL``` and ```Qt``` allowing the user to simply connect to a bug database and manage the bugs - add new ones, view, edit and filter existing ones.

---

# Documentation

### User Documentation

The program is a bug tracking software, using a remote PostgreSQL database.
In the _Settings_ tab, you can set the database parameters, like the hostname, database name, user name and password. The _Table of table names_ variable is important to set, as this is the table in the database where you can specify the all the names of other tables you would like to use.

You can specify one enum type (there is more information about this restriction in the _Developer_ part of the docs) and assign it to multiple columns. For example: you can have a "bug importance" enum, and have two levels of importance in two columns, both of which would use this enum type.  

Once you have set the connection settings, you can select **Connect to databse** in the menu. If you have set the settings correctly, the program loads the databse. If not, a SQL error message is shown in the status bar.

Once connected, all of the bugs should load into the program and you are free to view them. Two actions are possible, _editing an existing bug_ and _adding a new one_.

Editing an existing bug is done by **double clicking** on the row of the bug. This brings up a dialog where you can edit the bug. If you fail to make a valid bug entry, the dialog closes and a SQL error is shown in the status bar of the program.

Adding a new bug is done through **Bug -> Add a new bug** in the menu. Again, a dialog appears and you have to fill in every field with the respective types of values (i.e. "False" if the colums is boolean, "25" if it is a number,...). If you fail to make a valid bug entry, the dialog closes and a SQL error is shown in the status bar of the program.

Note: the program expects more users working on the same database at the same time and is safe against this behavior. Therefore, two messages can show up when editing a bug.

* _"This bug is locked, because it is being edited by someone else."_ In this case, someone else might be changing the values of the bug and you should wait until the second user is done. You can retry in a minute.

* _"This bug has been edited and has a new value, please relod the database."_ In this case, the user finished editing the bug and you are now viewing an older version. The program wants you to reload the database, so it can download the new version and let you edit it safely, without losing your colleague's data.

Note: you login as a user to the database, if you do not have the rights to modify the database, you will not be able to do according actions (like edit, add or even view the bugs), this is a database problem and you should refer to your databse administrator.

### Developer Documentation
