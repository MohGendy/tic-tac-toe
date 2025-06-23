cmake_minimum_required(VERSION 3.10)
project(windows_stub C)

# collect all the .c files from the amalgamation or src/
file(GLOB WINDOWS_STUB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Build windows_stub as a static library
add_library(windows_stub STATIC ${WINDOWS_STUB_SOURCES})

# Install headers (if you need install() later)
target_include_directories(windows_stub
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional compile-time flags: tweak as you like
# e.g. target_compile_definitions(windows_stub PRIVATE SQLITE_THREADSAFE=1)

# (You no longer need add_executable here!)