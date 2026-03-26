#include <iostream>
#include <string>
#include "engine.h"
#include "printer.h"

#define EXIT_COMMAND ".quit"
#define LIST_TABLES_COMMAND ".tables"
#define WHITESPACES " \t"

void execute_command(const Command& command, Engine& engine);

int main(int argc, const char * argv[]) {
    std::string command;
    Engine engine;
    while (true){
        getline(std::cin, command);
        trim(command);
        if(command == EXIT_COMMAND){
            break;
        }
        if(command == LIST_TABLES_COMMAND){
            std::vector<std::string> names = engine.getTableNames();
            printTableNames(names);
        }
        else{
            try{
                std::vector<Token> tokens = splitCommand(command);
                Command parsedCommand = parseCommand(tokens);
                execute_command(parsedCommand, engine);
            }
            catch(const CommandException& e){
                std::cout << e.what() << std::endl;
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

