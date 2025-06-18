cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Include directories
include_directories(include)
add_subdirectory(googletest)
include_directories(googletest/include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Gather test sources and include game logic files
file(GLOB TEST_SOURCES "tests/*.cpp" "src/gameStructure.cpp" "src/ai.cpp")

# Add executables
add_executable(my_program ${SOURCES})
add_executable(runTests ${TEST_SOURCES})

# Link libraries
target_link_libraries(my_program gtest gtest_main pthread)
target_link_libraries(runTests gtest gtest_main pthread)