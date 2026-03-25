#ifndef table_h
#define table_h

#include <iostream>
#include <variant>
#include <string>
#include <vector>

using Value = std::variant<int, std::string>;

struct Column {
    std::string name;
    enum Type { INTEGER, TEXT } type;
    //will be used for google testing
    bool operator==(const Column& other) const;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Value>> rows;
    Table(const std::string& name, const std::vector<Column>& columns);
    Table(const std::string& name, const std::vector<Column>& columns, const std::vector<std::vector<Value>>& rows);
    // default constructor to be able to use the [] in the unordered map
    Table() = default;
    void addRow(const std::vector<Value>& row);
    //will be used for google testing
    bool operator==(const Table& other) const;
};

#endif /* table_h */
