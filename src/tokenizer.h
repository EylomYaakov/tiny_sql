#ifndef tokenizer_h
#define tokenizer_h

#include <iostream>
#include <vector>
#include <exception>

#define WHITESPACES " \t"

struct Token{
    enum Type {KEYWORD, IDENTIFIER, LITERAL, SYMBOL} type;
    std::string value;
    Token(const std::string& value, Token::Type type);
    
    //will be used for google testing
    bool operator==(const Token& other) const;
    
};

Token::Type findTokenType(std::string& value, const std::vector<Token>& tokens);

std::vector<Token> splitCommand(const std::string& command);

bool isKeyword(std::string value);

bool isIdentifer(const std::vector<Token>& tokens);

bool isSymbol(const std::string& value);

bool isLiteral(const std::vector<Token>& tokens);

void ltrim(std::string& s);

void rtrim(std::string& s);

void trim(std::string& s);


// an exception class to throw all command related exceptions
class CommandException: public std::exception{
    std::string message;
public:
    CommandException(const std::string& message);
    virtual const char* what() const noexcept;
};

#endif /* tokenizer_h */
