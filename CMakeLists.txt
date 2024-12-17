#######################################
#######################################
#############  DUTESIER  ##############
#######################################
#######################################

cmake_minimum_required(VERSION 3.14)

# Define the project name and the programming language
project(Pepino LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
add_compile_options(-Wall -Werror -Wextra)

# Define source files
set(SRC_FILES 
    src/pepino.cpp
    src/Interpreter.cpp
    src/Logger.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Token.cpp
    src/Object.cpp
    src/StepRegistry.cpp
    # src/Environment.cpp
    # src/Function.cpp
    )

# Add the library target
add_library(Pepino ${SRC_FILES})
target_include_directories(Pepino PUBLIC include)

# # Add target for manually testing
# add_executable(PepinoMainTest ${SRC_FILES} main.cpp)
# target_link_libraries(PepinoMainTest PRIVATE Pepino)

# Add Google Test
find_package(GTest)

# Enable testing
enable_testing()

# Add the test executable
add_executable(PepinoTest 
    # tests/AstPrinter_test.cpp
    tests/pepino_test.cpp
    # tests/Interpreter_test.cpp
    tests/Lexer_test.cpp
    # tests/Parser_test.cpp
    # tests/Object_test.cpp
    )


# Link the test executable with the library and Google Test
target_link_libraries(PepinoTest PRIVATE Pepino GTest::gtest_main)

# Add tests
include(GoogleTest)
gtest_discover_tests(PepinoTest)