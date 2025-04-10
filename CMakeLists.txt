# CLICreatures CMake Build File by Rylan Kellogg

# Export compile_commands.json for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)

project(
  creat
  VERSION 0.0.0
  LANGUAGES CXX
)

add_library(
  creatlib
  src/creat.cpp
)
target_include_directories(creatlib PUBLIC inc)

add_executable(
  creature
  src/main.cpp
)
target_include_directories(creature PUBLIC inc)
target_link_libraries(creature PUBLIC creatlib)

add_dependencies(creature creatlib)
