#include "tokenizer.h"


Token::Token(const std::string& value, Token::Type type): value(value), type(type) {}

std::vector<Token> splitCommand(const std::string& command){
    std::vector<Token> tokens;
    std::string delimiters = " \t(),\"\'";
    size_t start = 0, end = 0;
    while(start != std::string::npos && end != std::string::npos){
        end = command.find_first_of(delimiters, start);
        std::string tokenValue = command.substr(start, end-start);
        Token current = Token(tokenValue, findTokenType(tokenValue, tokens));
        tokens.push_back(current);
        start = command.find_first_not_of(delimiters, end+1);
        // if the dellimeter is a symbol, add it to the token list (we will need it to check the command structure later)
        if(end != std::string::npos){
            std::string removedDelimiters =  command.substr(end, start-end);
            // ignore leading and trailing whitespaces
            trim(removedDelimiters);
            if(isSymbol(removedDelimiters)){
                Token symbol = Token(removedDelimiters, Token::SYMBOL);
                tokens.push_back(symbol);
            }
        }
        
    }
    return tokens;
}



Token::Type findTokenType(const std::string& value, const std::vector<Token>& tokens){
    if(isKeyword(value)){
        return Token::KEYWORD;
    }
    if(isIdentifer(tokens)){
        return Token::IDENTIFIER;
    }
    if(isLiteral(tokens)){
        return Token::LITERAL;
    }
    if(isSymbol(value)){
        return Token::SYMBOL;
    }
    throw "unkown value";
}

bool isKeyword(const std::string& value){
    return value == "CREATE" || value == "TABLE" || value == "INSERT" || value == "INTO" || value == "VALUES" || value == "SELECT" || value == "FROM" || value == "WHERE" || value == "DELETE" || value == "DROP" || value == "INTEGER" || value == "TEXT";
}

bool isIdentifer(const std::vector<Token>& tokens){
    // first token is never identifer
    if(tokens.empty()){
        return false;
    }
    Token prevToken = tokens[tokens.size()-1];
    std::string prevValue = prevToken.value;
    Token::Type prevType = prevToken.type;
    bool isTableName = (prevValue == "TABLE" || prevValue == "INTO" || prevValue == "FROM");
    bool isColName = (prevValue == "SELECT" || prevValue == "WHERE");
    // col name can also be after a symbol (',' or '(') in CREATE and SELECT commands
    isColName = isColName || ((tokens[0].value == "CREATE" || tokens[0].value == "SELECT") && (prevValue == "," || prevValue == "("));
    return isTableName || isColName;
}

bool isSymbol(const std::string& value){
    return value == "(" || value == ")" || value == "," || value == "=";
}

bool isLiteral(const std::vector<Token>& tokens){
    // first token is never literal
    if(tokens.empty()){
        return false;
    }
    Token prevToken = tokens[tokens.size()-1];
    std::string prevValue = prevToken.value;
    Token::Type prevType = prevToken.type;
    // literals comes only after '=' or after symbol in INSERT command
    return prevValue == "=" || (tokens[0].value == "INSERT" && prevType == Token::SYMBOL);
}


//remove leading whitespaces
void ltrim(std::string& s){
    s.erase(0, s.find_first_not_of(WHITESPACES));
}


//remove trailing whitespaces
void rtrim(std::string& s){
    s.erase(s.find_last_not_of(WHITESPACES) + 1);
}

void trim(std::string& s){
    ltrim(s);
    rtrim(s);
}



bool Token::operator==(const Token& other) const{
    return other.value == value && other.type == type;
}


