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
    src/Logger.cpp
    src/BasicTestRunner.cpp
    src/Catch2TestRunner.cpp
    src/TestController.cpp
    src/parsing/Lexer.cpp
    src/parsing/Parser.cpp
    src/parsing/Token.cpp
    )

# Add the library target
add_library(Pepino ${SRC_FILES})
target_include_directories(Pepino PUBLIC include)

# # Add target for manually testing
# add_executable(PepinoMainTest ${SRC_FILES} main.cpp)
# target_link_libraries(PepinoMainTest PRIVATE Pepino)

# Add Google Test
find_package(GTest)

# Add Catch2
find_package(Catch2 3 REQUIRED)


# Link the Catch2 test executable with the library and Catch2
target_link_libraries(Pepino PRIVATE Catch2::Catch2)


# Add the test executable
add_executable(PepinoTest 
    tests/pepino_test.cpp
    tests/step_implementations.cpp
    tests/steps_test.cpp
    tests/lexer_test.cpp
    tests/parser_test.cpp
    )


# Link the test executable with the library and Google Test
target_link_libraries(PepinoTest PRIVATE Pepino GTest::gtest_main)



# Add tests
include(GoogleTest)
gtest_discover_tests(PepinoTest)