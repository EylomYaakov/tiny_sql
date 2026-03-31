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

void printWelcome(){
    std::cout << "Welcome to TinySQL!" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "CREATE TABLE <name> (<col> <type>, ...)" << std::endl;
    std::cout << "INSERT INTO <name> VALUES (...)" << std::endl;
    std::cout << "SELECT <cols|*> FROM <name>" << std::endl;
    std::cout << "SELECT <cols|*> FROM <name> WHERE <col> = <value>" << std::endl;
    std::cout << "DELETE FROM <name> WHERE <col> = <value>" << std::endl;
    std::cout << "DROP TABLE <name>" << std::endl;
    std::cout << ".tables" << std::endl;
    std::cout << ".save <file> - <file> is optional. if not specified, tables will be saved to \"tabels.csv\"" << std::endl;
    std::cout << "All tables will automatically be saved to \"tables.csv\" on .quit" << std::endl;
    std::cout << ".load <file> - <file> is optional. if not specified, tables will be loaded from \"tables.csv\". loading will delete all current tables" << std::endl;
    std::cout << "Tables will automatically be loaded from \"tables.csv\" on startup" << std::endl;
    std::cout << ".quit" << std::endl;
    std::cout << "Enter a command:" << std::endl;
}
