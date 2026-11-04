cmake_minimum_required(VERSION 3.16)
project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


add_executable(lab3 
    src/main.cpp
    src/book_set.cpp
    src/book_set.h
    src/book.cpp
    src/book.h
)

target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(lab3 PRIVATE -Wall -Wextra -Wpedantic)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(lab3_tests
    tests/test_lab3.cpp
    src/book_set.cpp
    src/book.cpp
)

target_include_directories(lab3_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(lab3_tests GTest::gtest_main)
target_compile_options(lab3_tests PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME Lab3Tests COMMAND lab3_tests)