#include <iostream>
#include <string>
#include "engine.h"
#include "printer.h"

#define EXIT_COMMAND ".quit"
#define LIST_TABLES_COMMAND ".tables"
#define SAVE_COMMAND ".save"
#define LOAD_COMMAND ".load"
#define SAVE_LOAD_COMMANDS_LENGTH 5
#define WHITESPACES " \t"
#define DEFAULT_FILE "tables.csv"

void execute_command(const Command& command, Engine& engine);
void execute_save_load(const std::string& command, Engine& engine);

int main(int argc, const char * argv[]) {
    std::string command;
    Engine engine;
    printWelcome();
    engine.loadTables(DEFAULT_FILE, true);
    while (true){
        getline(std::cin, command);
        trim(command);
        if(command == EXIT_COMMAND){
            engine.saveTables(DEFAULT_FILE);
            break;
        }
        else if(command == LIST_TABLES_COMMAND){
            std::vector<std::string> names = engine.getTableNames();
            printTableNames(names);
        }
        else if(command.starts_with(SAVE_COMMAND) || command.starts_with(LOAD_COMMAND)){
            execute_save_load(command, engine);
        }
        else{
            try{
                std::vector<Token> tokens = splitCommand(command);
                Command parsedCommand = parseCommand(tokens);
                execute_command(parsedCommand, engine);
            }
            catch(const CommandException& e){
                std::cerr << e.what() << std::endl;
            }
        }
    }
    return EXIT_SUCCESS;
}


void execute_command(const Command& command, Engine& engine){
    CommandType type = command.getType();
    if(type == CREATE_TABLE){
        engine.createTable(command);
    }
    else if(type == INSERT_INTO){
        engine.insertIntoTable(command);
    }
    else if(type == SELECT || type == SELECT_WHERE){
        Table table = engine.select(command);
        printTable(table);
    }
    else if(type == DELETE_FROM){
        engine.deleteFromTable(command);
    }
    else if(type == DROP_TABLE){
        engine.dropTable(command);
    }
}

void execute_save_load(const std::string& command, Engine& engine){
    // starting index of the file path(if specified)
    size_t filePathStart = command.find_first_not_of(" ", SAVE_LOAD_COMMANDS_LENGTH);
    std::string filePath = DEFAULT_FILE;
    if(filePathStart != std::string::npos){
        filePath = command.substr(filePathStart, command.length()-filePathStart);
        filePath = parseString(filePath);
    }
    if(command.starts_with(SAVE_COMMAND)){
        engine.saveTables(filePath);
    }
    else{
        engine.loadTables(filePath);
    }
}
