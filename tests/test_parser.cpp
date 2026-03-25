#include <iostream>

#include "../src/parser.h"
#include <gtest/gtest.h>


TEST(ParserTest, CreateTableTests){
    std::vector<Token> tokens1 = {
        {"CREATE", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"TABLE1", Token::IDENTIFIER},
        {"(", Token::SYMBOL},
        {"Col1", Token::IDENTIFIER},
        {"INTEGER", Token::KEYWORD},
        {")", Token:: SYMBOL}
    };
    Command expected1 = {
        CREATE_TABLE,
        "TABLE1",
        {"Col1"},
        {Column::INTEGER},
        {}
    };
    
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - \"CREATE TABLE TABLE1 (Col1 INTEGER)\"";
    
    std::vector<Token> tokens2 = {
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
    Command expected2 = {
        CREATE_TABLE,
        "table2",
        {"col1", "col2", "col3"},
        {Column::TEXT, Column::TEXT, Column::INTEGER},
        {}
    };
    
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - CREATE TABLE table2 (col1 TEXT, col2 TEXT, col3 INTEGER)";
    
    std::vector<Token> tokens3 = {
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
    Command expected3 = {
        CREATE_TABLE,
        "table3",
        {"col1", "col2"},
        {Column::TEXT, Column::INTEGER},
        {}
    };
    EXPECT_EQ(parseCommand(tokens3), expected3) << "Test 3 failed - CREATE    TALBE  table3 (  col1 TEXT , col2  INTEGER  ";
    
}


TEST(ParserTest, InsertIntoTests){
    std::vector<Token> tokens1 = {
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
    Command expected1 = {
        INSERT_INTO,
        "table1",
        {},
        {},
        {1, 10, 100}
    };
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - INSERT INTO table1 VALUES (1, 10, 100)";
    
    std::vector<Token> tokens2 = {
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
    
    Command expected2 = {
        INSERT_INTO,
        "table2",
        {},
        {},
        {"HI", "HELLO", "HEY"}
    };
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - INSERT INTO table2 VALUES (\'HI\', \"HELLO\", \'HEY\')";
    
    std::vector<Token> tokens3 = {
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
    Command expected3 = {
        INSERT_INTO,
        "table3",
        {},
        {},
        {1, 11}
    };
    EXPECT_EQ(parseCommand(tokens3), expected3) << "Test 3 failed - INSERT  INTO   table3 VALUES  ( 1 , 11    )";
}

TEST(ParserTest, SelectFromTests){
    std::vector<Token> tokens1 = {
        {"SELECT", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
    };
    
    Command expected1 = {
        SELECT,
        "table1",
        {"col1"},
        {},
        {}
    };
    
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - SELECT col1 From table1";
    
    std::vector<Token> tokens2 = {
        {"SELECT", Token::KEYWORD},
        {"\'col1\'", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"col2", Token::IDENTIFIER},
        {",", Token::SYMBOL},
        {"\"col3\"", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"\'table2\'", Token::IDENTIFIER},
    };
    
    Command expected2 = {
        SELECT,
        "table2",
        {"col1", "col2", "col3"},
        {},
        {}
    };
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - SELECT \'col1\', col2, \"col3\" FROM \'table2\'";
    
    std::vector<Token> tokens3 = {
        {"SELECT", Token::KEYWORD},
        {"*", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table3", Token::IDENTIFIER},
    };
    
    Command expected3 = {
        SELECT,
        "table3",
        {"*"},
        {},
        {}
    };
    EXPECT_EQ(parseCommand(tokens3), expected3) << "Test 3 failed - SELECT  *   FROM table3";
}

TEST(ParserTest, SelectWhereTests){
    std::vector<Token> tokens1 = {
        {"SELECT", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col2", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"10", Token::LITERAL},
    };
    
    Command expected1 = {
        SELECT_WHERE,
        "table1",
        {"col1", "col2"},
        {},
        {10}
    };

    
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - SELECT col1 FROM table1 WHERE col2 = 10";
    
    std::vector<Token> tokens2 = {
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
    
    Command expected2 = {
        SELECT_WHERE,
        "table2",
        {"col1", "col2", "col3", "col3"},
        {},
        {"hello"}
    };
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - SELECT   col1, col2, col3  FROM table2 WHERE col3  = \'hello\'";
    
    std::vector<Token> tokens3 = {
        {"SELECT", Token::KEYWORD},
        {"*", Token::IDENTIFIER},
        {"FROM", Token::KEYWORD},
        {"users", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"id", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"5", Token::LITERAL},
    };
    
    Command expected3 = {
        SELECT_WHERE,
        "users",
        {"*", "id"},
        {},
        {5}
    };
    EXPECT_EQ(parseCommand(tokens3), expected3) << "Test 3 failed - SELECT * FROM users WHERE id = 5";
}

TEST(ParserTest, DeleteFromTests){
    std::vector<Token> tokens1 = {
        {"DELETE", Token::KEYWORD},
        {"FROM", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"10", Token::LITERAL},
    };
    
    Command expected1 = {
        DELETE_FROM,
        "table1",
        {"col1"},
        {},
        {10}
    };
    
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - DELETE FROM table1 WHERE col1 = 10";
    
    std::vector<Token> tokens2 = {
        {"DELETE", Token::KEYWORD},
        {"FROM", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        {"WHERE", Token::KEYWORD},
        {"col1", Token::IDENTIFIER},
        {"=", Token::SYMBOL},
        {"\'hello\'", Token::LITERAL},
        
    };
    
    Command expected2 = {
        DELETE_FROM,
        "table2",
        {"col1"},
        {},
        {"hello"}
    };
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - DELETE   FROM  table2 WHERE col1 = \'hello\'";
}

TEST(ParserTest, DropTableTests){
    std::vector<Token> tokens1 = {
        {"DROP", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table1", Token::IDENTIFIER},
    };
    
    Command expected1 = {
        DROP_TABLE,
        "table1",
        {},
        {},
        {}
    };
    
    EXPECT_EQ(parseCommand(tokens1), expected1) << "Test 1 failed - DROP TABLE table1";
    
    std::vector<Token> tokens2 = {
        {"DROP", Token::KEYWORD},
        {"TABLE", Token::KEYWORD},
        {"table2", Token::IDENTIFIER},
        
    };
    
    Command expected2 = {
        DROP_TABLE,
        "table2",
        {},
        {},
        {}
    };
    
    EXPECT_EQ(parseCommand(tokens2), expected2) << "Test 2 failed - DROP    TABLE   table2";
}
