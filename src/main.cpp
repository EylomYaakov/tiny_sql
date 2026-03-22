#include <iostream>
#include <string>
#include "tokenizer.h"

#define EXIT_COMMAND ".quit"
#define WHITESPACES " \t"


int main(int argc, const char * argv[]) {
    std::string command;
    while (true){
        std::cin >> command;
        trim(command);
        if(command == EXIT_COMMAND){
            break;
        }
        //tokenzier logic here
    }
    return EXIT_SUCCESS;
}


