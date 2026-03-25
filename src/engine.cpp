#include "engine.h"


void Engine::createTable(Command command){
    std::string tableName = command.getTableName();
    if(tables.find(tableName) != tables.end()){
        throw "table " + tableName + " already exists";
    }
    std::vector<std::string> columnNames = command.getColumnNames();
    std::vector<Column::Type> columnTypes = command.getColumnTypes();
    std::vector<Column> columns;
    for(int i = 0; i<columnNames.size(); i++){
        Column column = {columnNames[i], columnTypes[i]};
        columns.push_back(column);
    }
    Table table(tableName, columns);
    tables.insert({tableName, table});
}

void Engine::insertIntoTable(Command command){
    std::string tableName = command.getTableName();
    tableExistsCheck(tableName);
    std::vector<Value> values = command.getValues();
    // using & so we would be able to change the table
    Table& table = tables[tableName];
    if(table.columns.size() != values.size()){
        throw "incorrect values count";
    }
    std::vector<Value> row;
    for(int i = 0; i<values.size(); i++){
        Value toAdd = values[i];
        // index should be 0 for INTEGER and 1 for TEXT - same as the types enum
        if(toAdd.index() == table.columns[i].type){
            row.push_back(toAdd);
        }
        else{
            // value is int but should be string
            if(toAdd.index() == Column::INTEGER){
                throw std::to_string(std::get<int>(toAdd)) + " is not a a string";
            }
            // value is string but should be int
            else{
                throw std::get<std::string>(toAdd) + " is not an integer";
            }
        }
    }
    table.addRow(row);
}

Table Engine::select(Command command){
    std::string tableName = command.getTableName();
    tableExistsCheck(tableName);
    Table table = tables[tableName];
    std::vector<std::string> columnNames = command.getColumnNames();
    Value whereFilterValue;
    int whereFilterColumnIndex = -1;
    if(command.getType() == SELECT_WHERE){
        //if there is "WHERE" - the last column name is the name of the filter column, and the first(and only) value is the filter value
        whereFilterValue = command.getValues()[0];
        whereFilterColumnIndex = findWhereFilterColumn(columnNames, tableName);
    }
    
    // keep the chosen columns - we will need to display their names too.
    std::vector<Column> chosenColumns;
    // find chosen columns indexes
    std::vector<int> columnIndexes = findColumnIndexes(columnNames, chosenColumns, tableName);
    
    std::vector<std::vector<Value>> filteredRows(table.rows.size());
    for(int i=0; i<columnIndexes.size(); i++){
        // for each one of the column indexes, add the element in this index into the filtered row - for each row(except for rows that doesn't fit to the "WHERE" filter condition).
        bool added = false;
        for(int j=0; j<filteredRows.size(); j++){
            // skip rows that doesn't fit the "WHERE" filter condition(if there is one)
            if(command.getType() == SELECT_WHERE && table.rows[j][whereFilterColumnIndex] != whereFilterValue){
                continue;
            }
            filteredRows[j].push_back(table.rows[j][columnIndexes[i]]);
            added = true;
        }
        
        
        // remove columns that has no rows to dispaly(because of "WHERE" filter)
        if (added == false){
            std::erase(chosenColumns, table.columns[columnIndexes[i]]);
        }
    }
    // remove the empty rows(happens when there is a WHERE filter)
    std::erase_if(filteredRows, [](const std::vector<Value>& row) {
        return row.empty();
    });
    
    // return a table and not just the rows to be able to print the column names too.
    Table filteredTable(tableName, chosenColumns);
    filteredTable.rows = filteredRows;
    return filteredTable;
    
}


void Engine::deleteFromTable(Command command){
    std::string tableName = command.getTableName();
    tableExistsCheck(tableName);
    std::string columnName = command.getColumnNames()[0];
    int columnIndex = findColumn(columnName, tableName);
    if(columnIndex == -1){
        throw columnName + " does not exists in table " + tableName;
    }
    Value value = command.getValues()[0];
    Table& table = tables[tableName];
    // remove all rows such that row[columnIndex] = value
    std::erase_if(table.rows, [columnIndex, value](const std::vector<Value>& row){ return row[columnIndex] == value;});
}

void Engine::dropTable(Command command){
    std::string tableName = command.getTableName();
    tableExistsCheck(tableName);
    tables.erase(tableName);
}



// checks if the table exists and throws exception if not
void Engine::tableExistsCheck(const std::string& tableName) const{
    if(tables.find(tableName) == tables.end()){
        throw "table " + tableName + " does not exists";
    }
    
}

// find column index in a table. if the column does not exists, return -1
int Engine::findColumn(const std::string& columnName, const std::string& tableName){
    bool found = false;
    for(int i=0; i<tables[tableName].columns.size(); i++){
        if(tables[tableName].columns[i].name == columnName){
            return i;
        }
    }
    return -1;
}


// a function that gets columnNames, find their indexes and add them to the chosen columns vector(need in order to have the actual columns with their name and not *)
std::vector<int> Engine::findColumnIndexes(const std::vector<std::string>& columnNames, std::vector<Column>& chosenNames, std::string tableName){
    Table table = tables[tableName];
    std::vector<int> columnIndexes;
    for(int i =0; i<columnNames.size(); i++){
        bool found = false;
        // find the index of the current column
        for(int j=0; j<table.columns.size(); j++){
            // include all indexes and names for *
            if(columnNames[i] == "*"){
                columnIndexes.push_back(j);
                chosenNames.push_back(table.columns[j]);
                found = true;
            }
            else if(table.columns[j].name == columnNames[i]){
                columnIndexes.push_back(j);
                chosenNames.push_back(table.columns[j]);
                found = true;
                break;
            }
        }
        // throw exception if column does not exists
        if(!found){
            throw columnNames[i] + " does not exists in table " + tableName;
        }
    }
    return columnIndexes;
}


// a function that finds the index of the column in where command( <col> = value), deletes it from the columnNames vector and throws exception if the colum name does not exists.
int Engine::findWhereFilterColumn(std::vector<std::string>& columnNames, const std::string& tableName){
    std::string whereFilterColumn = columnNames[columnNames.size()-1];
    columnNames.pop_back();
    int whereFilterColumnIndex = findColumn(whereFilterColumn, tableName);
    if(whereFilterColumnIndex == -1){
        throw whereFilterColumn + " does not exists in table " + tableName;
    }
    return whereFilterColumnIndex;
}



