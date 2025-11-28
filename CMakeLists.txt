cmake_minimum_required(VERSION 3.20)
project(cms LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(core
    src/core/record_array.cpp
    src/core/app_state.cpp
    src/core/predicate.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# main program
add_executable(cms src/app/main.cpp)
target_link_libraries(cms PRIVATE core)

# tests
include(CTest)
enable_testing()

add_executable(test_basic tests/test_basic.cpp)
add_test(NAME basic COMMAND test_basic)
