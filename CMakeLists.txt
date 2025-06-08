cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Include directories
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(my_program ${SOURCES})
