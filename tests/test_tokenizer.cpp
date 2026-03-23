#include <iostream>

#include "../src/tokenizer.h"
#include <gtest/gtest.h>



TEST(TokenizerTest, CreateTableTests){
    std::string command1 = "CREATE TABLE TABLE1 (Col1 INTEGER)";
    std::vector<Token> expected1 = {
        {"CREATE", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"TABLE1", Token::IDENTIFIER},
        {"(", Token::SYMBOL},
        {"Col1", Token::IDENTIFIER},
        {"INTEGER", Token::KEYWORD},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - \"CREATE TABLE TABLE1 (Col1 INTEGER)\"";
    
    
    std::string command2 = "CREATE TABLE table2 (col1 TEXT, col2 TEXT, col3 INTEGER)";
    std::vector<Token> expected2 = {
        {"CREATE", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        {"(", Token::SYMBOL},
        {"col1", Token::IDENTIFIER},
        {"TEXT", Token::KEYWORD},
        {",", Token::SYMBOL},
        {"col2", Token::IDENTIFIER},
        {"TEXT", Token::KEYWORD},
        {",", Token::SYMBOL},
        {"col3", Token::IDENTIFIER},
        {"INTEGER", Token::KEYWORD},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - CREATE TABLE table2 (col1 TEXT, col2 TEXT, col3 INTEGER)";
    
    std::string command3 = "CREATE    TABLE  table3 (  col1 TEXT , col2  INTEGER )   ";
    std::vector<Token> expected3 = {
        {"CREATE", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table3", Token::IDENTIFIER},
        {"(", Token::SYMBOL},
        {"col1", Token::IDENTIFIER},
        {"TEXT", Token::KEYWORD},
        {",", Token::SYMBOL},
        {"col2", Token::IDENTIFIER},
        {"INTEGER", Token::KEYWORD},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command3), expected3) << "Test 3 failed - CREATE    TALBE  table3 (  col1 TEXT , col2  INTEGER  ";
    
}


TEST(TokenizerTest, InsertIntoTests){
    std::string command1 = "INSERT INTO table1 VALUES (1, 10, 100)";
    std::vector<Token> expected1 = {
        {"INSERT", Token::KEYWORD},
        {"INTO", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
        {"VALUES", Token::KEYWORD},
        {"(", Token::SYMBOL},
        {"1", Token::LITERAL},
        {",", Token::SYMBOL},
        {"10", Token::LITERAL},
        {",", Token::SYMBOL},
        {"100", Token::LITERAL},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - INSERT INTO table1 VALUES (1, 10, 100)";
    
    std::string command2 = "INSERT INTO table2 VALUES (\'HI\', \"HELLO\", \'HEY\')";
    std::vector<Token> expected2 = {
        {"INSERT", Token::KEYWORD},
        {"INTO", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        {"VALUES", Token::KEYWORD},
        {"(", Token::SYMBOL},
        {"\'HI\'", Token::LITERAL},
        {",", Token::SYMBOL},
        {"\"HELLO\"", Token::LITERAL},
        {",", Token::SYMBOL},
        {"\'HEY\'", Token::LITERAL},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - INSERT INTO table2 VALUES (\'HI\', \"HELLO\", \'HEY\')";
    
    std::string command3 = "INSERT  INTO   table3 VALUES  ( 1 , 11    )";
    std::vector<Token> expected3 = {
        {"INSERT", Token::KEYWORD},
        {"INTO", Token::KEYWORD},
        {"table3", Token::IDENTIFIER},
        {"VALUES", Token::KEYWORD},
        {"(", Token::SYMBOL},
        {"1", Token::LITERAL},
        {",", Token::SYMBOL},
        {"11", Token::LITERAL},
        {")", Token:: SYMBOL}
    };
    EXPECT_EQ(splitCommand(command3), expected3) << "Test 3 failed - INSERT  INTO   table3 VALUES  ( 1 , 11    )";
}

TEST(TokenizerTest, SelectFromTests){
    std::string command1 = "SELECT col1 FROM table1";
    std::vector<Token> expected1 = {
        {"SELECT", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
    };
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - SELECT col1 From table1";
    
    std::string command2 = "SELECT \'col1\', col2, \"col3\" FROM \'table2\'";
    std::vector<Token> expected2 = {
        {"SELECT", Token::KEYWORD},
        {"\'col1\'", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"col2", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"\"col3\"", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"\'table2\'", Token::IDENTIFIER},
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - SELECT \'col1\', col2, \"col3\" FROM \'table2\'";
    
    std::string command3 = "SELECT  *   FROM table3";
    std::vector<Token> expected3 = {
        {"SELECT", Token::KEYWORD},
        {"*", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table3", Token::IDENTIFIER},
    };
    EXPECT_EQ(splitCommand(command3), expected3) << "Test 3 failed - SELECT  *   FROM table3";
}

TEST(TokenizerTest, SelectWhereTests){
    std::string command1 = "SELECT col1 FROM table1 WHERE col2 = 10";
    std::vector<Token> expected1 = {
        {"SELECT", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col2", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"10", Token::LITERAL},
    };
    
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - SELECT col1 FROM table1 WHERE col2 = 10";
    
    std::string command2 = "SELECT   col1, col2, col3  FROM table2 WHERE col3  = \'hello\'";
    std::vector<Token> expected2 = {
        {"SELECT", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"col2", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"col3", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col3", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"\'hello\'", Token::LITERAL},
        
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - SELECT   col1, col2, col3  FROM table2 WHERE col3  = \'hello\'";
    
    std::string command3 = "SELECT * FROM users WHERE id = 5";
    std::vector<Token> expected3 = {
        {"SELECT", Token::KEYWORD},
        {"*", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"users", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"id", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"5", Token::LITERAL},
        
    };
    EXPECT_EQ(splitCommand(command3), expected3) << "Test 3 failed - SELECT * FROM users WHERE id = 5";
}

TEST(TokenizerTest, DeleteFromTests){
    std::string command1 = "DELETE FROM table1 WHERE col1 = 10";
    std::vector<Token> expected1 = {
        {"DELETE", Token::KEYWORD},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"10", Token::LITERAL},
    };
    
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - DELETE FROM table1 WHERE col1 = 10";
    
    std::string command2 = "DELETE   FROM  table2 WHERE col1 = \'hello\'";
    std::vector<Token> expected2 = {
        {"DELETE", Token::KEYWORD},
        {"FROM", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"\'hello\'", Token::LITERAL},
        
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - DELETE   FROM  table2 WHERE col1 = \'hello\'";
}

TEST(TokenizerTest, DropTableTests){
    std::string command1 = "DROP TABLE table1";
    std::vector<Token> expected1 = {
        {"DROP", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
    };
    
    EXPECT_EQ(splitCommand(command1), expected1) << "Test 1 failed - DROP TABLE table1";
    
    std::string command2 = "DROP    TABLE   table2";
    std::vector<Token> expected2 = {
        {"DROP", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        
    };
    EXPECT_EQ(splitCommand(command2), expected2) << "Test 2 failed - DROP    TABLE   table2";
}




