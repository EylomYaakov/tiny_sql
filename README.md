# TinySQL – In-Memory SQL Database

TinySQL is in-memory SQL database written in C++.  
It supports basic SQL operations such as creating tables, inserting data, querying, and persistence to disk.

## Features

- In-memory database
- Simple SQL-like syntax
- Supports INTEGER and TEXT types
- Basic commands: CREATE, INSERT, SELECT, DELETE, DROP
- Save/load database to file

## supported commands

For all supported commands, keywords can be written in uppercase, lowercase letters, or any combination of both. For example, 'CREATE TABLE', 'create table' and 'Create Table' are all completely valid.

 Additionally, all identifiers(table and column names) and string values can be typed with or without quotes. For example, name, 'name' and "name" are all completely valid.
 if an identifier or value contains space, comma, equal sign or parentheses, you should use quotes. For example, "Eylom =Yaakov" and 'Eylom, (Yaakov)' are both fine, but Eylom Yaakov without quotes will not work.

### `CREATE TABLE <name> (<col> <type>, ...)`

This command creates a new table with the given column names and types.
`<col>` should be the column name (it can be any name) and the type must be either INTEGER or TEXT.

Usage example:

- `CREATE TABLE "users" ('id' INTEGER, first_name TEXT, 'last_name' TEXT, age INTEGER)` - creates a table named users, with 4 columns, id and first_name, last_name and age.

The table name is a unique identifier, so after running this command you will not be able to create another table with the same name.

### `INSERT INTO <name> VALUES (...)`

This command inserts a row into an existing table. The number and types of the values should match the number and types of the columns in the table.

Usage examples:

- `insert into users values (1, Eylom, Yaakov, 19)`
- `insert into users values (2, "Yuval", "Cohen", 20)`
- `insert into users values (3, 'Omer', 'Yaakov', 20)`
- `insert into users values (4, 'Ben', 'Levi', 21)`

These commands will insert four rows into the users table

### `SELECT <cols|*> FROM <name>`

This command prints the matching columns for all rows. use `*` to select all columns. You can use either column names, `*` or both. you may also repeat columns or use `*` multiple times. in such cases, matching columns will be printed several times(depending to how many times you repeat them or use `*`)

Usage examples:

- `SELECT * FROM users` - prints the whole table:

```
+---------------------------+
|id|first_name|last_name|age|
+---------------------------+
| 1|     Eylom|   Yaakov| 19|
+---------------------------+
| 2|     Yuval|    Cohen| 20|
+---------------------------+
| 3|      Omer|   Yaakov| 20|
+---------------------------+
| 4|       Ben|     Levi| 21|
+---------------------------+
```

- `select first_name, last_name from users` - prints only the first_name and last_name columns: 

```
+--------------------+
|first_name|last_name|
+--------------------+
|     Eylom|   Yaakov|
+--------------------+
|     Yuval|    Cohen|
+--------------------+
|      Omer|   Yaakov|
+--------------------+
|       Ben|     Levi|
+--------------------+
```

- `select first_name, *, *, first_name from users` - prints the first_name column, then all the table columns twice, and then the first_name column again: 

```
+-----------------------------------------------------------------------------+
|first_name|id|first_name|last_name|age|id|first_name|last_name|age|first_name|
+-----------------------------------------------------------------------------+
|     Eylom| 1|     Eylom|   Yaakov| 19| 1|     Eylom|   Yaakov| 19|     Eylom|
+-----------------------------------------------------------------------------+
|     Yuval| 2|     Yuval|    Cohen| 20| 2|     Yuval|    Cohen| 20|     Yuval|
+-----------------------------------------------------------------------------+
|      Omer| 3|      Omer|   Yaakov| 20| 3|      Omer|   Yaakov| 20|      Omer|
+-----------------------------------------------------------------------------+
|       Ben| 4|       Ben|     Levi| 21| 4|       Ben|     Levi| 21|       Ben|
+-----------------------------------------------------------------------------+
```

### `SELECT <cols|*> FROM <name> WHERE <col> = <value>`

Same as SELECT command, but prints only the rows where `<col> = <value>`.


Usage examples:

- `SELECT * FROM users WHERE age = 20' - will print`:

```
+---------------------------+
|id|first_name|last_name|age|
+---------------------------+
| 2|     Yuval|    Cohen| 20|
+---------------------------+
| 3|      Omer|   Yaakov| 20|
+---------------------------+
```

- `select first_name, last_name from users where last_name = Yaakov` - will print: 

```
+--------------------+
|first_name|last_name|
+--------------------+
|     Eylom|   Yaakov|
+--------------------+
|      Omer|   Yaakov|
+--------------------+
```

- `select * from users where id = 4` - will print: 

```
+---------------------------+
|id|first_name|last_name|age|
+---------------------------+
| 4|       Ben|     Levi| 21|
+---------------------------+
```


### `DELETE FROM <name> WHERE <col> = <value>`

This command remove rows all the rows where `<col> = <value>` from table `<name>`

Usage example:

- `DELETE FROM <name> WHERE last_name = Yaakov` will delete these rows from table users:

```
+---------------------------+
| 1|     Eylom|   Yaakov| 19|
+---------------------------+
| 3|      Omer|   Yaakov| 20|
+---------------------------+
```

### `DROP TABLE <name>`

This command remove the table `<name>` entirely. After the table is removed, you can create a new table with this name.

### `.tables`

This command list all exisiting tables.

### `.save <file>`

This command saves all tables to a file with the path `<file>`.
The `<file>` parameter is optional. if not specified, tables will be saved to 'tables.cvs'

### `.load <file>`

This command loads tables from a file with the given path `<file>`.
The `<file>` parameter is optional. if not specified, tables will be loaded from 'tables.csv'

load from 'tables.cvs' happens automatically on startup.

### `.quit`

This command saves the tables to 'tables.csv' and quits the REPL.
