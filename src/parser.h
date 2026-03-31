#ifndef parser_h
#define parser_h

#include <iostream>
#include "table.h"
#include "tokenizer.h"


enum CommandType {CREATE_TABLE, INSERT_INTO, SELECT, SELECT_WHERE, DELETE_FROM, DROP_TABLE};

class Command{
private:
    CommandType type;
    std::string tableName;
    std::vector<std::string> columnNames;
    std::vector<Column::Type> columnTypes;
    std::vector<Value> values;
public:
    Command(CommandType type);
    // consturctor for easier tests
    Command(CommandType type, const std::string& tableName, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes, const std::vector<Value>& values);
    void setType(CommandType type);
    void setTableName(const std::string& tableName);
    void addColumnName(const std::string& columnName);
    void addColumnType(Column::Type type);
    void addValue(const Value& value);
    CommandType getType() const;
    std::string getTableName() const;
    std::vector<std::string> getColumnNames() const;
    std::vector<Column::Type> getColumnTypes() const;
    std::vector<Value> getValues() const;
    //will be used for google testing
    bool operator==(const Command& other) const;
};

Command parseCommand(const std::vector<Token>& tokens);

Command parseCreateTable(const std::vector<Token>& tokens);

Command parseInsertInto(const std::vector<Token>& tokens);

Command parseSelectFrom(const std::vector<Token>& tokens);

Command parseSelectFromWhere(const std::vector<Token>& tokens);

Command parseDeleteFrom(const std::vector<Token>& tokens);

Command parseDropTable(const std::vector<Token>& tokens);

int find(const std::vector<Token>& tokens, std::string s, int start=0);

Value parseLiteral(const std::string& literal);

std::string parseString(const std::string& s);

// returning the minimum number of tokens each command should have
int getMinCommandTokens(CommandType type);


#endif /* parser_h */
