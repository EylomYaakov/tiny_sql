#ifndef tokenizer_h
#define tokenizer_h

#include <iostream>
#include <vector>

#define WHITESPACES " \t"

struct Token{
    enum Type {KEYWORD, IDENTIFIER, LITERAL, SYMBOL} type;
    std::string value;
    Token(const std::string& value, Token::Type type);
    
    //will be used for google testing
    bool operator==(const Token& other) const;
    
};

Token::Type findTokenType(const std::string& value, const std::vector<Token>& tokens);

std::vector<Token> splitCommand(const std::string& command);

bool isKeyword(const std::string& value);

bool isIdentifer(const std::vector<Token>& tokens);

bool isSymbol(const std::string& value);

bool isLiteral(const std::vector<Token>& tokens);

void ltrim(std::string& s);

void rtrim(std::string& s);

void trim(std::string& s);

#endif /* tokenizer_h */
