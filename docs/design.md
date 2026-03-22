#  Design notes

## tokenizer
I started by implementing the tokenizer and tokenizer tests. I followed the instructions and created a Token struct containing a value and a type. I also implemented operator== so I would be able to compare Tokens in testing later.

I implemented the splitting mechanism using find_first_of and find_first_not_of functions to find the delimiters, and trimmed each token to remove extra  whitespaces.

At first, I wasn't sure how I should classify each token into a token type. I used only the current token and the previous one but I realized it isn't enough to distinguish between literals and identifiers(column name specifically), so I am now using the whole tokens vector. Also, I am currently distinguishing between literals and identifiers going very specifically into the command. I am checking the command type and the previous token, and I am not sure if its optimal to determine the token type this way. I am trying to think of a way to make it less specific.
 
 ## webistes I used for learning
 
[CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) to learn about Cmake

[cplusplus](https://cplusplus.com) for cpp refrence

[GoogleTest User's Guide](https://google.github.io/googletest/) for google testing

[W3Schools](https://www.w3schools.com) and [geeksforgeeks](https://www.geeksforgeeks.org) for SQL and c++ learn


