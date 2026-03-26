#include "parser.h"

Command parseCommand(const std::vector<Token>& tokens){
    // shortest command have 3 tokens
    if(tokens.size() < 3){
        throw CommandException("syntax error: incomplete command");
    }
    //first token is always a keyword
    if(tokens[0].type != Token::KEYWORD){
        throw CommandException("unknown command: " + tokens[0].value);
    }
    if(tokens[0].value == "CREATE" && tokens[1].value == "TABLE"){
        return parseCreateTable(tokens);
    }
    else if(tokens[0].value == "INSERT" && tokens[1].value == "INTO"){
        return parseInsertInto(tokens);
    }
    else if(tokens[0].value == "SELECT"){
        // no where clause
        if(find(tokens, "WHERE") == -1){
            return parseSelectFrom(tokens);
        }
        return parseSelectFromWhere(tokens);
    }
    else if(tokens[0].value == "DELETE" && tokens[1].value == "FROM"){
        return parseDeleteFrom(tokens);
    }
    else if(tokens[0].value == "DROP" && tokens[1].value == "TABLE"){
        return parseDropTable(tokens);
    }
    throw CommandException("unknown command: " + tokens[0].value);
}

Command parseCreateTable(const std::vector<Token>& tokens){
    Command command(CREATE_TABLE);
    command.setTableName(parseString(tokens[2].value));
    //create table command should have at least 7 tokens
    if(tokens.size() < 7){
        throw CommandException("syntax error: incomplete command");
    }
    // 4rd and last token should be ()
    if(tokens[3].value != "(" || tokens[tokens.size()-1].value != ")"){
        throw CommandException("syntax error: expected ()");
    }
    // 5th token should be the first column name
    for(int i=4; i+2<tokens.size(); i += 3){
        command.addColumnName(parseString(tokens[i].value));
        std::string type = tokens[i+1].value;
        if(type == "INTEGER"){
            command.addColumnType(Column::INTEGER);
        }
        else if(type == "TEXT"){
            command.addColumnType(Column::TEXT);
        }
        else{
            throw CommandException("unknown type: " + type);
        }
        std::string symbol = tokens[i+2].value;
        // the symbol should be ',', excpet the last one
        if(symbol != "," && i+2 != tokens.size()-1){
            throw CommandException("syntax error: expected \',\'");
        }
    }
    return command;
}

Command parseInsertInto(const std::vector<Token>& tokens){
    Command command(INSERT_INTO);
    command.setTableName(parseString(tokens[2].value));
    //insert into command should have at least 6 tokens
    if(tokens.size() < 6){
        throw CommandException("syntax error: incomplete command");
    }
    if(tokens[3].value != "VALUES"){
        throw CommandException("syntax error: incomplete command");
    }
    // 4th and last token should be ()
    if(tokens[4].value != "(" || tokens[tokens.size()-1].value != ")"){
        throw CommandException("syntax error: expected ()");
    }
    for(int i=5; i+1 < tokens.size(); i+=2){
        command.addValue(parseLiteral(tokens[i].value));
        std::string symbol = tokens[i+1].value;
        // the symbol should be ',', excpet the last one
        if(symbol != "," && i+1 != tokens.size()-1){
            throw CommandException("syntax error: expected \',\'");
        }
    }
    return command;
}

Command parseSelectFrom(const std::vector<Token>& tokens){
    Command command(SELECT);
    //select command should have at least 4 tokens
    if(tokens.size() < 4){
        throw CommandException("syntax error: incomplete command");
    }
    //select command must contain "FROM" and it should be after the first 2 tokens
    if(find(tokens, "FROM", 2) == -1){
        throw CommandException("syntax error: incomplete command");
    }
    //should be the first col
    int i = 1;
    do{
        command.addColumnName(parseString(tokens[i].value));
        // after each column name, there should be ',' or "FROM"
        if(tokens[i+1].value != "," && tokens[i+1].value != "FROM"){
            throw CommandException("syntax error: expected \',\'");
        }
        i += 2;
    // stop when passed the "FROM" token
    }while(tokens[i-1].value != "FROM");
    command.setTableName(parseString(tokens[i].value));
    //if there is no WHERE clause table name should be the last token
    if(i != tokens.size()-1 && tokens[i+1].value != "WHERE"){
        throw CommandException("syntax error: unknown value " + tokens[i+1].value);
    }
    return command;
}

Command parseSelectFromWhere(const std::vector<Token>& tokens){
    // would throw exception if there is no "WHERE"
    Command command = parseSelectFrom(tokens);
    command.setType(SELECT_WHERE);
    
    //select command should have at least 7 tokens
    if(tokens.size() < 7){
        throw CommandException("syntax error: incomplete command");
    }
    
    // "WHERE" should only be 5th token or after
    int i = find(tokens, "WHERE", 4) + 1;
    // last col name belongs to the WHERE clause and not the SELECT one
    command.addColumnName(parseString(tokens[i].value));
    // between the col name and the literal, there should be '='
    if(tokens[i+1].value != "="){
        throw CommandException("syntax error: expected \'=\'");
    }
    command.addValue(parseLiteral(tokens[i+2].value));
    i += 3;
    // the value should be the last token
    if(i < tokens.size()){
        throw CommandException("syntax error: unknown value" + tokens[i].value);
    }
    return command;
}


int find(const std::vector<Token>& tokens, std::string s, int start){
    for(int i=start; i<tokens.size(); i++){
        if(tokens[i].value == s){
            return i;
        }
    }
    return -1;
}


Command parseDropTable(const std::vector<Token>& tokens){
    Command command(DROP_TABLE);
    command.setTableName(parseString(tokens[2].value));
    if(tokens.size() > 3){
        throw CommandException("syntax error: incomplete command");
    }
    return command;
}


Command parseDeleteFrom(const std::vector<Token>& tokens){
    Command command(DELETE_FROM);
    command.setTableName(parseString(tokens[2].value));
    //delete from command should have at least 6 tokens
    if(tokens.size() < 7){
        throw CommandException("syntax error: incomplete command");
    }
    if(tokens[3].value != "WHERE"){
        throw CommandException("syntax error: incomplete command");
    }
    command.addColumnName(parseString(tokens[4].value));
    if(tokens[5].value != "="){
        throw CommandException("syntax error: expected \'=\'");
    }
    command.addValue(parseLiteral(tokens[6].value));
    return command;
}

Value parseLiteral(const std::string& literal){
    // the literal started and ended with ' or " - its a text;
    if(literal != parseString(literal)){
        Value text = parseString(literal);
        return text;
    }
    try{
        Value number = stoi(literal);
        return number;
    }
    catch(const std::invalid_argument& e){
        // if cant parse as int, return a string.
        return literal;
    }
}

// parse string that may start and end with ' or ". if so, remove them.
std::string parseString(const std::string& s){
    size_t length = s.length();
    if(length >= 2 && ((s[0] == '\'' && s[length-1] == '\'' ) || (s[0] == '\"' && s[length-1] == '\"'))){
        return s.substr(1, length-2);
    }
    return s;
}


Command::Command(CommandType type): type(type){}

Command::Command(CommandType type, const std::string& tableName, const std::vector<std::string>& columnNames, const std::vector<Column::Type>& columnTypes, const std::vector<Value>& values): type(type), tableName(tableName), columnNames(columnNames), columnTypes(columnTypes), values(values) {}

void Command::setType(CommandType type){
    this->type = type;
}

void Command::setTableName(const std::string& tableName){
    this->tableName =  tableName;
}

void Command::addColumnName(const std::string& columnName){
    this->columnNames.push_back(columnName);
}

void Command::addColumnType(Column::Type type){
    this->columnTypes.push_back(type);
}

void Command::addValue(const Value& value){
    this->values.push_back(value);
}

std::string Command::getTableName() const{
    return this->tableName;
}

std::vector<std::string> Command::getColumnNames() const{
    return this->columnNames;
}

std::vector<Column::Type> Command::getColumnTypes() const{
    return this->columnTypes;
}

std::vector<Value> Command::getValues() const{
    return this->values;
}

CommandType Command::getType() const{
    return this->type;
}

bool Command::operator==(const Command& other) const{
    if(type != other.type){
        return false;
    }
    if(tableName != other.tableName){
        return false;
    }
    if(columnNames != other.columnNames){
        return false;
    }
    if(columnTypes != other.columnTypes){
        return false;
    }
    if(values != other.values){
        return false;
    }
    return true;
}

