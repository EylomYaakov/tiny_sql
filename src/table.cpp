#include "table.h"


Table::Table(const std::string& name, const std::vector<Column>& columns): name(name), columns(columns) {}


void Table::addRow(const std::vector<Value>& row){
    rows.push_back(row);
}

Table::Table(const std::string& name, const std::vector<Column>& columns, const std::vector<std::vector<Value>>& rows): name(name), columns(columns), rows(rows) {}

bool Table::operator==(const Table& other) const{
    if(name != other.name){
        return false;
    }
    if(columns != other.columns){
        return false;
    }
    if(rows != other.rows){
        return false;
    }
    return true;
}


bool Column::operator==(const Column& other) const{
    if(name != other.name){
        return false;
    }
    if(type != other.type){
        return false;
    }
    return true;
}

std::string valueToString(Value value){
    // value is integer
    if(value.index() == Column::INTEGER){
        return std::to_string(std::get<int>(value));
    }
    // value is text(string)
    return std::get<std::string>(value);
}
