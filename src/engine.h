#ifndef engine_h
#define engine_h

#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "parser.h"

class Engine{
    std::unordered_map<std::string, Table> tables;
    void tableExistsCheck(const std::string& tableName) const;
    int findColumn(const std::string& columnName, const std::string& tableName);
    std::vector<int> findColumnIndexes(const std::vector<std::string>& columnNames, std::vector<Column>& chosenColumns, std::string TableName);
    int findWhereFilterColumn(std::vector<std::string>& columnNames, const std::string& columnName);
public:
    void createTable(Command command);
    void insertIntoTable(Command command);
    Table select(Command command);
    void deleteFromTable(Command command);
    void dropTable(Command command);
    std::vector<std::string> getTableNames() const;
    
};

#endif /* engine_h */
