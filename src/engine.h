#ifndef engine_h
#define engine_h

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "parser.h"


class Engine{
    std::unordered_map<std::string, Table> tables;
    void tableExistsCheck(const std::string& tableName) const;
    int findColumn(const std::string& columnName, const std::string& tableName);
    std::vector<int> findColumnIndexes(const std::vector<std::string>& columnNames, std::vector<Column>& chosenColumns, std::string TableName);
    int findWhereFilterColumn(std::vector<std::string>& columnNames, const std::string& columnName);
    std::string addEscaping(const std::string& line) const;
    std::string removeEscaping(const std::string& escapedLine) const;
    std::vector<std::string> getLineValues(const std::string& line) const;
    public:
    void createTable(Command command);
    void insertIntoTable(Command command);
    Table select(Command command);
    void deleteFromTable(Command command);
    void dropTable(Command command);
    std::vector<std::string> getTableNames() const;
    void saveTables(const std::string& filePath) const;
    void loadTables(const std::string& filePath, bool startup = false);
    
};

#endif /* engine_h */
