#ifndef printer_h
#define printer_h

#include <iostream>
#include "table.h"
#include <numeric> 


std::vector<size_t> getColumnWidths(const Table& table);
std::string addPadding(std::string value, size_t width);
void printTable(const Table& table);
void printTableNames(const std::vector<std::string>& names);
void printWelcome();



#endif /* printer_h */
