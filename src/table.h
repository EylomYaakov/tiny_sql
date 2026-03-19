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
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<Value>> rows;
};

#endif /* table_h */
