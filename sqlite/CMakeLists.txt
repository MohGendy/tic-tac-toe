cmake_minimum_required(VERSION 3.10)
project(sqlite3 C)

# collect all the .c files from the amalgamation or src/
file(GLOB SQLITE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Build sqlite3 as a static library
add_library(sqlite3 STATIC ${SQLITE_SOURCES})

# Install headers (if you need install() later)
target_include_directories(sqlite3
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional compile-time flags: tweak as you like
# e.g. target_compile_definitions(sqlite3 PRIVATE SQLITE_THREADSAFE=1)

# (You no longer need add_executable here!)