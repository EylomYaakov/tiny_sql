#  Design notes

## tokenizer
I started by implementing the tokenizer and tokenizer tests. I followed the instructions and created a Token struct containing a value and a type. I also implemented operator== so I would be able to compare Tokens in testing later.

I implemented the splitting mechanism using find_first_of and find_first_not_of functions to find the delimiters, and trimmed each token to remove extra  whitespaces.

At first, I wasn't sure how I should classify each token into a token type. I used only the current token and the previous one but I realized it isn't enough to distinguish between literals and identifiers(column name specifically), so I am now using the whole tokens vector. Also, I am currently distinguishing between literals and identifiers going very specifically into the command. I am checking the command type and the previous token, and I am not sure if its optimal to determine the token type this way. I am trying to think of a way to make it less specific.


## parser

I started by writing the Command class. It has a command type attribute and all the other fields a command could have. I built constructors, getters and setters and operator==(for testing)

After that, I built the actual parsing logic. I identified the command type by the first and second tokens and built a function for each command type. I made sure the command structure is correct, and if not i threw exception(that will later be caught and printed). 

At first, I wasnt sure how to identify if each value is string or integer, because previously in the tokenizer i split by ' and ", removing them from the string. so, i changed the tokenizer splitCommand function to not split by ' and ", and now the quotes are still part of the string. now, i know that if a value starts and ends with quotes its a string, and if not i try to parse it as int. if cant parse it as int, i treat it as a string(to support text values without quotes).
 
## webistes I used for learning
 
 
 
[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) to learn about Cmake

[cplusplus](https://cplusplus.com) for cpp refrence

[GoogleTest User's Guide](https://google.github.io/googletest/) for google testing

[W3Schools](https://www.w3schools.com) and [geeksforgeeks](https://www.geeksforgeeks.org) for SQL and c++ learn


