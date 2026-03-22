cmake_minimum_required(VERSION 3.16)
project(tiny_sql)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/engine.cpp src/parser.cpp src/printer.cpp src/table.cpp src/tokenizer.cpp)
add_library(tiny_sql_lib ${SOURCES})

add_executable(tiny_sql src/main.cpp)
target_link_libraries(tiny_sql tiny_sql_lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tokenizerTest tests/test_tokenizer.cpp)
target_link_libraries(tokenizerTest tiny_sql_lib gtest_main)
add_test(NAME tokenizer_test COMMAND tokenizerTest)
