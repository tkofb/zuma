cmake_minimum_required(VERSION 3.10.0)
project(zuma VERSION 0.1.0 LANGUAGES C CXX)

include_directories(src/parser)

add_executable(zuma 
    src/main.cpp
    src/parser/futures/futuresTrade.cpp
)

