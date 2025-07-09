cmake_minimum_required(VERSION 3.10.0)
project(zuma VERSION 0.1.0 LANGUAGES C CXX)

include_directories(src/parser)
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
 
target_link_libraries(zuma
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)


