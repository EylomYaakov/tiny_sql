#  Design notes

## tokenizer
I started by implementing the tokenizer and tokenizer tests. I followed the instructions and created a Token struct containing a value and a type. I also implemented operator== so I would be able to compare Tokens in testing later.

I implemented the splitting mechanism using find_first_of and find_first_not_of functions to find the delimiters, and trimmed each token to remove extra  whitespaces.

At first, I wasn't sure how I should classify each token into a token type. I used only the current token and the previous one but I realized it isn't enough to distinguish between literals and identifiers(column name specifically), so I am now using the whole tokens vector. Also, I am currently distinguishing between literals and identifiers going very specifically into the command. I am checking the command type and the previous token, and I am not sure if its optimal to determine the token type this way. I am trying to think of a way to make it less specific.


## parser

I started by writing the Command class. It has a command type attribute and all the other fields a command could have. I built constructors, getters and setters and operator==(for testing)

After that, I built the actual parsing logic. I identified the command type by the first and second tokens and built a function for each command type. I made sure the command structure is correct, and if not i threw exception(that will later be caught and printed). 

At first, I wasnt sure how to identify if each value is string or integer, because previously in the tokenizer i split by ' and ", removing them from the string. so, i changed the tokenizer splitCommand function to not split by ' and ", and now the quotes are still part of the string. now, i know that if a value starts and ends with quotes its a string, and if not i try to parse it as int. if cant parse it as int, i treat it as a string(to support text values without quotes).
 
 ## Engine
 
I created an Engine class that contains an unordered_map of all the tables. I created a dedicated function for each command and then I created an execute_command function that checks the command type and calls the appropriate function for each command. However, because the select function wasn't void(it returns a table), while all the other functions were void, I didn't know what the return type of the execute_command function should be, so I removed it(and I am thinking about calling each command function from the main).

I also merged the select and selectWhere functions into one function because they were very similar, but I had some bugs when calling this function for SELECT WHERE command. I didn't think about the edge cases of empty rows and empty columns that can happen when using a col = value filter, so I didn't delete them(the empty rows and columns). My function returned a bunch of empty vectors and in my tests I was expecting something else, so my tests failed. I found out the source of the bug by going through the code and using debug prints. I noticed the size of the columns vector I was returning was 4(it has 4 empty columns) while I expected it to be 0, so I figured out what the cause of the bug was and added code that deletes empty columns and rows 
 
## Printer & main


I implemented a simple printer that finds each column's width by finding the longest value that should be printed in that column. After that, when printing, for each value shorter than the width, I add padding spaces at the start so that all column values to have the same width.

After that, I implemented the REPL loop in main. when getting a command I check if it's .quit or .tables, and if not I call the tokenizer and parser functions to create the tokens vector and then Command object, and after that I call an execute_command function that checks the command type and calls the appropriate function from the engine.
  
I had a bug because the exception I threw was char\*(the default type thrown when using ""), and I tried using + for string concatenation which isn't available for char\*, so I created CommandException class that derived from std::exception, and I use it to throw exceptions now
  
## webistes I used for learning
 
 
 
[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) to learn about CMake

[cplusplus](https://cplusplus.com) for cpp refrence

[GoogleTest User's Guide](https://google.github.io/googletest/) for google testing

[W3Schools](https://www.w3schools.com) and [geeksforgeeks](https://www.geeksforgeeks.org) for SQL and c++ learn


