#include <iostream>

#include "../src/engine.h"
#include <gtest/gtest.h>



TEST(EngineTest, allCommandsTest){
    Engine engine;
    
    Command createTable = {
        CREATE_TABLE,
        "users",
        {"id", "first name", "last name", "age"},
        {Column::INTEGER, Column::TEXT, Column::TEXT, Column::INTEGER},
        {}
    };
    
    engine.createTable(createTable);
    
    Command insertRow = {
        INSERT_INTO,
        "users",
        {},
        {},
        {1, "Eylom", "Yaakov", 19}
    };
    
    engine.insertIntoTable(insertRow);
    
    insertRow = {
        INSERT_INTO,
        "users",
        {},
        {},
        {2, "Itay", "Cohen", 24}
    };
    
    engine.insertIntoTable(insertRow);
    
    insertRow = {
        INSERT_INTO,
        "users",
        {},
        {},
        {3, "Omer", "Levi", 15},
    };
    
    engine.insertIntoTable(insertRow);

    
    Command selectAll = {
        SELECT,
        "users",
        {"*"},
        {},
        {}
    };
    
    Table result = engine.select(selectAll);
    
    Table expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}},
        {{1, "Eylom", "Yaakov", 19}, {2, "Itay", "Cohen", 24}, {3, "Omer", "Levi", 15}}
    };
    
    EXPECT_EQ(result, expected) << "FAILED TEST 1";
    
    insertRow = {
        INSERT_INTO,
        "users",
        {},
        {},
        {4, "Itay", "Levi", 19},
    };
    
    engine.insertIntoTable(insertRow);
    
    result = engine.select(selectAll);
    
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}},
        {{1, "Eylom", "Yaakov", 19}, {2, "Itay", "Cohen", 24}, {3, "Omer", "Levi", 15}, {4, "Itay", "Levi", 19}}
    };
    
    EXPECT_EQ(result, expected) << "FAILED TEST 2";
    
    Command selectItay = {
        SELECT_WHERE,
        "users",
        {"id", "first name", "age", "first name"},
        {},
        {"Itay"}
    };
    
    result = engine.select(selectItay);
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"age", Column::INTEGER}},
        {{2, "Itay", 24}, {4, "Itay", 19}}
    };

    EXPECT_EQ(result, expected) << "FAILED TEST 3";
    
    Command selectAge19{
        SELECT_WHERE,
        "users",
        {"id", "first name", "age"},
        {},
        {19}
    };
    
    result = engine.select(selectAge19);
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}},
        {{1, "Eylom"}, {4, "Itay"}}
    };
    
    
    EXPECT_EQ(result, expected) <<  "FAILED TEST 4";
    
    // first name should appear 3 times, id should appear 2 times and all the other columns one time
    Command selectAllFewTimes = {
        SELECT,
        "users",
        {"first name", "first name", "*", "id"},
        {},
        {}
    };
    
    result = engine.select(selectAllFewTimes);
    
    
    expected = {
        "users",
        {{"first name", Column:: TEXT}, {"first name", Column:: TEXT}, {"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}, {"id", Column::INTEGER}},
        {{"Eylom", "Eylom", 1, "Eylom", "Yaakov", 19, 1}, {"Itay", "Itay", 2, "Itay", "Cohen", 24, 2}, {"Omer", "Omer", 3, "Omer", "Levi", 15, 3}, {"Itay", "Itay", 4, "Itay", "Levi", 19, 4}}
    };
    
    EXPECT_EQ(result, expected) << "FAILED TEST 5";
    
    Command deleteItay = {
        DELETE_FROM,
        "users",
        {"first name"},
        {},
        {"Itay"}
    };
    
    engine.deleteFromTable(deleteItay);
    
    result = engine.select(selectAll);
    
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}},
        {{1, "Eylom", "Yaakov", 19}, {3, "Omer", "Levi", 15}}
    };
    
    EXPECT_EQ(result, expected) << "FAILED TEST 6";
    
    Command createTable2 = {
        CREATE_TABLE,
        "new users",
        {"email"},
        {Column::TEXT},
        {}
    };
    
    engine.createTable(createTable2);
    
    insertRow = {
        INSERT_INTO,
        "new users",
        {},
        {},
        {"test@gmail.com"},
    };
    
    engine.insertIntoTable(insertRow);
    
    
    // columns should appear twice
    Command selectAllTwice = {
        SELECT,
        "new users",
        {"*", "*"},
        {},
        {}
    };
    
    result = engine.select(selectAllTwice);
    
    
    expected = {
        "new users",
        {{"email", Column::TEXT}, {"email", Column:: TEXT}},
        {{"test@gmail.com", "test@gmail.com"}}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 7";
    
    result = engine.select(selectAll);
    
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}},
        {{1, "Eylom", "Yaakov", 19}, {3, "Omer", "Levi", 15}}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 8";
    
    
    Command selectId5 = {
        SELECT_WHERE,
        "users",
        {"*", "id"},
        {},
        {5}
    };
    
    result = engine.select(selectId5);
    
    // no one has id = 5
    expected = {
        "users",
        {},
        {}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 9";
    
    Command dropTable = {
        DROP_TABLE,
        "users",
        {},
        {},
        {}
    };
    
    engine.dropTable(dropTable);
    
    result = engine.select(selectAllTwice);
    
    
    expected = {
        "new users",
        {{"email", Column::TEXT}, {"email", Column:: TEXT}},
        {{"test@gmail.com", "test@gmail.com"}}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 10";
    
    engine.createTable(createTable);
    
    result = engine.select(selectAll);
    
    // table should be empty
    expected = {
        "users",
        {},
        {}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 11";
    
    insertRow = {
        INSERT_INTO,
        "users",
        {},
        {},
        {1, "Eylom", "Yaakov", 19}
    };
    
    engine.insertIntoTable(insertRow);
    
    result = engine.select(selectAll);
    
    // table should be empty
    expected = {
        "users",
        {{"id", Column::INTEGER}, {"first name", Column:: TEXT}, {"last name", Column::TEXT}, {"age", Column::INTEGER}},
        {{1, "Eylom", "Yaakov", 19}}
    };
    
    
    EXPECT_EQ(result, expected) << "FAILED TEST 12";
}
