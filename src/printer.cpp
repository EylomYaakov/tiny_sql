#include "printer.h"



std::vector<size_t> getColumnWidths(const Table& table){
    std::vector<size_t> columnWidths;
    for(int i = 0; i<table.columns.size(); i++){
        size_t width = table.columns[i].name.length();
        for(int j = 0; j<table.rows.size(); j++){
            size_t length = valueToString(table.rows[j][i]).length();
            if(length > width){
                width = length;
            }
        }
        columnWidths.push_back(width);
    }
    return columnWidths;
}


std::string valueToString(Value value){
    // value is integer
    if(value.index() == Column::INTEGER){
        return std::to_string(std::get<int>(value));
    }
    // value is text(string)
    return std::get<std::string>(value);
}


std::string addPadding(std::string value, size_t width){
    size_t paddingNeeded = width - value.length();
    // no padding needed
    if(paddingNeeded <= 0){
        return value;
    }
    return value.insert(0, paddingNeeded, ' ');
}

void printTable(const Table& table){
    // there is no data to display
    if(table.columns.size() == 0){
        std::cout << "the result set is empty" << std::endl;
        return;
    }
    std::vector<size_t> columnWidths = getColumnWidths(table);
    size_t rowWidth = std::reduce(columnWidths.begin(), columnWidths.end());
    // each column has '|' after, and there is an extra one at the start
    rowWidth = rowWidth + table.columns.size()+1;
    std::string rowSeperator(rowWidth, '-');
    // first and last char should be '+' instead of '-'
    rowSeperator[0] = '+';
    rowSeperator[rowSeperator.size()-1] = '+';
    std::cout << rowSeperator << std::endl;
    //print column names
    for(int i = 0; i<table.columns.size(); i++){
        std::cout << "|" << addPadding(table.columns[i].name, columnWidths[i]);
    }
    std::cout << "|" << std::endl << rowSeperator << std::endl;
    
    //print rows
    for(int i = 0; i<table.rows.size(); i++){
        for(int j = 0; j<table.rows[i].size(); j++){
            std::string valueToPrint = addPadding(valueToString(table.rows[i][j]), columnWidths[j]);
            std::cout << "|" << valueToPrint;
        }
        std::cout << "|" << std::endl << rowSeperator << std::endl;
    }
}


void printTableNames(const std::vector<std::string>& names){
    if(names.size() == 0){
        std::cout << "there are no tables" << std::endl;
    }
    for(int i=0; i<names.size(); i++){
        std::cout << names[i] << std::endl;
    }
}
