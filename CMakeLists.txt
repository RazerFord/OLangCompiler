cmake_minimum_required (VERSION 3.22)

project(OLangCompiler)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
endif()

include_directories(src)

add_executable(main src/main.cpp src/syntactical-analyzer/details.cpp)

add_executable(lexical-analyzer-starter src/lexical-analyzer-starter.cpp)
