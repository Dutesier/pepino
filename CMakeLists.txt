#######################################
#######################################
#############  DUTESIER  ##############
#######################################
#######################################

cmake_minimum_required(VERSION 3.14)

project(Pepino LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Werror -Wextra)

set(SRC_FILES 
    src/pepino.cpp
    src/Logger.cpp
    src/BasicTestRunner.cpp
    src/TestController.cpp
    src/parsing/Lexer.cpp
    src/parsing/Parser.cpp
    src/parsing/Token.cpp
    src/HookRegistry.cpp
    )

add_library(Pepino ${SRC_FILES})

# Expose the public include directory
set(PEPINO_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Pepino
    PUBLIC
        $<BUILD_INTERFACE:${PEPINO_INC_DIR}>
        $<INSTALL_INTERFACE:include>
)

find_package(GTest)

# Add the test executable
add_executable(PepinoTest 
    tests/pepino_test.cpp
    tests/steps_test.cpp
    tests/lexer_test.cpp
    tests/parser_test.cpp
    )
target_link_libraries(PepinoTest PRIVATE Pepino GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(PepinoTest)
