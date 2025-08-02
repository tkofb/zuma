cmake_minimum_required(VERSION 3.10.0)
include(${CMAKE_TOOLCHAIN_FILE})
project(zuma VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_BUILD_TYPE Release)

include_directories(src/parser)
include_directories(src/tui)
include(FetchContent)


FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
 
FetchContent_MakeAvailable(ftxui)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(zuma ${SOURCES})
find_package(libpqxx CONFIG REQUIRED)
find_library(PQ_LIB pq REQUIRED)
 
target_link_libraries(zuma
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE libpqxx::pqxx
  PRIVATE ${PQ_LIB}
)


