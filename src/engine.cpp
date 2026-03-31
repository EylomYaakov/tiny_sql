#include "engine.h"


void Engine::createTable(Command command){
    std::string tableName = command.getTableName();
    if(tables.find(tableName) != tables.end()){
        throw CommandException("error: table " + tableName + " already exists");
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
        throw CommandException("error: incorrect values count");
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
                // consider the value as string instead of throwing an exception
                Value stringValue = std::to_string(std::get<int>(toAdd));
                row.push_back(stringValue);
            }
            // value is string but should be int
            else{
                throw CommandException("error: " + std::get<std::string>(toAdd) + " is not an integer");
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
        throw CommandException("error: " + columnName + " does not exists in table " + tableName);
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
        throw CommandException("error: table " + tableName + " does not exists");
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
            throw CommandException("error: " + columnNames[i] + " does not exists in table " + tableName);
        }
    }
    return columnIndexes;
}


// a function that finds the index of the column in where command( <col> = value), deletes it from the columnNames vector and throws exception if the columnפ name does not exists.
int Engine::findWhereFilterColumn(std::vector<std::string>& columnNames, const std::string& tableName){
    std::string whereFilterColumn = columnNames[columnNames.size()-1];
    columnNames.pop_back();
    int whereFilterColumnIndex = findColumn(whereFilterColumn, tableName);
    if(whereFilterColumnIndex == -1){
        throw CommandException("error: " + whereFilterColumn + " does not exists in table " + tableName);
    }
    return whereFilterColumnIndex;
}



std::vector<std::string> Engine::getTableNames() const{
    std::vector<std::string> names;
    for(const std::pair<const std::string, Table>& pair : tables){
        names.push_back(pair.first);
    }
    return names;
}


void Engine::saveTables(const std::string& filePath) const{
    std::ofstream file(filePath);
    if(!file.is_open()){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    for(const std::pair<const std::string, Table>& pair : tables){
        // write table name
        file << pair.first << "\n";
        Table table = pair.second;
        
        //write column names
        for(int i=0; i<table.columns.size(); i++){
            file <<  "\"" << table.columns[i].name << "\"";
            if(i != table.columns.size() - 1){
                file << ",";
            }
        }
        
        file << "\n";
        
        //write column types
        for(int i=0; i<table.columns.size(); i++){
            file << "\"" << table.columns[i].type << "\"";
            if(i != table.columns.size() - 1){
                file << ",";
            }
        }
        file << "\n";
        
        // write data
        for(int i = 0; i<table.rows.size(); i++){
            for(int j = 0; j<table.rows[i].size(); j++){
                file << "\"" << valueToString(table.rows[i][j]) << "\"";
                if(j != table.rows[i].size()-1){
                    file << ",";
                }
            }
            file << "\n";
        }
        // seperation line between tables
        file << "\n";
    }
    file.close();
}

void Engine::loadTables(const std::string& filePath, bool startup){
    if(!startup){
        std::cout << "loading will delete all current tables, are you sure you want to load? type N for no or anything else for yes" << std::endl;
        std::string answer;
        getline(std::cin, answer);
        if(answer == "N"){
            return;
        }
        
    }
    std::ifstream file(filePath);
    // don't print error on startup - the file might not exists
    if(!file.is_open() && !startup){
        std::cerr << "Failed to open file" << std::endl;
        return;
    }
    else if(!file.is_open()){
        return;
    }
    // clear tables
    tables.clear();
    Table table;
    std::string tableName;
    std::string line;
    std::vector<std::string> columnNames;
    std::vector<std::string> columnTypes;
    // row values
    std::vector<std::string> values;
    std::vector<Column> columns;
    std::vector<std::vector<Value>> rows;
    // which row of the current table in the csv we are reading(first is table name, second is column names, third is column types, and the rest are data)
    int tableIndex = 0;
    while(std::getline(file, line)){
        std::stringstream s(line);
        // new table
        if(line.empty()){
            table = {tableName, columns, rows};
            tables.insert({tableName, table});
            tableIndex = -1;
            tableName.clear();
            columnNames.clear();
            columns.clear();
            rows.clear();
        }
        // table name
        else if(tableIndex == 0){
            tableName = line;
        }
        // column names
        else if(tableIndex == 1){
            columnNames = getLineValues(line);
        }
        // column types
        else if(tableIndex == 2){
            columnTypes = getLineValues(line);
            for(int i=0; i<columnTypes.size(); i++){
                Column column;
                // INTEGER
                if(columnTypes[i] == "0"){
                    column = {columnNames[i], Column::INTEGER};
                }
                // TEXT
                else{
                    column = {columnNames[i], Column::TEXT};
                }
                columns.push_back(column);
            }
            
        }
        // data
        else{
            values = getLineValues(line);
            std::vector<Value> row;
            int i = 0;
            for(int i=0; i<values.size(); i++){
                if(columns[i].type == Column::INTEGER){
                    try{
                        row.push_back(std::stoi(values[i]));
                    }
                    catch(const std::invalid_argument& e){
                        std::cerr << "error loading file" << std::endl;
                    }
                }
                else{
                    row.push_back(values[i]);
                }
            }
            rows.push_back(row);
        }
        tableIndex++;
    }
    file.close();
}

std::string Engine::addEscaping(const std::string& line) const{
    std::string escapedLine;
    for(int i=0; i<line.length(); i++){
        // add \ escaping before real '#' and '\'
        if(line[i] == '#' || line[i] == '\\'){
            escapedLine += '\\';
            escapedLine += line[i];
        }
        // ',' get replaced with '#'
        else if(line[i] == ','){
            escapedLine += '#';
        }
        else{
            escapedLine += line[i];
        }
    }
    return escapedLine;
}

std::string Engine::removeEscaping(const std::string& escapedLine) const{
    std::string line;
    for(int i=0; i<escapedLine.size(); i++){
        if(escapedLine[i] == '\\'){
            if(i != escapedLine.size()-1 && (escapedLine[i+1] == '#' || escapedLine[i+1] == '\\')){
                line += escapedLine[i+1];
                // skip next iteration - already added
                i++;
                continue;
            }
        }
        // replace '#' with ','
        else if(escapedLine[i] == '#'){
            line += ",";
        }
        else{
            line += escapedLine[i];
        }
    }
    return line;
}

std::vector<std::string> Engine::getLineValues(const std::string& line) const{
    std::vector<std::string> values;
    std::string value;
    bool quote = false;
    for(int i=0; i<line.size(); i++){
        // end of value - values splitted by ,
        if(line[i] == ',' && !quote){
            values.push_back(value);
            value.clear();
        }
        // start or end of value -  each value starts and ends with quotes
        else if(line[i] == '\"'){
            quote = !quote;
        }
        // reading value - inside quotes
        else{
            value += line[i];
        }
    }
    // push back last value(not ',' after him)
    values.push_back(value);
    return values;
}

